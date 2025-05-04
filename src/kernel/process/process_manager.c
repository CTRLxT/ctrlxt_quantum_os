/**
 * @file process_manager.c
 * @brief Process Management System implementation
 */

#include "process_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Process management state */
static bool pm_initialized = false;
static uint32_t pm_max_processes = 0;
static ProcessStats pm_stats = {0};

/* Process and thread tracking */
static Process* process_list_head = NULL;
static uint64_t next_process_id = 1;
static uint64_t next_thread_id = 1;

/* Process entanglement tracking */
#define MAX_PROCESS_ENTANGLEMENTS 128
static ProcessEntanglement process_entanglements[MAX_PROCESS_ENTANGLEMENTS];
static uint64_t next_entanglement_id = 1;

/**
 * @brief Find a process by ID
 */
static Process* find_process(ProcessId process_id) {
    Process* process = process_list_head;
    
    while (process) {
        if (process->id == process_id) {
            return process;
        }
        process = process->next;
    }
    
    return NULL;
}

/**
 * @brief Find a thread by ID
 */
static Thread* find_thread(ThreadId thread_id) {
    Process* process = process_list_head;
    
    while (process) {
        Thread* thread = process->threads;
        while (thread) {
            if (thread->id == thread_id) {
                return thread;
            }
            thread = thread->next;
        }
        process = process->next;
    }
    
    return NULL;
}

/**
 * @brief Find a thread by ID within a specific process
 */
static Thread* find_thread_in_process(ProcessId process_id, ThreadId thread_id) {
    Process* process = find_process(process_id);
    if (!process) {
        return NULL;
    }
    
    Thread* thread = process->threads;
    while (thread) {
        if (thread->id == thread_id) {
            return thread;
        }
        thread = thread->next;
    }
    
    return NULL;
}

/**
 * @brief Find a process entanglement by ID
 */
