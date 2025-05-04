/**
 * @file test_scheduler.c
 * @brief Unit tests for the Process Scheduler
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../src/kernel/process/scheduler.h"
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
 * @brief Create a test process with multiple threads
 */
static ProcessId create_test_process(const char* name, int thread_count) {
    /* Create a process */
    ProcessParams process_params = {
        .name = {0}, /* Will be filled in */
        .entry_point = (HalVirtualAddr)mock_process_entry,
        .stack_size = 64 * 1024, /* 64KB stack */
        .heap_size = 256 * 1024, /* 256KB heap */
        .priority = PRIORITY_NORMAL,
        .quantum_capable = true,
        .resonance_level = NODE_TECHNOLOGIST
    };
    
    /* Copy name with bounds checking */
    strncpy(process_params.name, name, sizeof(process_params.name) - 1);
    
    ProcessId process_id;
    bool result = pm_create_process(&process_params, &process_id);
    assert(result == true);
    
    /* Create additional threads if requested */
    for (int i = 1; i < thread_count; i++) {
        ThreadParams thread_params = {
            .process_id = process_id,
            .entry_point = (HalVirtualAddr)mock_thread_entry,
            .arg = NULL,
            .stack_size = 32 * 1024, /* 32KB stack */
            .priority = PRIORITY_NORMAL,
            .quantum_capable = true
        };
        
        ThreadId thread_id;
        result = pm_create_thread(&thread_params, &thread_id);
        assert(result == true);
    }
    
    return process_id;
}

/**
 * @brief Test scheduler initialization
 */
static void test_scheduler_init(void) {
    printf("Testing scheduler_init...\n");
    
    /* Initialize HAL, memory manager, and process manager first */
    hal_init();
    mm_init(0);
    pm_init(100);
    
    /* Initialize scheduler */
    bool result = scheduler_init(SCHEDULER_ROUND_ROBIN, 10000000, true); /* 10ms quantum */
    assert(result == true);
    
    /* Get scheduler state */
    SchedulerState state;
    scheduler_get_state(&state);
    
    /* Verify state */
    assert(state.type == SCHEDULER_ROUND_ROBIN);
    assert(state.time_slice == 10000000);
    assert(state.preemption_enabled == true);
    assert(state.current_process == 0);
    assert(state.current_thread == 0);
    
    printf("scheduler_init test passed!\n");
}

/**
 * @brief Test adding threads to scheduler
 */
static void test_scheduler_add_thread(void) {
    printf("\nTesting scheduler_add_thread...\n");
    
    /* Create two test processes with one thread each */
    ProcessId process_id1 = create_test_process("SchedulerTest1", 1);
    ProcessId process_id2 = create_test_process("SchedulerTest2", 1);
    
    /* Get threads */
    Thread* threads1[10];
    Thread* threads2[10];
    uint32_t thread_count1 = pm_get_process_threads(process_id1, threads1, 10);
    uint32_t thread_count2 = pm_get_process_threads(process_id2, threads2, 10);
    
    assert(thread_count1 == 1);
    assert(thread_count2 == 1);
    
    ThreadId thread_id1 = threads1[0]->id;
    ThreadId thread_id2 = threads2[0]->id;
    
    /* Add threads to scheduler */
    bool result = scheduler_add_thread(thread_id1);
    assert(result == true);
    
    result = scheduler_add_thread(thread_id2);
    assert(result == true);
    
    /* Set different priorities */
    result = scheduler_set_thread_priority(thread_id1, PRIORITY_HIGH);
    assert(result == true);
    
    /* Remove a thread */
    result = scheduler_remove_thread(thread_id2);
    assert(result == true);
    
    /* Try to remove again (should fail) */
    result = scheduler_remove_thread(thread_id2);
    assert(result == false);
    
    printf("scheduler_add_thread test passed!\n");
}

/**
 * @brief Test starting and stopping the scheduler
 */
static void test_scheduler_start_stop(void) {
    printf("\nTesting scheduler_start and scheduler_stop...\n");
    
    /* Start the scheduler */
    bool result = scheduler_start();
    assert(result == true);
    
    /* Verify scheduler is running */
    SchedulerState state;
    scheduler_get_state(&state);
    assert(state.current_thread != 0); /* Should have scheduled a thread */
    
    /* Perform a manual context switch */
    result = scheduler_context_switch(true);
    assert(result == true);
    
    /* Stop the scheduler */
    result = scheduler_stop();
    assert(result == true);
    
    /* Verify scheduler is stopped */
    scheduler_get_state(&state);
    assert(state.current_thread == 0);
    
    printf("scheduler_start and scheduler_stop test passed!\n");
}

/**
 * @brief Test blocking and unblocking threads
 */
