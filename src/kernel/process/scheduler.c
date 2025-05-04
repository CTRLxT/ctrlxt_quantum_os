/**
 * @file scheduler.c
 * @brief Process Scheduler implementation
 */

#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../hal/hal.h"

/* Scheduler state */
static bool scheduler_initialized = false;
static bool scheduler_running = false;
static SchedulerState scheduler_state = {0};

/* Ready queues for different priority levels */
#define PRIORITY_QUEUE_COUNT (PRIORITY_QUANTUM + 1)
static SchedulerQueueEntry* ready_queues[PRIORITY_QUEUE_COUNT] = {NULL};

/* Quantum superposition state tracking */
#define MAX_SUPERPOSITIONS 32
typedef struct {
    ThreadId thread_id;
    double probability;
    NodeLevel resonance_level;
} SuperpositionState;
static SuperpositionState superposition_states[MAX_SUPERPOSITIONS] = {0};

/**
 * @brief Get current timestamp in nanoseconds
 */
static uint64_t get_timestamp_ns(void) {
    /* In a real implementation, this would use a high-precision timer */
    /* For simulation, we'll just use time() and convert to nanoseconds */
    return (uint64_t)time(NULL) * 1000000000ULL;
}

/**
 * @brief Initialize the scheduler
 */
bool scheduler_init(SchedulerType type, uint64_t time_slice, bool preemption_enabled) {
    /* Check if already initialized */
    if (scheduler_initialized) {
        return true;
    }
    
    /* Initialize scheduler state */
    scheduler_state.type = type;
    scheduler_state.current_process = 0;
    scheduler_state.current_thread = 0;
    scheduler_state.time_slice = (time_slice > 0) ? time_slice : SCHEDULER_DEFAULT_QUANTUM;
    scheduler_state.last_context_switch = 0;
    scheduler_state.total_context_switches = 0;
    scheduler_state.superposition_count = 0;
    scheduler_state.preemption_enabled = preemption_enabled;
    scheduler_state.resonance_level = NODE_ZERO_POINT;
    
    /* Initialize ready queues */
    for (int i = 0; i < PRIORITY_QUEUE_COUNT; i++) {
        ready_queues[i] = NULL;
    }
    
    /* Initialize superposition states */
    memset(superposition_states, 0, sizeof(superposition_states));
    
    scheduler_initialized = true;
    printf("Scheduler initialized (type: %d, time slice: %llu ns)\n", 
           type, (unsigned long long)scheduler_state.time_slice);
    
    return true;
}

/**
 * @brief Shutdown the scheduler
 */
void scheduler_shutdown(void) {
    if (!scheduler_initialized) {
        return;
    }
    
    /* Stop scheduler if running */
    if (scheduler_running) {
        scheduler_stop();
    }
    
    /* Free all queue entries */
    for (int i = 0; i < PRIORITY_QUEUE_COUNT; i++) {
        SchedulerQueueEntry* entry = ready_queues[i];
        while (entry) {
            SchedulerQueueEntry* next = entry->next;
            free(entry);
            entry = next;
        }
        ready_queues[i] = NULL;
    }
    
    /* Clear superposition states */
    memset(superposition_states, 0, sizeof(superposition_states));
    
    scheduler_initialized = false;
    printf("Scheduler shutdown complete\n");
}

/**
 * @brief Add a thread to a priority queue
 */
static bool add_to_queue(ThreadId thread_id, PriorityLevel priority) {
    /* Validate priority */
    if (priority >= PRIORITY_QUEUE_COUNT) {
        priority = PRIORITY_NORMAL;
    }
    
    /* Get thread info */
    Thread* thread = pm_get_thread(thread_id);
    if (!thread) {
        return false;
    }
    
    /* Create a new queue entry */
    SchedulerQueueEntry* entry = (SchedulerQueueEntry*)malloc(sizeof(SchedulerQueueEntry));
    if (!entry) {
        return false;
    }
    
    /* Initialize the entry */
    entry->process_id = thread->process_id;
    entry->thread_id = thread_id;
    entry->priority = priority;
    entry->quantum_probability = 1.0; /* Default to 100% execution probability */
    entry->next = NULL;
    
    /* Check for superposition */
    for (int i = 0; i < MAX_SUPERPOSITIONS; i++) {
        if (superposition_states[i].thread_id == thread_id) {
            entry->quantum_probability = superposition_states[i].probability;
            break;
        }
    }
    
    /* Add to the appropriate queue (at the end) */
    if (ready_queues[priority] == NULL) {
        ready_queues[priority] = entry;
    } else {
        SchedulerQueueEntry* current = ready_queues[priority];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = entry;
    }
    
    return true;
}

