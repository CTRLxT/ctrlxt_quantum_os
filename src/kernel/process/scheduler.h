/**
 * @file scheduler.h
 * @brief Process Scheduler for CTRLxT OS
 *
 * This file defines the interface for the process scheduler,
 * which manages the execution of processes and threads,
 * including quantum superposition scheduling.
 */

#ifndef CTRLXT_SCHEDULER_H
#define CTRLXT_SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include "process_manager.h"
#include "../../quantum/resonance/resonant_frequencies.h"

/**
 * @brief Scheduler quantum time slice in nanoseconds
 */
#define SCHEDULER_DEFAULT_QUANTUM     10000000  /* 10 ms */

/**
 * @brief Scheduler types
 */
typedef enum {
    SCHEDULER_ROUND_ROBIN,            /**< Simple round-robin scheduler */
    SCHEDULER_PRIORITY,                /**< Priority-based scheduler */
    SCHEDULER_MULTILEVEL_FEEDBACK,     /**< Multilevel feedback queue scheduler */
    SCHEDULER_REALTIME,                /**< Real-time scheduler */
    SCHEDULER_QUANTUM                  /**< Quantum superposition scheduler */
} SchedulerType;

/**
 * @brief Scheduler state structure
 */
typedef struct {
    SchedulerType type;                /**< Scheduler type */
    ProcessId current_process;         /**< Currently executing process */
    ThreadId current_thread;           /**< Currently executing thread */
    uint64_t time_slice;               /**< Time slice in nanoseconds */
    uint64_t last_context_switch;      /**< Timestamp of last context switch */
    uint64_t total_context_switches;   /**< Total number of context switches */
    uint32_t superposition_count;      /**< Number of superposition states */
    bool preemption_enabled;           /**< Whether preemption is enabled */
    NodeLevel resonance_level;         /**< Scheduler resonance level */
} SchedulerState;

/**
 * @brief Scheduler ready queue entry
 */
typedef struct SchedulerQueueEntry {
    ProcessId process_id;              /**< Process ID */
    ThreadId thread_id;                /**< Thread ID */
    PriorityLevel priority;            /**< Thread priority */
    double quantum_probability;        /**< Quantum execution probability */
    struct SchedulerQueueEntry* next;  /**< Next entry in queue */
} SchedulerQueueEntry;

/**
 * @brief Initialize the scheduler
 * 
 * @param type Scheduler type
 * @param time_slice Time slice in nanoseconds (0 for default)
 * @param preemption_enabled Whether preemption is enabled
 * @return true if initialization succeeded, false otherwise
 */
bool scheduler_init(SchedulerType type, uint64_t time_slice, bool preemption_enabled);

/**
 * @brief Shutdown the scheduler
 */
void scheduler_shutdown(void);

/**
 * @brief Start the scheduler
 * 
 * @return true if scheduler started successfully, false otherwise
 */
bool scheduler_start(void);

/**
 * @brief Stop the scheduler
 * 
 * @return true if scheduler stopped successfully, false otherwise
 */
bool scheduler_stop(void);

/**
 * @brief Add a thread to the ready queue
 * 
 * @param thread_id Thread ID to add
 * @return true if addition succeeded, false otherwise
 */
bool scheduler_add_thread(ThreadId thread_id);

/**
 * @brief Remove a thread from the ready queue
 * 
 * @param thread_id Thread ID to remove
 * @return true if removal succeeded, false otherwise
 */
bool scheduler_remove_thread(ThreadId thread_id);

/**
 * @brief Block a thread
 * 
 * @param thread_id Thread ID to block
 * @return true if blocking succeeded, false otherwise
 */
bool scheduler_block_thread(ThreadId thread_id);

/**
 * @brief Unblock a thread
 * 
 * @param thread_id Thread ID to unblock
 * @return true if unblocking succeeded, false otherwise
 */
bool scheduler_unblock_thread(ThreadId thread_id);

/**
 * @brief Change thread priority
 * 
 * @param thread_id Thread ID
 * @param priority New priority
 * @return true if change succeeded, false otherwise
 */
bool scheduler_set_thread_priority(ThreadId thread_id, PriorityLevel priority);

/**
 * @brief Create a quantum superposition for a thread
 * 
 * @param thread_id Thread ID
 * @param resonance_level Resonance level for quantum operations
 * @return true if creation succeeded, false otherwise
 */
bool scheduler_create_superposition(ThreadId thread_id, NodeLevel resonance_level);

/**
 * @brief Collapse a thread's quantum superposition
 * 
 * @param thread_id Thread ID
 * @param probability_bias Bias towards specific outcome (0.0-1.0, 0.5 for unbiased)
 * @return true if collapse succeeded, false otherwise
 */
bool scheduler_collapse_superposition(ThreadId thread_id, double probability_bias);

/**
 * @brief Perform a context switch
 * 
 * @param force Whether to force a context switch even if time slice isn't expired
 * @return true if context switch succeeded, false otherwise
 */
bool scheduler_context_switch(bool force);

/**
 * @brief Get the currently executing thread
 * 
 * @return Currently executing thread ID or 0 if none
 */
ThreadId scheduler_get_current_thread(void);

/**
 * @brief Get the scheduler state
 * 
 * @param state Pointer to SchedulerState structure to fill
 */
void scheduler_get_state(SchedulerState* state);

/**
 * @brief Change the scheduler type
 * 
 * @param type New scheduler type
 * @return true if change succeeded, false otherwise
 */
bool scheduler_change_type(SchedulerType type);

/**
 * @brief Set the scheduler's resonance level
 * 
 * @param level New resonance level
 * @return true if change succeeded, false otherwise
 */
bool scheduler_set_resonance_level(NodeLevel level);

#endif /* CTRLXT_SCHEDULER_H */