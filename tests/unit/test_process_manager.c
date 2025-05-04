/**
 * @file test_process_manager.c
 * @brief Unit tests for the Process Management System
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../src/kernel/process/process_manager.h"
#include "../../src/kernel/memory/memory_manager.h"
#include "../../src/kernel/hal/hal.h"

/* Mock process entry point */
static void mock_process_entry(void) {
    /* This would never be called in the test */
    printf("Mock process entry point\n");
}

/* Mock thread entry point */
static void* mock_thread_entry(void* arg) {
    /* This would never be called in the test */
    printf("Mock thread entry point with arg %p\n", arg);
    return NULL;
}

/**
 * @brief Test process manager initialization
 */
static void test_pm_init(void) {
    printf("Testing pm_init...\n");
    
    /* Initialize HAL and memory manager first */
    hal_init();
    mm_init(0);
    
    /* Initialize process manager */
    bool result = pm_init(100); /* 100 max processes */
    assert(result == true);
    
    /* Get process stats */
    ProcessStats stats;
    pm_get_stats(&stats);
    
    /* Verify stats are initialized */
    assert(stats.total_processes == 0);
    assert(stats.total_threads == 0);
    assert(stats.total_entanglements == 0);
    
    printf("pm_init test passed!\n");
}

/**
 * @brief Test process creation and termination
 */
static void test_process_creation(void) {
    printf("\nTesting process creation and termination...\n");
    
    /* Create a process */
    ProcessParams params = {
        .name = "TestProcess",
        .entry_point = (HalVirtualAddr)mock_process_entry,
        .stack_size = 64 * 1024, /* 64KB stack */
        .heap_size = 256 * 1024, /* 256KB heap */
        .priority = PRIORITY_NORMAL,
        .quantum_capable = true,
        .resonance_level = NODE_TECHNOLOGIST
    };
    
    ProcessId process_id;
    bool result = pm_create_process(&params, &process_id);
    assert(result == true);
    assert(process_id != 0);
    
    /* Get process info */
    Process* process = pm_get_process(process_id);
    assert(process != NULL);
    assert(strcmp(process->name, "TestProcess") == 0);
    assert(process->state == PROCESS_CREATED);
    assert(process->priority == PRIORITY_NORMAL);
    assert(process->resonance_level == NODE_TECHNOLOGIST);
    assert(process->thread_count == 1); /* Should have a main thread */
    
    /* Get process stats */
    ProcessStats stats;
    pm_get_stats(&stats);
    assert(stats.total_processes == 1);
    assert(stats.total_threads == 1);
    
    /* Terminate the process */
    result = pm_terminate_process(process_id, 0);
    assert(result == true);
    
    /* Verify process is gone */
    process = pm_get_process(process_id);
    assert(process == NULL);
    
    /* Get updated stats */
    pm_get_stats(&stats);
    assert(stats.total_processes == 0);
    assert(stats.total_threads == 0);
    
    printf("Process creation and termination test passed!\n");
}

/**
 * @brief Test thread creation and management
 */
static void test_thread_management(void) {
    printf("\nTesting thread management...\n");
    
    /* Create a process */
    ProcessParams process_params = {
        .name = "ThreadTestProcess",
        .entry_point = (HalVirtualAddr)mock_process_entry,
        .stack_size = 64 * 1024,
        .heap_size = 256 * 1024,
        .priority = PRIORITY_NORMAL,
        .quantum_capable = true,
        .resonance_level = NODE_MATRIX_ARCHITECT
    };
    
    ProcessId process_id;
    bool result = pm_create_process(&process_params, &process_id);
    assert(result == true);
    
    /* Create additional threads */
    ThreadParams thread_params = {
        .process_id = process_id,
        .entry_point = (HalVirtualAddr)mock_thread_entry,
        .arg = NULL,
        .stack_size = 32 * 1024, /* 32KB stack */
        .priority = PRIORITY_HIGH,
        .quantum_capable = true
    };
    
    ThreadId thread_id1, thread_id2;
    
    result = pm_create_thread(&thread_params, &thread_id1);
    assert(result == true);
    assert(thread_id1 != 0);
    
    result = pm_create_thread(&thread_params, &thread_id2);
    assert(result == true);
    assert(thread_id2 != 0);
    assert(thread_id2 != thread_id1);
    
    /* Get process info */
    Process* process = pm_get_process(process_id);
    assert(process != NULL);
    assert(process->thread_count == 3); /* Main thread + 2 new threads */
    
    /* Get thread information */
    Thread* thread1 = pm_get_thread(thread_id1);
    assert(thread1 != NULL);
    assert(thread1->process_id == process_id);
    assert(thread1->priority == PRIORITY_HIGH);
    assert(thread1->state == THREAD_CREATED);
    
    /* Test thread priority change */
    result = pm_set_thread_priority(thread_id1, PRIORITY_HIGHEST);
    assert(result == true);
    
    thread1 = pm_get_thread(thread_id1);
    assert(thread1->priority == PRIORITY_HIGHEST);
    
    /* Get list of threads for the process */
    Thread* threads[10];
    uint32_t thread_count = pm_get_process_threads(process_id, threads, 10);
    assert(thread_count == 3);
    
    /* Terminate one thread */
    result = pm_terminate_thread(thread_id1, 0);
    assert(result == true);
    
    /* Verify thread is gone */
    thread1 = pm_get_thread(thread_id1);
    assert(thread1 == NULL);
    
    /* Get updated process info */
    process = pm_get_process(process_id);
    assert(process != NULL);
    assert(process->thread_count == 2); /* Main thread + 1 remaining thread */
    
    /* Terminate the process (will terminate all remaining threads) */
    result = pm_terminate_process(process_id, 0);
    assert(result == true);
    
    printf("Thread management test passed!\n");
}