/**
 * @brief Remove a thread from priority queues
 */
static bool remove_from_queues(ThreadId thread_id) {
    bool found = false;
    
    /* Check all queues */
    for (int i = 0; i < PRIORITY_QUEUE_COUNT; i++) {
        SchedulerQueueEntry* prev = NULL;
        SchedulerQueueEntry* current = ready_queues[i];
        
        while (current) {
            if (current->thread_id == thread_id) {
                /* Remove this entry */
                if (prev) {
                    prev->next = current->next;
                } else {
                    ready_queues[i] = current->next;
                }
                
                free(current);
                found = true;
                break; /* Thread should only be in one queue */
            }
            
            prev = current;
            current = current->next;
        }
        
        if (found) {
            break;
        }
    }
    
    return found;
}

/**
 * @brief Get the next thread to run from ready queues
 */
static ThreadId get_next_thread(void) {
    /* Implementation depends on scheduler type */
    switch (scheduler_state.type) {
        case SCHEDULER_ROUND_ROBIN:
        case SCHEDULER_PRIORITY:
        case SCHEDULER_MULTILEVEL_FEEDBACK:
            /* Start with highest priority queue */
            for (int i = PRIORITY_QUEUE_COUNT - 1; i >= 0; i--) {
                if (ready_queues[i]) {
                    /* Simple round-robin within priority level - take first thread and move to back */
                    SchedulerQueueEntry* entry = ready_queues[i];
                    ThreadId next_thread = entry->thread_id;
                    
                    /* Remove from front of queue */
                    ready_queues[i] = entry->next;
                    
                    /* For multilevel feedback, adjust priority if using full time slice */
                    if (scheduler_state.type == SCHEDULER_MULTILEVEL_FEEDBACK) {
                        /* In a real implementation, we'd track time slice usage and adjust priority */
                        /* For now, we'll just re-add at the same priority level */
                    }
                    
                    /* Re-add to back of queue for round-robin */
                    free(entry); /* Free old entry */
                    add_to_queue(next_thread, i); /* Re-add to back of queue */
                    
                    return next_thread;
                }
            }
            break;
            
        case SCHEDULER_REALTIME:
            /* Real-time scheduler would have more sophisticated scheduling */
            /* For now, just use priority scheduling */
            for (int i = PRIORITY_QUEUE_COUNT - 1; i >= 0; i--) {
                if (ready_queues[i]) {
                    return ready_queues[i]->thread_id;
                }
            }
            break;
            
        case SCHEDULER_QUANTUM:
            /* Quantum scheduler considers superposition states */
            /* Randomly select based on quantum probability */
            
            /* First, get all candidate threads with their probabilities */
            ThreadId candidates[MAX_SUPERPOSITIONS * 2]; /* Allow for both superposition and normal threads */
            double probabilities[MAX_SUPERPOSITIONS * 2];
            int candidate_count = 0;
            double total_probability = 0.0;
            
            /* Start with superposition threads */
            for (int i = 0; i < MAX_SUPERPOSITIONS; i++) {
                if (superposition_states[i].thread_id != 0) {
                    candidates[candidate_count] = superposition_states[i].thread_id;
                    probabilities[candidate_count] = superposition_states[i].probability;
                    total_probability += probabilities[candidate_count];
                    candidate_count++;
                }
            }
            
            /* Add normal threads from queues (prioritizing higher priority) */
            for (int i = PRIORITY_QUEUE_COUNT - 1; i >= 0 && candidate_count < MAX_SUPERPOSITIONS * 2; i--) {
                SchedulerQueueEntry* entry = ready_queues[i];
                while (entry && candidate_count < MAX_SUPERPOSITIONS * 2) {
                    /* Check if already in candidates (from superposition) */
                    bool already_added = false;
                    for (int j = 0; j < candidate_count; j++) {
                        if (candidates[j] == entry->thread_id) {
                            already_added = true;
                            break;
                        }
                    }
                    
                    if (!already_added) {
                        candidates[candidate_count] = entry->thread_id;
                        probabilities[candidate_count] = entry->quantum_probability * (i + 1) / PRIORITY_QUEUE_COUNT;
                        total_probability += probabilities[candidate_count];
                        candidate_count++;
                    }
                    
                    entry = entry->next;
                }
            }
            
            /* If no candidates, return 0 */
            if (candidate_count == 0 || total_probability <= 0.0) {
                return 0;
            }
            
            /* Normalize probabilities */
            for (int i = 0; i < candidate_count; i++) {
                probabilities[i] /= total_probability;
            }
            
            /* Select a thread based on probabilities */
            double random_value = (double)rand() / RAND_MAX;
            double cumulative_probability = 0.0;
            
            for (int i = 0; i < candidate_count; i++) {
                cumulative_probability += probabilities[i];
                if (random_value <= cumulative_probability) {
                    return candidates[i];
                }
            }
            
            /* Fallback to first candidate if something went wrong */
            return candidates[0];
            
            break;
    }
    
    return 0; /* No thread found */
}

