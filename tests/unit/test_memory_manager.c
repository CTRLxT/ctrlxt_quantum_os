/**
 * @file test_memory_manager.c
 * @brief Unit tests for the Memory Management System
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../src/kernel/memory/memory_manager.h"
#include "../../src/kernel/hal/hal.h"

/**
 * @brief Test memory manager initialization
 */
static void test_mm_init(void) {
    printf("Testing mm_init...\n");
    
    /* Initialize HAL first */
    hal_init();
    
    /* Initialize memory manager */
    bool result = mm_init(0); /* Use default memory limit */
    assert(result == true);
    
    /* Get memory stats */
    MemoryStats stats;
    mm_get_stats(&stats);
    
    /* Verify stats are initialized */
    assert(stats.total_physical > 0);
    assert(stats.free_physical > 0);
    assert(stats.total_regions == 0);
    assert(stats.total_entanglements == 0);
    
    printf("mm_init test passed!\n");
}

/**
 * @brief Test virtual memory allocation and freeing
 */
static void test_mm_virtual_memory(void) {
    printf("\nTesting virtual memory operations...\n");
    
    /* Get initial memory stats */
    MemoryStats stats_before;
    mm_get_stats(&stats_before);
    
    /* Allocate virtual memory */
    const uint64_t TEST_SIZE = 4096;
    HalVirtualAddr addr = mm_alloc_virtual(TEST_SIZE, MEMORY_TYPE_RAM, MM_FLAG_READ | MM_FLAG_WRITE);
    assert(addr != NULL);
    
    /* Get memory stats after allocation */
    MemoryStats stats_after_alloc;
    mm_get_stats(&stats_after_alloc);
    
    /* Verify stats changed correctly */
    assert(stats_after_alloc.total_regions == stats_before.total_regions + 1);
    assert(stats_after_alloc.used_virtual >= stats_before.used_virtual + TEST_SIZE);
    
    /* Get region info */
    MemoryRegion* region = mm_get_region_info(addr);
    assert(region != NULL);
    assert(region->size == TEST_SIZE);
    assert(region->type == MEMORY_TYPE_RAM);
    assert(region->flags == (MM_FLAG_READ | MM_FLAG_WRITE));
    
    /* Write to the memory */
    memset(addr, 0xAA, TEST_SIZE);
    
    /* Free the memory */
    bool free_result = mm_free_virtual(addr);
    assert(free_result == true);
    
    /* Get memory stats after freeing */
    MemoryStats stats_after_free;
    mm_get_stats(&stats_after_free);
    
    /* Verify stats returned to original values */
    assert(stats_after_free.total_regions == stats_before.total_regions);
    
    printf("Virtual memory operations test passed!\n");
}

/**
 * @brief Test memory entanglement
 */
static void test_mm_entanglement(void) {
    printf("\nTesting memory entanglement...\n");
    
    /* Allocate two memory regions */
    const uint64_t TEST_SIZE = 1024;
    HalVirtualAddr addr1 = mm_alloc_virtual(TEST_SIZE, MEMORY_TYPE_RAM, MM_FLAG_READ | MM_FLAG_WRITE | MM_FLAG_QUANTUM);
    assert(addr1 != NULL);
    
    HalVirtualAddr addr2 = mm_alloc_virtual(TEST_SIZE, MEMORY_TYPE_RAM, MM_FLAG_READ | MM_FLAG_WRITE | MM_FLAG_QUANTUM);
    assert(addr2 != NULL);
    
    /* Write different patterns to each region */
    memset(addr1, 0xAA, TEST_SIZE);
    memset(addr2, 0x55, TEST_SIZE);
    
    /* Create entanglement */
    uint64_t entanglement_id = mm_create_entanglement(addr1, addr2, NODE_QUANTUM_GUARDIAN);
    
    /* Entanglement might fail if hardware doesn't support quantum operations */
    const HalOperations* hal_ops = hal_get_operations();
    if (hal_ops->has_quantum_support && hal_ops->has_quantum_support()) {
        assert(entanglement_id != 0);
        
        /* Get entanglement info */
        EntanglementInfo info;
        bool get_info_result = mm_get_entanglement_info(entanglement_id, &info);
        assert(get_info_result == true);
        assert(info.resonance_level == NODE_QUANTUM_GUARDIAN);
        assert(info.is_synchronized == true);
        
        /* Verify regions are now synchronized (should have addr1's pattern) */
        unsigned char* data1 = (unsigned char*)addr1;
        unsigned char* data2 = (unsigned char*)addr2;
        assert(data1[0] == data2[0]);
        
        /* Modify first region */
        memset(addr1, 0xCC, TEST_SIZE);
        
        /* Synchronize the entanglement */
        bool sync_result = mm_sync_entanglement(entanglement_id);
        assert(sync_result == true);
        
        /* Verify both regions have the new pattern */
        assert(data2[0] == 0xCC);
        
        /* Break the entanglement */
        bool break_result = mm_break_entanglement(entanglement_id);
        assert(break_result == true);
        
        /* Memory regions should no longer be entangled */
        MemoryRegion* region1 = mm_get_region_info(addr1);
        assert(region1->entanglement_id == 0);
        
        MemoryRegion* region2 = mm_get_region_info(addr2);
        assert(region2->entanglement_id == 0);
        
        printf("Memory entanglement test passed!\n");
    } else {
        printf("Skipping memory entanglement test - hardware doesn't support quantum operations\n");
    }
    
    /* Free the memory regions */
    mm_free_virtual(addr1);
    mm_free_virtual(addr2);
}

/**
 * @brief Test memory statistics
 */
static void test_mm_stats(void) {
    printf("\nTesting memory statistics...\n");
    
    /* Print current memory statistics */
    mm_print_stats();
    
    /* Get memory stats */
    MemoryStats stats;
    mm_get_stats(&stats);
    
    /* Verify basic stats integrity */
    assert(stats.total_physical > 0);
    assert(stats.total_physical >= stats.used_physical);
    assert(stats.free_physical == stats.total_physical - stats.used_physical);
    
    printf("Memory statistics test passed!\n");
}

/**
 * @brief Test memory manager shutdown
 */
static void test_mm_shutdown(void) {
    printf("\nTesting mm_shutdown...\n");
    
    /* Shutdown memory manager */
    mm_shutdown();
    
    /* Shutdown HAL */
    hal_shutdown();
    
    printf("mm_shutdown test passed!\n");
}

/**
 * @brief Main test function
 */
int main(void) {
    printf("Running Memory Management System tests...\n\n");
    
    test_mm_init();
    test_mm_virtual_memory();
    test_mm_entanglement();
    test_mm_stats();
    test_mm_shutdown();
    
    printf("\nAll Memory Management System tests passed!\n");
    
    return 0;
}