/**
 * @brief Test process entanglement
 */
static void test_process_entanglement(void) {
    printf("\nTesting process entanglement...\n");
    
    /* Create two processes */
    ProcessParams process_params1 = {
        .name = "EntanglementProcess1",
        .entry_point = (HalVirtualAddr)mock_process_entry,
        .stack_size = 64 * 1024,
        .heap_size = 128 * 1024,
        .priority = PRIORITY_NORMAL,
        .quantum_capable = true,
        .resonance_level = NODE_QUANTUM_GUARDIAN
    };
    
    ProcessParams process_params2 = {
        .name = "EntanglementProcess2",
        .entry_point = (HalVirtualAddr)mock_process_entry,
        .stack_size = 64 * 1024,
        .heap_size = 128 * 1024,
        .priority = PRIORITY_HIGH,
        .quantum_capable = true,
        .resonance_level = NODE_PORTAL_TECHNICIAN
    };
    
    ProcessId process_id1, process_id2;
    bool result1 = pm_create_process(&process_params1, &process_id1);
    bool result2 = pm_create_process(&process_params2, &process_id2);
    assert(result1 == true && result2 == true);
    
    /* Create entanglement between the processes */
    uint64_t entanglement_id = pm_create_process_entanglement(
        process_id1, process_id2, ENTANGLE_STATE, NODE_SINGULARITY);
    
    /* Entanglement might fail if hardware doesn't support quantum operations */
    const HalOperations* hal_ops = hal_get_operations();
    if (hal_ops->has_quantum_support && hal_ops->has_quantum_support()) {
        assert(entanglement_id != 0);
        
        /* Get process info */
        Process* process1 = pm_get_process(process_id1);
        Process* process2 = pm_get_process(process_id2);
        
        assert(process1->entanglement_id == entanglement_id);
        assert(process2->entanglement_id == entanglement_id);
        assert(process1->state == PROCESS_QUANTUM);
        assert(process2->state == PROCESS_QUANTUM);
        
        /* Synchronize the entanglement */
        result1 = pm_sync_process_entanglement(entanglement_id);
        assert(result1 == true);
        
        /* Break the entanglement */
        result1 = pm_break_process_entanglement(entanglement_id);
        assert(result1 == true);
        
        /* Verify entanglement is broken */
        process1 = pm_get_process(process_id1);
        process2 = pm_get_process(process_id2);
        
        assert(process1->entanglement_id == 0);
        assert(process2->entanglement_id == 0);
        assert(process1->state == PROCESS_READY);
        assert(process2->state == PROCESS_READY);
        
        printf("Process entanglement test passed!\n");
    } else {
        printf("Skipping process entanglement test - hardware doesn't support quantum operations\n");
    }
    
    /* Terminate the processes */
    pm_terminate_process(process_id1, 0);
    pm_terminate_process(process_id2, 0);
}

/**
 * @brief Test process statistics
 */
static void test_process_stats(void) {
    printf("\nTesting process statistics...\n");
    
    /* Print current process statistics */
    pm_print_stats();
    
    /* Get process stats */
    ProcessStats stats;
    pm_get_stats(&stats);
    
    /* Verify basic stats integrity */
    assert(stats.total_processes == 0);
    assert(stats.total_threads == 0);
    
    printf("Process statistics test passed!\n");
}

/**
 * @brief Test process manager shutdown
 */
static void test_pm_shutdown(void) {
    printf("\nTesting pm_shutdown...\n");
    
    /* Create a process that won't be explicitly terminated */
    ProcessParams params = {
        .name = "ShutdownTestProcess",
        .entry_point = (HalVirtualAddr)mock_process_entry,
        .stack_size = 64 * 1024,
        .heap_size = 128 * 1024,
        .priority = PRIORITY_NORMAL,
        .quantum_capable = false,
        .resonance_level = NODE_ZERO_POINT
    };
    
    ProcessId process_id;
    pm_create_process(&params, &process_id);
    
    /* Shutdown process manager */
    pm_shutdown();
    
    /* Verify process is gone */
    Process* process = pm_get_process(process_id);
    assert(process == NULL);
    
    /* Get process stats */
    ProcessStats stats;
    pm_get_stats(&stats);
    assert(stats.total_processes == 0);
    assert(stats.total_threads == 0);
    
    /* Shutdown memory manager and HAL */
    mm_shutdown();
    hal_shutdown();
    
    printf("pm_shutdown test passed!\n");
}

/**
 * @brief Main test function
 */
int main(void) {
    printf("Running Process Management System tests...\n\n");
    
    test_pm_init();
    test_process_creation();
    test_thread_management();
    test_process_entanglement();
    test_process_stats();
    test_pm_shutdown();
    
    printf("\nAll Process Management System tests passed!\n");
    
    return 0;
}