/**
 * @brief Start the scheduler
 */
bool scheduler_start(void) {
    if (!scheduler_initialized || scheduler_running) {
        return false;
    }
    
    /* Get initial timestamp */
    scheduler_state.last_context_switch = get_timestamp_ns();
    
    /* Initialize random seed for quantum scheduling */
    srand((unsigned int)time(NULL));
    
    scheduler_running = true;
    printf("Scheduler started\n");
    
    /* Perform initial context switch */
    return scheduler_context_switch(true);
}

/**
 * @brief Stop the scheduler
 */
bool scheduler_stop(void) {
    if (!scheduler_initialized || !scheduler_running) {
        return false;
    }
    
    scheduler_running = false;
    scheduler_state.current_process = 0;
    scheduler_state.current_thread = 0;
    
    printf("Scheduler stopped\n");
    return true;
}

/**
 * @brief Add a thread to the ready queue
 */
bool scheduler_add_thread(ThreadId thread_id) {
    if (!scheduler_initialized) {
        return false;
    }
    
    /* Get thread info */
    Thread* thread = pm_get_thread(thread_id);
    if (!thread) {
        return false;
    }
    
    /* Remove from queues if already present */
    remove_from_queues(thread_id);
    
    /* Update thread state */
    if (thread->state != THREAD_RUNNING) {
        thread->state = THREAD_READY;
    }
    
    /* Add to appropriate queue */
    return add_to_queue(thread_id, thread->priority);
}

/**
 * @brief Remove a thread from the ready queue
 */
bool scheduler_remove_thread(ThreadId thread_id) {
    if (!scheduler_initialized) {
        return false;
    }
    
    return remove_from_queues(thread_id);
}

/**
 * @brief Block a thread
 */
bool scheduler_block_thread(ThreadId thread_id) {
    if (!scheduler_initialized) {
        return false;
    }
    
    /* Get thread info */
    Thread* thread = pm_get_thread(thread_id);
    if (!thread) {
        return false;
    }
    
    /* Remove from ready queues */
    remove_from_queues(thread_id);
    
    /* Update thread state */
    thread->state = THREAD_BLOCKED;
    
    /* If this is the current thread, force a context switch */
    if (scheduler_running && thread_id == scheduler_state.current_thread) {
        return scheduler_context_switch(true);
    }
    
    return true;
}

/**
 * @brief Unblock a thread
 */
bool scheduler_unblock_thread(ThreadId thread_id) {
    if (!scheduler_initialized) {
        return false;
    }
    
    /* Get thread info */
    Thread* thread = pm_get_thread(thread_id);
    if (!thread) {
        return false;
    }
    
    /* Update thread state */
    thread->state = THREAD_READY;
    
    /* Add to ready queue */
    bool result = add_to_queue(thread_id, thread->priority);
    
    /* Consider context switch if higher priority than current thread */
    if (result && scheduler_running && scheduler_state.preemption_enabled) {
        Thread* current_thread = pm_get_thread(scheduler_state.current_thread);
        if (current_thread && thread->priority > current_thread->priority) {
            scheduler_context_switch(true);
        }
    }
    
    return result;
}

/**
 * @brief Change thread priority
 */