static void test_scheduler_block_unblock(void) {
    printf("\nTesting scheduler_block_thread and scheduler_unblock_thread...\n");
    
    /* Create a test process with three threads */
    ProcessId process_id = create_test_process("BlockTest", 3);
    
    /* Get threads */
    Thread* threads[10];
    uint32_t thread_count = pm_get_process_threads(process_id, threads, 10);
    assert(thread_count == 3);
    
    /* Add all threads to scheduler */
    for (uint32_t i = 0; i < thread_count; i++) {
        scheduler_add_thread(threads[i]->id);
    }
    
    /* Start the scheduler */
    scheduler_start();
    
    /* Block one thread */
    bool result = scheduler_block_thread(threads[1]->id);
    assert(result == true);
    
    /* Verify thread is blocked */
    Thread* thread = pm_get_thread(threads[1]->id);
    assert(thread->state == THREAD_BLOCKED);
    
    /* Unblock the thread */
    result = scheduler_unblock_thread(threads[1]->id);
    assert(result == true);
    
    /* Verify thread is ready */
    thread = pm_get_thread(threads[1]->id);
    assert(thread->state == THREAD_READY);
    
    /* Stop the scheduler */
    scheduler_stop();
    
    printf("scheduler_block_thread and scheduler_unblock_thread test passed!\n");
}

/**
 * @brief Test quantum superposition
 */
static void test_scheduler_superposition(void) {
    printf("\nTesting quantum superposition...\n");
    
    /* Check if quantum operations are supported */
    const HalOperations* hal_ops = hal_get_operations();
    if (hal_ops->has_quantum_support && hal_ops->has_quantum_support()) {
        /* Create a test process with one thread */
        ProcessId process_id = create_test_process("SuperpositionTest", 1);
        
        /* Get thread */
        Thread* threads[1];
        uint32_t thread_count = pm_get_process_threads(process_id, threads, 1);
        assert(thread_count == 1);
        
        ThreadId thread_id = threads[0]->id;
        
        /* Change scheduler to quantum type */
        bool result = scheduler_change_type(SCHEDULER_QUANTUM);
        assert(result == true);
        
        /* Create superposition */
        result = scheduler_create_superposition(thread_id, NODE_QUANTUM_GUARDIAN);
        assert(result == true);
        
        /* Verify thread is in quantum state */
        Thread* thread = pm_get_thread(thread_id);
        assert(thread->state == THREAD_QUANTUM);
        
        /* Add thread to scheduler */
        scheduler_add_thread(thread_id);
        
        /* Start scheduler */
        scheduler_start();
        
        /* Perform some context switches */
        for (int i = 0; i < 5; i++) {
            scheduler_context_switch(true);
        }
        
        /* Collapse superposition */
        result = scheduler_collapse_superposition(thread_id, 0.8); /* Bias towards survival */
        assert(result == true);
        
        /* Thread may or may not have survived collapse - check its state */
        thread = pm_get_thread(thread_id);
        if (thread) {
            printf("Thread survived quantum collapse with state: %d\n", thread->state);
        } else {
            printf("Thread did not survive quantum collapse\n");
        }
        
        /* Stop scheduler */
        scheduler_stop();
        
        printf("Quantum superposition test passed!\n");
    } else {
        printf("Skipping quantum superposition test - hardware doesn't support quantum operations\n");
    }
}

/**
 * @brief Test changing scheduler type
 */
static void test_scheduler_change_type(void) {
    printf("\nTesting scheduler_change_type...\n");
    
    /* Scheduler should be stopped at this point */
    bool result = scheduler_change_type(SCHEDULER_PRIORITY);
    assert(result == true);
    
    /* Verify type changed */
    SchedulerState state;
    scheduler_get_state(&state);
    assert(state.type == SCHEDULER_PRIORITY);
    
    /* Change back to round robin */
    result = scheduler_change_type(SCHEDULER_ROUND_ROBIN);
    assert(result == true);
    
    printf("scheduler_change_type test passed!\n");
}

/**
 * @brief Test resonance level setting
 */
static void test_scheduler_resonance(void) {
    printf("\nTesting scheduler_set_resonance_level...\n");
    
    /* Set resonance level */
    bool result = scheduler_set_resonance_level(NODE_MATRIX_ARCHITECT);
    assert(result == true);
    
    /* Verify level changed */
    SchedulerState state;
    scheduler_get_state(&state);
    assert(state.resonance_level == NODE_MATRIX_ARCHITECT);
    
    printf("scheduler_set_resonance_level test passed!\n");
}

/**
 * @brief Test scheduler shutdown
 */
static void test_scheduler_shutdown(void) {
    printf("\nTesting scheduler_shutdown...\n");
    
    /* Shutdown scheduler */
    scheduler_shutdown();
    
    /* Clean up - terminate all test processes */
    Process* processes[100];
    uint32_t count = pm_get_all_processes(processes, 100);
    
    for (uint32_t i = 0; i < count; i++) {
        pm_terminate_process(processes[i]->id, 0);
    }
    
    /* Shutdown other components */
    pm_shutdown();
    mm_shutdown();
    hal_shutdown();
    
    printf("scheduler_shutdown test passed!\n");
}

/**
 * @brief Main test function
 */
int main(void) {
    printf("Running Process Scheduler tests...\n\n");
    
    test_scheduler_init();
    test_scheduler_add_thread();
    test_scheduler_start_stop();
    test_scheduler_block_unblock();
    test_scheduler_superposition();
    test_scheduler_change_type();
    test_scheduler_resonance();
    test_scheduler_shutdown();
    
    printf("\nAll Process Scheduler tests passed!\n");
    
    return 0;
}