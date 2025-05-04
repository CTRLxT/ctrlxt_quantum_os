/**
 * @file test_hal.c
 * @brief Unit tests for the Hardware Abstraction Layer
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../src/kernel/hal/hal.h"

/**
 * @brief Test HAL initialization
 */
static void test_hal_init(void) {
    printf("Testing hal_init...\n");
    
    bool result = hal_init();
    assert(result == true);
    
    printf("hal_init test passed!\n");
}

/**
 * @brief Test getting architecture name
 */
static void test_hal_get_architecture_name(void) {
    printf("\nTesting hal_get_architecture_name...\n");
    
    const char* arch_name = hal_get_architecture_name();
    assert(arch_name != NULL);
    assert(arch_name[0] != '\0');
    
    printf("Architecture name: %s\n", arch_name);
    printf("hal_get_architecture_name test passed!\n");
}

/**
 * @brief Test getting processor information
 */
static void test_hal_get_processor_info(void) {
    printf("\nTesting hal_get_processor_info...\n");
    
    const HalOperations* ops = hal_get_operations();
    assert(ops != NULL);
    assert(ops->get_processor_info != NULL);
    
    HalProcessorInfo proc_info;
    ops->get_processor_info(&proc_info);
    
    printf("Processor vendor: %s\n", proc_info.vendor);
    printf("Processor model: %s\n", proc_info.model);
    printf("Core count: %u\n", proc_info.core_count);
    printf("Frequency: %llu MHz\n", (unsigned long long)proc_info.frequency_mhz);
    printf("Has quantum unit: %s\n", proc_info.has_quantum_unit ? "Yes" : "No");
    printf("Resonance level: %d\n", proc_info.resonance_level);
    
    printf("hal_get_processor_info test passed!\n");
}

/**
 * @brief Test getting memory information
 */
static void test_hal_get_memory_info(void) {
    printf("\nTesting hal_get_memory_info...\n");
    
    const HalOperations* ops = hal_get_operations();
    assert(ops != NULL);
    assert(ops->get_memory_info != NULL);
    
    HalMemoryInfo mem_info;
    ops->get_memory_info(&mem_info);
    
    printf("Total physical memory: %llu bytes\n", (unsigned long long)mem_info.total_physical);
    printf("Available physical memory: %llu bytes\n", (unsigned long long)mem_info.available_physical);
    printf("Page size: %u bytes\n", mem_info.page_size);
    printf("Supports quantum entanglement: %s\n", mem_info.supports_entanglement ? "Yes" : "No");
    
    if (mem_info.supports_entanglement) {
        printf("Total quantum memory: %llu qubits\n", (unsigned long long)mem_info.total_quantum);
        printf("Available quantum memory: %llu qubits\n", (unsigned long long)mem_info.available_quantum);
        printf("Entanglement limit: %u regions\n", mem_info.entanglement_limit);
    }
    
    printf("hal_get_memory_info test passed!\n");
}

/**
 * @brief Test quantum support detection
 */
static void test_hal_quantum_support(void) {
    printf("\nTesting hal_has_quantum_support...\n");
    
    const HalOperations* ops = hal_get_operations();
    assert(ops != NULL);
    assert(ops->has_quantum_support != NULL);
    
    bool has_quantum = ops->has_quantum_support();
    printf("System has quantum support: %s\n", has_quantum ? "Yes" : "No");
    
    printf("hal_has_quantum_support test passed!\n");
}

/**
 * @brief Test HAL shutdown
 */
static void test_hal_shutdown(void) {
    printf("\nTesting hal_shutdown...\n");
    
    hal_shutdown();
    
    printf("hal_shutdown test passed!\n");
}

/**
 * @brief Main test function
 */
int main(void) {
    printf("Running Hardware Abstraction Layer tests...\n\n");
    
    test_hal_init();
    test_hal_get_architecture_name();
    test_hal_get_processor_info();
    test_hal_get_memory_info();
    test_hal_quantum_support();
    test_hal_shutdown();
    
    printf("\nAll Hardware Abstraction Layer tests passed!\n");
    
    return 0;
}