bool scheduler_set_thread_priority(ThreadId thread_id, PriorityLevel priority) {
    if (!scheduler_initialized) {
        return false;
    }
    
    /* Validate priority */
    if (priority >= PRIORITY_QUEUE_COUNT) {
        return false;
    }
    
    /* Change priority in process manager */
    bool result = pm_set_thread_priority(thread_id, priority);
    if (!result) {
        return false;
    }
    
    /* If thread is in a ready queue, update its position */
    if (remove_from_queues(thread_id)) {
        add_to_queue(thread_id, priority);
    }
    
    /* Consider context switch if necessary */
    if (scheduler_running && scheduler_state.preemption_enabled) {
        if (scheduler_state.current_thread == thread_id) {
            /* Current thread's priority changed - might need to yield to higher priority */
            Thread* current_thread = pm_get_thread(thread_id);
            if (current_thread && priority < current_thread->priority) {
                scheduler_context_switch(true);
            }
        } else {
            /* Other thread's priority changed - might need to preempt current */
            Thread* current_thread = pm_get_thread(scheduler_state.current_thread);
            if (current_thread && priority > current_thread->priority) {
                scheduler_context_switch(true);
            }
        }
    }
    
    return true;
}

/**
 * @brief Create a quantum superposition for a thread
 */
bool scheduler_create_superposition(ThreadId thread_id, NodeLevel resonance_level) {
    if (!scheduler_initialized) {
        return false;
    }
    
    /* Check if quantum operations are supported */
    const HalOperations* hal_ops = hal_get_operations();
    if (!hal_ops || !hal_ops->has_quantum_support || !hal_ops->has_quantum_support()) {
        printf("Cannot create thread superposition: quantum operations not supported\n");
        return false;
    }
    
    /* Get thread info */
    Thread* thread = pm_get_thread(thread_id);
    if (!thread) {
        return false;
    }
    
    /* Find a free superposition slot */
    int slot_index = -1;
    for (int i = 0; i < MAX_SUPERPOSITIONS; i++) {
        if (superposition_states[i].thread_id == 0) {
            slot_index = i;
            break;
        } else if (superposition_states[i].thread_id == thread_id) {
            /* Thread already in superposition, just update it */
            slot_index = i;
            break;
        }
    }
    
    if (slot_index == -1) {
        printf("Cannot create thread superposition: maximum superpositions reached\n");
        return false;
    }
    
    /* Set up the superposition */
    superposition_states[slot_index].thread_id = thread_id;
    superposition_states[slot_index].probability = 0.5; /* 50% initial probability */
    superposition_states[slot_index].resonance_level = resonance_level;
    
    /* Update thread state to quantum */
    thread->state = THREAD_QUANTUM;
    
    /* Update scheduler stats */
    scheduler_state.superposition_count++;
    
    /* If thread is in ready queue, update its probability */
    for (int i = 0; i < PRIORITY_QUEUE_COUNT; i++) {
        SchedulerQueueEntry* entry = ready_queues[i];
        while (entry) {
            if (entry->thread_id == thread_id) {
                entry->quantum_probability = 0.5;
                break;
            }
            entry = entry->next;
        }
    }
    
    printf("Created quantum superposition for thread %llu with resonance level %d\n",
           (unsigned long long)thread_id, resonance_level);
    
    return true;
}

/**
 * @brief Collapse a thread's quantum superposition
 */
bool scheduler_collapse_superposition(ThreadId thread_id, double probability_bias) {
    if (!scheduler_initialized) {
        return false;
    }
    
    /* Find the superposition */
    int slot_index = -1;
    for (int i = 0; i < MAX_SUPERPOSITIONS; i++) {
        if (superposition_states[i].thread_id == thread_id) {
            slot_index = i;
            break;
        }
    }
    
    if (slot_index == -1) {
        return false; /* Not in superposition */
    }
    
    /* Get thread info */
    Thread* thread = pm_get_thread(thread_id);
    if (!thread) {
        /* Thread doesn't exist anymore, just clear the superposition */
        superposition_states[slot_index].thread_id = 0;
        scheduler_state.superposition_count--;
        return true;
    }
    
    /* Collapse the superposition based on probability and bias */
    double collapse_probability = superposition_states[slot_index].probability;
    
    /* Apply bias */
    if (probability_bias >= 0.0 && probability_bias <= 1.0) {
        collapse_probability = collapse_probability * probability_bias + 
                             (1.0 - collapse_probability) * (1.0 - probability_bias);
    }
    
    /* Generate random value for collapse */
    double random_value = (double)rand() / RAND_MAX;
    
    /* Determine collapse outcome */
    bool thread_exists = (random_value <= collapse_probability);
    
    if (thread_exists) {
        /* Thread survived collapse, restore to READY state */
        thread->state = THREAD_READY;
        
        /* Add to ready queue if not already there */
        bool in_queue = false;
        for (int i = 0; i < PRIORITY_QUEUE_COUNT; i++) {
            SchedulerQueueEntry* entry = ready_queues[i];
            while (entry) {
                if (entry->thread_id == thread_id) {
                    in_queue = true;
                    entry->quantum_probability = 1.0; /* Restore normal probability */
                    break;
                }
                entry = entry->next;
            }
            if (in_queue) break;
        }
        
        if (!in_queue) {
            add_to_queue(thread_id, thread->priority);
        }
        
        printf("Thread %llu survived quantum collapse\n", (unsigned long long)thread_id);
    } else {
        /* Thread did not survive collapse, terminate it */
        printf("Thread %llu did not survive quantum collapse, terminating\n", (unsigned long long)thread_id);
        pm_terminate_thread(thread_id, 0);
    }
    
    /* Clear superposition state */
    superposition_states[slot_index].thread_id = 0;
    scheduler_state.superposition_count--;
    
    return true;
}