static ProcessEntanglement* find_entanglement(uint64_t entanglement_id) {
    for (int i = 0; i < MAX_PROCESS_ENTANGLEMENTS; i++) {
        if (process_entanglements[i].id == entanglement_id) {
            return &process_entanglements[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Find a free entanglement slot
 */
static ProcessEntanglement* find_free_entanglement_slot(void) {
    for (int i = 0; i < MAX_PROCESS_ENTANGLEMENTS; i++) {
        if (process_entanglements[i].id == 0) {
            return &process_entanglements[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Add a thread to a process
 */
static void add_thread_to_process(Process* process, Thread* thread) {
    /* Add to head of list for simplicity */
    thread->next = process->threads;
    thread->prev = NULL;
    
    if (process->threads) {
        process->threads->prev = thread;
    }
    
    process->threads = thread;
    process->thread_count++;
    
    /* Update statistics */
    pm_stats.total_threads++;
}

/**
 * @brief Remove a thread from a process
 */
static void remove_thread_from_process(Process* process, Thread* thread) {
    if (thread->prev) {
        thread->prev->next = thread->next;
    } else {
        process->threads = thread->next;
    }
    
    if (thread->next) {
        thread->next->prev = thread->prev;
    }
    
    process->thread_count--;
    
    /* Update statistics */
    pm_stats.total_threads--;
}

/**
 * @brief Add a process to the process list
 */
static void add_process(Process* process) {
    /* Add to head of list for simplicity */
    process->next = process_list_head;
    process->prev = NULL;
    
    if (process_list_head) {
        process_list_head->prev = process;
    }
    
    process_list_head = process;
    
    /* Update statistics */
    pm_stats.total_processes++;
    
    if (process->state == PROCESS_RUNNING) {
        pm_stats.running_processes++;
    } else if (process->state == PROCESS_BLOCKED) {
        pm_stats.blocked_processes++;
    } else if (process->state == PROCESS_QUANTUM) {
        pm_stats.quantum_processes++;
    }
}

/**
 * @brief Remove a process from the process list
 */
static void remove_process(Process* process) {
    if (process->prev) {
        process->prev->next = process->next;
    } else {
        process_list_head = process->next;
    }
    
    if (process->next) {
        process->next->prev = process->prev;
    }
    
    /* Update statistics */
    pm_stats.total_processes--;
    
    if (process->state == PROCESS_RUNNING) {
        pm_stats.running_processes--;
    } else if (process->state == PROCESS_BLOCKED) {
        pm_stats.blocked_processes--;
    } else if (process->state == PROCESS_QUANTUM) {
        pm_stats.quantum_processes--;
    }
}

/**
 * @brief Update process state and statistics
 */
static void update_process_state(Process* process, ProcessState new_state) {
    /* Update statistics based on state change */
    if (process->state == PROCESS_RUNNING && new_state != PROCESS_RUNNING) {
        pm_stats.running_processes--;
    } else if (process->state != PROCESS_RUNNING && new_state == PROCESS_RUNNING) {
        pm_stats.running_processes++;
    }
    
    if (process->state == PROCESS_BLOCKED && new_state != PROCESS_BLOCKED) {
        pm_stats.blocked_processes--;
    } else if (process->state != PROCESS_BLOCKED && new_state == PROCESS_BLOCKED) {
        pm_stats.blocked_processes++;
    }
    
    if (process->state == PROCESS_QUANTUM && new_state != PROCESS_QUANTUM) {
        pm_stats.quantum_processes--;
    } else if (process->state != PROCESS_QUANTUM && new_state == PROCESS_QUANTUM) {
        pm_stats.quantum_processes++;
    }
    
    /* Update process state */
    process->state = new_state;
    
    /* If process is terminated, update its threads */
    if (new_state == PROCESS_TERMINATED) {
        Thread* thread = process->threads;
        while (thread) {
            thread->state = THREAD_TERMINATED;
            thread = thread->next;
        }
    }
}

/**
 * @brief Initialize process entanglement table
 */
static void init_process_entanglements(void) {
    memset(process_entanglements, 0, sizeof(process_entanglements));
    next_entanglement_id = 1;
    pm_stats.total_entanglements = 0;
}

/**
 * @brief Initialize the process management system
 */
bool pm_init(uint32_t max_processes) {
    /* Check if already initialized */
    if (pm_initialized) {
        return true;
    }
    
    /* Initialize process management state */
    pm_max_processes = (max_processes > 0) ? max_processes : MAX_PROCESSES;
    memset(&pm_stats, 0, sizeof(pm_stats));
    process_list_head = NULL;
    next_process_id = 1;
    next_thread_id = 1;
    
    /* Initialize process entanglements */
    init_process_entanglements();
    
    pm_initialized = true;
    printf("Process Manager initialized (max processes: %u)\n", pm_max_processes);
    
    return true;
}

/**
 * @brief Shutdown the process management system
 */
void pm_shutdown(void) {
    if (!pm_initialized) {
        return;
    }
    
    /* Terminate all processes */
    while (process_list_head) {
        pm_terminate_process(process_list_head->id, 0);
    }
    
    /* Reset state */
    pm_max_processes = 0;
    memset(&pm_stats, 0, sizeof(pm_stats));
    process_list_head = NULL;
    next_process_id = 1;
    next_thread_id = 1;
    
    /* Clear entanglements */
    init_process_entanglements();
    
    pm_initialized = false;
    printf("Process Manager shutdown complete\n");
}

/**
 * @brief Create a new process
 */
bool pm_create_process(const ProcessParams* params, ProcessId* process_id) {
    if (!pm_initialized || !params || !process_id) {
        return false;
    }
    
    /* Check if we've reached the maximum number of processes */
    if (pm_stats.total_processes >= pm_max_processes) {
        printf("Cannot create process: maximum number of processes reached\n");
        return false;
    }
    
    /* Allocate a new process structure */
    Process* process = (Process*)malloc(sizeof(Process));
    if (!process) {
        printf("Cannot create process: memory allocation failed\n");
        return false;
    }
    
    /* Initialize the process */
    memset(process, 0, sizeof(Process));
    process->id = next_process_id++;
    strncpy(process->name, params->name, sizeof(process->name) - 1);
    process->state = PROCESS_CREATED;
    process->priority = params->priority;
    process->memory_size = 0; /* Will be set when memory is allocated */
    process->threads = NULL;
    process->thread_count = 0;
    process->entanglement_id = 0;
    process->exit_code = 0;
    process->creation_time = time(NULL);
    process->execution_time = 0;
    process->resonance_level = params->resonance_level;
    
    /* Create process memory */
    uint64_t total_memory_size = params->heap_size + params->stack_size + (1024 * 1024); /* 1MB for code/data */
    
    /* Allocate virtual memory for the process */
    process->memory_map = mm_alloc_virtual(total_memory_size, MEMORY_TYPE_RAM, 
                                      MM_FLAG_READ | MM_FLAG_WRITE | 
                                      (params->quantum_capable ? MM_FLAG_QUANTUM : 0));
    
    if (!process->memory_map) {
        printf("Cannot create process: memory allocation failed\n");
        free(process);
        return false;
    }
    
    process->memory_size = total_memory_size;
    
    /* Set up segments (in a real implementation, this would be more sophisticated) */
    process->code_segment = process->memory_map;
    process->data_segment = (uint8_t*)process->memory_map + (512 * 1024); /* 512KB after code */
    process->heap = (uint8_t*)process->data_segment + (512 * 1024); /* 512KB after data */
    
    /* Create main thread */
    ThreadParams thread_params = {
        .process_id = process->id,
        .entry_point = params->entry_point,
        .arg = NULL,
        .stack_size = params->stack_size,
        .priority = params->priority,
        .quantum_capable = params->quantum_capable
    };
    
    ThreadId main_thread_id;
    if (!pm_create_thread(&thread_params, &main_thread_id)) {
        printf("Cannot create process: main thread creation failed\n");
        mm_free_virtual(process->memory_map);
        free(process);
        return false;
    }
    
    /* Add process to the list */
    add_process(process);
    
    /* Return the process ID */
    *process_id = process->id;
    
    printf("Created process %llu (%s) with main thread %llu\n", 
           (unsigned long long)process->id, process->name, (unsigned long long)main_thread_id);
    
    return true;
}

/**
 * @brief Terminate a process
 */
bool pm_terminate_process(ProcessId process_id, uint64_t exit_code) {
    if (!pm_initialized) {
        return false;
    }
    
    /* Find the process */
    Process* process = find_process(process_id);
    if (!process) {
        printf("Cannot terminate process: process %llu not found\n", (unsigned long long)process_id);
        return false;
    }
    
    /* Check if the process is already terminated */
    if (process->state == PROCESS_TERMINATED) {
        return true;
    }
    
    /* Check if the process is entangled */
    if (process->entanglement_id != 0) {
        /* Break the entanglement */
        pm_break_process_entanglement(process->entanglement_id);
    }
    
    /* Update process state and set exit code */
    update_process_state(process, PROCESS_TERMINATED);
    process->exit_code = exit_code;
    
    /* Free process memory */
    if (process->memory_map) {
        mm_free_virtual(process->memory_map);
        process->memory_map = NULL;
    }
    
    /* Free all threads */
    while (process->threads) {
        Thread* thread = process->threads;
        remove_thread_from_process(process, thread);
        
        /* Free thread stack if it was allocated separately */
        if (thread->stack_base && thread->stack_base != process->memory_map) {
            mm_free_virtual(thread->stack_base);
        }
        
        free(thread);
    }
    
    /* Remove process from the list */
    remove_process(process);
    
    /* Free the process structure */
    free(process);
    
    printf("Terminated process %llu with exit code %llu\n", 
           (unsigned long long)process_id, (unsigned long long)exit_code);
    
    return true;
}

/**
 * @brief Create a new thread in a process
 */
bool pm_create_thread(const ThreadParams* params, ThreadId* thread_id) {
    if (!pm_initialized || !params || !thread_id) {
        return false;
    }
    
    /* Find the process */
    Process* process = find_process(params->process_id);
    if (!process) {
        printf("Cannot create thread: process %llu not found\n", (unsigned long long)params->process_id);
        return false;
    }
    
    /* Check if the process is terminated */
    if (process->state == PROCESS_TERMINATED) {
        printf("Cannot create thread: process %llu is terminated\n", (unsigned long long)params->process_id);
        return false;
    }
    
    /* Check if maximum threads per process reached */
    if (process->thread_count >= MAX_THREADS_PER_PROCESS) {
        printf("Cannot create thread: maximum number of threads reached for process %llu\n", 
               (unsigned long long)params->process_id);
        return false;
    }
    
    /* Allocate a new thread structure */
    Thread* thread = (Thread*)malloc(sizeof(Thread));
    if (!thread) {
        printf("Cannot create thread: memory allocation failed\n");
        return false;
    }
    
    /* Initialize the thread */
    memset(thread, 0, sizeof(Thread));
    thread->id = next_thread_id++;
    thread->process_id = params->process_id;
    thread->state = THREAD_CREATED;
    thread->priority = params->priority;
    thread->entry_point = params->entry_point;
    thread->user_data = params->arg;
    thread->resonance_level = process->resonance_level; /* Inherit from process */
    
    /* Allocate stack for the thread */
    uint64_t stack_size = params->stack_size > 0 ? params->stack_size : (1024 * 1024); /* 1MB default */
    
    /* In a real implementation, we'd allocate from the process's memory space */
    /* For simulation, we'll create a separate allocation */
    thread->stack_base = mm_alloc_virtual(stack_size, MEMORY_TYPE_RAM, 
                                       MM_FLAG_READ | MM_FLAG_WRITE | 
                                       (params->quantum_capable ? MM_FLAG_QUANTUM : 0));
    
    if (!thread->stack_base) {
        printf("Cannot create thread: stack allocation failed\n");
        free(thread);
        return false;
    }
    
    thread->stack_size = stack_size;
    
    /* Set up thread context (in a real implementation, this would initialize CPU state) */
    thread->context.stack_pointer = (uint64_t)((uint8_t*)thread->stack_base + stack_size - 16); /* Top of stack */
    thread->context.program_counter = (uint64_t)params->entry_point;
    
    /* Add thread to the process */
    add_thread_to_process(process, thread);
    
    /* Return the thread ID */
    *thread_id = thread->id;
    
    printf("Created thread %llu in process %llu\n", 
           (unsigned long long)thread->id, (unsigned long long)process->id);
    
    return true;
}

/**
 * @brief Terminate a thread
 */
bool pm_terminate_thread(ThreadId thread_id, uint64_t exit_code) {
    if (!pm_initialized) {
        return false;
    }
    
    /* Find the thread */
    Thread* thread = find_thread(thread_id);
    if (!thread) {
        printf("Cannot terminate thread: thread %llu not found\n", (unsigned long long)thread_id);
        return false;
    }
    
    /* Find the process */
    Process* process = find_process(thread->process_id);
    if (!process) {
        printf("Cannot terminate thread: process %llu not found\n", (unsigned long long)thread->process_id);
        return false;
    }
    
    /* Check if the thread is already terminated */
    if (thread->state == THREAD_TERMINATED) {
        return true;
    }
    
    /* Update thread state */
    thread->state = THREAD_TERMINATED;
    
    /* If this is the last active thread, terminate the process */
    bool all_threads_terminated = true;
    Thread* t = process->threads;
    while (t) {
        if (t->id != thread_id && t->state != THREAD_TERMINATED) {
            all_threads_terminated = false;
            break;
        }
        t = t->next;
    }
    
    if (all_threads_terminated) {
        pm_terminate_process(process->id, exit_code);
    } else {
        /* Just remove this thread */
        remove_thread_from_process(process, thread);
        
        /* Free thread stack if it was allocated separately */
        if (thread->stack_base && thread->stack_base != process->memory_map) {
            mm_free_virtual(thread->stack_base);
        }
        
        free(thread);
        
        printf("Terminated thread %llu in process %llu\n", 
               (unsigned long long)thread_id, (unsigned long long)process->id);
    }
    
    return true;
}

/**
 * @brief Get information about a process
 */
Process* pm_get_process(ProcessId process_id) {
    if (!pm_initialized) {
        return NULL;
    }
    
    return find_process(process_id);
}

/**
 * @brief Get information about a thread
 */
Thread* pm_get_thread(ThreadId thread_id) {
    if (!pm_initialized) {
        return NULL;
    }
    
    return find_thread(thread_id);
}

/**
 * @brief Set process priority
 */
bool pm_set_process_priority(ProcessId process_id, PriorityLevel priority) {
    if (!pm_initialized) {
        return false;
    }
    
    /* Find the process */
    Process* process = find_process(process_id);
    if (!process) {
        return false;
    }
    
    /* Update priority */
    process->priority = priority;
    
    /* Update all threads in the process to match */
    Thread* thread = process->threads;
    while (thread) {
        thread->priority = priority;
        thread = thread->next;
    }
    
    return true;
}

/**
 * @brief Set thread priority
 */
bool pm_set_thread_priority(ThreadId thread_id, PriorityLevel priority) {
    if (!pm_initialized) {
        return false;
    }
    
    /* Find the thread */
    Thread* thread = find_thread(thread_id);
    if (!thread) {
        return false;
    }
    
    /* Update priority */
    thread->priority = priority;
    
    return true;
}

/**
 * @brief Create quantum entanglement between two processes
 */
uint64_t pm_create_process_entanglement(ProcessId first_process_id, ProcessId second_process_id,
                                        ProcessEntanglementType type, NodeLevel resonance_level) {
    if (!pm_initialized) {
        return 0;
    }
    
    /* Check if quantum operations are supported */
    const HalOperations* hal_ops = hal_get_operations();
    if (!hal_ops || !hal_ops->has_quantum_support || !hal_ops->has_quantum_support()) {
        printf("Cannot create process entanglement: quantum operations not supported\n");
        return 0;
    }
    
    /* Find the processes */
    Process* first_process = find_process(first_process_id);
    Process* second_process = find_process(second_process_id);
    
    if (!first_process || !second_process) {
        printf("Cannot create process entanglement: process not found\n");
        return 0;
    }
    
    /* Check if processes are already entangled */
    if (first_process->entanglement_id != 0 || second_process->entanglement_id != 0) {
        printf("Cannot create process entanglement: process already entangled\n");
        return 0;
    }
    
    /* Find a free entanglement slot */
    ProcessEntanglement* slot = find_free_entanglement_slot();
    if (!slot) {
        printf("Cannot create process entanglement: maximum entanglements reached\n");
        return 0;
    }
    
    /* Set up the entanglement */
    uint64_t entanglement_id = next_entanglement_id++;
    slot->id = entanglement_id;
    slot->first_process = first_process_id;
    slot->second_process = second_process_id;
    slot->type = type;
    slot->resonance_level = resonance_level;
    slot->stability = 0.95; /* Initial stability */
    slot->is_synchronized = true;
    
    /* Update the processes */
    first_process->entanglement_id = entanglement_id;
    second_process->entanglement_id = entanglement_id;
    
    /* If this is a state or execution entanglement, update both processes to QUANTUM state */
    if (type == ENTANGLE_STATE || type == ENTANGLE_EXECUTION) {
        update_process_state(first_process, PROCESS_QUANTUM);
        update_process_state(second_process, PROCESS_QUANTUM);
    }
    
    /* If this is a memory entanglement, entangle process memory */
    if (type == ENTANGLE_MEMORY) {
        /* Entangle the memory if both processes have valid memory maps */
        if (first_process->memory_map && second_process->memory_map) {
            /* Create memory entanglement */
            mm_create_entanglement(first_process->memory_map, second_process->memory_map, resonance_level);
        }
    }
    
    /* Update statistics */
    pm_stats.total_entanglements++;
    pm_stats.total_quantum_ops++;
    
    printf("Created process entanglement (ID: %llu, Type: %d) between processes %llu and %llu\n",
           (unsigned long long)entanglement_id, type, 
           (unsigned long long)first_process_id, (unsigned long long)second_process_id);
    
    return entanglement_id;
}

/**
 * @brief Break process entanglement
 */
bool pm_break_process_entanglement(uint64_t entanglement_id) {
    if (!pm_initialized) {
        return false;
    }
    
    /* Find the entanglement */
    ProcessEntanglement* entanglement = find_entanglement(entanglement_id);
    if (!entanglement) {
        printf("Cannot break process entanglement: entanglement %llu not found\n", 
               (unsigned long long)entanglement_id);
        return false;
    }
    
    /* Find the processes */
    Process* first_process = find_process(entanglement->first_process);
    Process* second_process = find_process(entanglement->second_process);
    
    /* If this was a memory entanglement, break memory entanglement */
    if (entanglement->type == ENTANGLE_MEMORY) {
        /* Find memory regions to disentangle */
        if (first_process && first_process->memory_map && 
            second_process && second_process->memory_map) {
            /* Disentangle the memory */
            MemoryRegion* region1 = mm_get_region_info(first_process->memory_map);
            MemoryRegion* region2 = mm_get_region_info(second_process->memory_map);
            
            if (region1 && region2 && region1->entanglement_id == region2->entanglement_id) {
                mm_break_entanglement(region1->entanglement_id);
            }
        }
    }
    
    /* Update processes if they still exist */
    if (first_process) {
        first_process->entanglement_id = 0;
        if (entanglement->type == ENTANGLE_STATE || entanglement->type == ENTANGLE_EXECUTION) {
            update_process_state(first_process, PROCESS_READY);
        }
    }
    
    if (second_process) {
        second_process->entanglement_id = 0;
        if (entanglement->type == ENTANGLE_STATE || entanglement->type == ENTANGLE_EXECUTION) {
            update_process_state(second_process, PROCESS_READY);
        }
    }
    
    /* Clear the entanglement slot */
    memset(entanglement, 0, sizeof(ProcessEntanglement));
    
    /* Update statistics */
    pm_stats.total_entanglements--;
    pm_stats.total_quantum_ops++;
    
    printf("Broke process entanglement (ID: %llu)\n", (unsigned long long)entanglement_id);
    
    return true;
}

/**
 * @brief Synchronize entangled processes
 */
bool pm_sync_process_entanglement(uint64_t entanglement_id) {
    if (!pm_initialized) {
        return false;
    }
    
    /* Find the entanglement */
    ProcessEntanglement* entanglement = find_entanglement(entanglement_id);
    if (!entanglement) {
        printf("Cannot synchronize process entanglement: entanglement %llu not found\n", 
               (unsigned long long)entanglement_id);
        return false;
    }
    
    /* Find the processes */
    Process* first_process = find_process(entanglement->first_process);
    Process* second_process = find_process(entanglement->second_process);
    
    if (!first_process || !second_process) {
        printf("Cannot synchronize process entanglement: process not found\n");
        return false;
    }
    
    /* Synchronize based on entanglement type */
    switch (entanglement->type) {
        case ENTANGLE_MEMORY:
            /* If this is a memory entanglement, synchronize memory */
            if (first_process->memory_map && second_process->memory_map) {
                /* Find memory regions */
                MemoryRegion* region1 = mm_get_region_info(first_process->memory_map);
                MemoryRegion* region2 = mm_get_region_info(second_process->memory_map);
                
                if (region1 && region2 && region1->entanglement_id == region2->entanglement_id) {
                    mm_sync_entanglement(region1->entanglement_id);
                }
            }
            break;
            
        case ENTANGLE_STATE:
            /* If this is a state entanglement, synchronize process states */
            if (first_process->state != second_process->state) {
                /* Choose the higher state (RUNNING > READY > BLOCKED) */
                if (first_process->state == PROCESS_RUNNING || second_process->state == PROCESS_RUNNING) {
                    update_process_state(first_process, PROCESS_RUNNING);
                    update_process_state(second_process, PROCESS_RUNNING);
                } else if (first_process->state == PROCESS_READY || second_process->state == PROCESS_READY) {
                    update_process_state(first_process, PROCESS_READY);
                    update_process_state(second_process, PROCESS_READY);
                } else {
                    update_process_state(first_process, PROCESS_BLOCKED);
                    update_process_state(second_process, PROCESS_BLOCKED);
                }
            }
            break;
            
        case ENTANGLE_EXECUTION:
            /* If this is an execution entanglement, synchronize execution state */
            /* This would involve more complex thread context synchronization */
            /* For simulation, we'll just synchronize thread states */
            {
                Thread *thread1 = first_process->threads;
                Thread *thread2 = second_process->threads;
                
                while (thread1 && thread2) {
                    if (thread1->state != thread2->state) {
                        /* Synchronize to the more active state */
                        if (thread1->state == THREAD_RUNNING || thread2->state == THREAD_RUNNING) {
                            thread1->state = THREAD_RUNNING;
                            thread2->state = THREAD_RUNNING;
                        } else if (thread1->state == THREAD_READY || thread2->state == THREAD_READY) {
                            thread1->state = THREAD_READY;
                            thread2->state = THREAD_READY;
                        } else {
                            thread1->state = THREAD_BLOCKED;
                            thread2->state = THREAD_BLOCKED;
                        }
                    }
                    
                    thread1 = thread1->next;
                    thread2 = thread2->next;
                }
            }
            break;
            
        case ENTANGLE_RESONANCE:
            /* If this is a resonance entanglement, synchronize resonance levels */
            {
                /* Choose the higher resonance level */
                NodeLevel target_level = (first_process->resonance_level > second_process->resonance_level) ?
                                         first_process->resonance_level : second_process->resonance_level;
                
                first_process->resonance_level = target_level;
                second_process->resonance_level = target_level;
                
                /* Update thread resonance levels as well */
                Thread *thread1 = first_process->threads;
                while (thread1) {
                    thread1->resonance_level = target_level;
                    thread1 = thread1->next;
                }
                
                Thread *thread2 = second_process->threads;
                while (thread2) {
                    thread2->resonance_level = target_level;
                    thread2 = thread2->next;
                }
            }
            break;
            
        default:
            /* Unknown entanglement type */
            printf("Cannot synchronize process entanglement: unknown entanglement type\n");
            return false;
    }
    
    /* Mark as synchronized */
    entanglement->is_synchronized = true;
    
    /* Reduce stability slightly due to synchronization stress */
    entanglement->stability *= 0.99;
    if (entanglement->stability < 0.5) {
        printf("Warning: Process entanglement stability is low (%.2f)\n", entanglement->stability);
    }
    
    /* Update statistics */
    pm_stats.total_quantum_ops++;
    
    printf("Synchronized process entanglement (ID: %llu)\n", (unsigned long long)entanglement_id);
    
    return true;
}

/**
 * @brief Get a list of all processes
 */
uint32_t pm_get_all_processes(Process** processes, uint32_t max_count) {
    if (!pm_initialized || !processes || max_count == 0) {
        return 0;
    }
    
    Process* process = process_list_head;
    uint32_t count = 0;
    
    while (process && count < max_count) {
        processes[count++] = process;
        process = process->next;
    }
    
    return count;
}

/**
 * @brief Get a list of all threads for a process
 */
uint32_t pm_get_process_threads(ProcessId process_id, Thread** threads, uint32_t max_count) {
    if (!pm_initialized || !threads || max_count == 0) {
        return 0;
    }
    
    /* Find the process */
    Process* process = find_process(process_id);
    if (!process) {
        return 0;
    }
    
    Thread* thread = process->threads;
    uint32_t count = 0;
    
    while (thread && count < max_count) {
        threads[count++] = thread;
        thread = thread->next;
    }
    
    return count;
}

/**
 * @brief Get process statistics
 */
void pm_get_stats(ProcessStats* stats) {
    if (!pm_initialized || !stats) {
        return;
    }
    
    /* Copy the statistics */
    memcpy(stats, &pm_stats, sizeof(ProcessStats));
}

/**
 * @brief Print process statistics
 */
void pm_print_stats(void) {
    if (!pm_initialized) {
        return;
    }
    
    printf("\nProcess Manager Statistics:\n");
    printf("Total Processes: %u\n", pm_stats.total_processes);
    printf("Running Processes: %u\n", pm_stats.running_processes);
    printf("Blocked Processes: %u\n", pm_stats.blocked_processes);
    printf("Quantum Processes: %u\n", pm_stats.quantum_processes);
    printf("Total Threads: %u\n", pm_stats.total_threads);
    printf("Total Process Entanglements: %u\n", pm_stats.total_entanglements);
    printf("Total Context Switches: %llu\n", (unsigned long long)pm_stats.total_context_switches);
    printf("Total Quantum Operations: %llu\n", (unsigned long long)pm_stats.total_quantum_ops);
}