/**
 * @brief Perform context switch to next thread
 */
bool scheduler_context_switch(bool force) {
    if (!scheduler_initialized || !scheduler_running) {
        return false;
    }
    
    /* Check if time slice has expired */
    uint64_t current_time = get_timestamp_ns();
    uint64_t elapsed = current_time - scheduler_state.last_context_switch;
    
    if (!force && elapsed < scheduler_state.time_slice) {
        return false; /* Time slice not expired yet */
    }
    
    /* Save current thread context if there is one */
    if (scheduler_state.current_thread != 0) {
        Thread* current = pm_get_thread(scheduler_state.current_thread);
        if (current) {
            /* In a real implementation, this would save CPU context */
            /* For simulation, just track execution time */
            current->execution_time += elapsed;
            
            /* Add back to ready queue if still runnable */
            if (current->state == THREAD_RUNNING) {
                current->state = THREAD_READY;
                add_to_queue(scheduler_state.current_thread, current->priority);
            }
        }
    }
    
    /* Select next thread to run */
    ThreadId next_thread = get_next_thread();
    
    /* If no thread is ready, idle */
    if (next_thread == 0) {
        scheduler_state.current_process = 0;
        scheduler_state.current_thread = 0;
        scheduler_state.last_context_switch = current_time;
        return true;
    }
    
    /* Get thread and process info */
    Thread* next = pm_get_thread(next_thread);
    if (!next) {
        /* Thread disappeared, try again */
        return scheduler_context_switch(true);
    }
    
    Process* process = pm_get_process(next->process_id);
    if (!process) {
        /* Process disappeared, try again */
        return scheduler_context_switch(true);
    }
    
    /* Update thread and process state */
    next->state = THREAD_RUNNING;
    if (process->state != PROCESS_RUNNING) {
        process->state = PROCESS_RUNNING;
    }
    
    /* Update scheduler state */
    scheduler_state.current_process = next->process_id;
    scheduler_state.current_thread = next_thread;
    scheduler_state.last_context_switch = current_time;
    scheduler_state.total_context_switches++;
    
    /* Load context (in a real implementation) */
    /* For simulation, just report the switch */
    printf("Context switch to thread %llu in process %llu\n", 
           (unsigned long long)next_thread, (unsigned long long)next->process_id);
    
    return true;
}

/**
 * @brief Get the currently executing thread
 */
ThreadId scheduler_get_current_thread(void) {
    if (!scheduler_initialized || !scheduler_running) {
        return 0;
    }
    
    return scheduler_state.current_thread;
}

/**
 * @brief Get the scheduler state
 */
void scheduler_get_state(SchedulerState* state) {
    if (!scheduler_initialized || !state) {
        return;
    }
    
    memcpy(state, &scheduler_state, sizeof(SchedulerState));
}

/**
 * @brief Change the scheduler type
 */
bool scheduler_change_type(SchedulerType type) {
    if (!scheduler_initialized) {
        return false;
    }
    
    /* Cannot change type while running */
    if (scheduler_running) {
        return false;
    }
    
    scheduler_state.type = type;
    printf("Scheduler type changed to %d\n", type);
    
    return true;
}

/**
 * @brief Set the scheduler's resonance level
 */
bool scheduler_set_resonance_level(NodeLevel level) {
    if (!scheduler_initialized) {
        return false;
    }
    
    scheduler_state.resonance_level = level;
    printf("Scheduler resonance level set to %d\n", level);
    
    return true;
}