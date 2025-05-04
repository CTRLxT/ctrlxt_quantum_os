/**
 * @file process_manager.h
 * @brief Process Management System for CTRLxT OS
 *
 * This file defines the interface for the process management system,
 * which handles process creation, scheduling, and quantum entanglement
 * between processes.
 */

#ifndef CTRLXT_PROCESS_MANAGER_H
#define CTRLXT_PROCESS_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "../memory/memory_manager.h"
#include "../../quantum/resonance/resonant_frequencies.h"

/* Process ID types */
typedef uint64_t ProcessId;
typedef uint64_t ThreadId;

/* Maximum process and thread limits */
#define MAX_PROCESSES 1024
#define MAX_THREADS_PER_PROCESS 64

/**
 * @brief Process states
 */
typedef enum {
    PROCESS_CREATED,      /**< Process created but not started */
    PROCESS_RUNNING,      /**< Process is currently running */
    PROCESS_READY,        /**< Process is ready to run */
    PROCESS_BLOCKED,      /**< Process is blocked waiting for a resource */
    PROCESS_SUSPENDED,    /**< Process is suspended by user or system */
    PROCESS_TERMINATED,   /**< Process has terminated */
    PROCESS_QUANTUM       /**< Process is in quantum superposition state */
} ProcessState;

/**
 * @brief Thread states
 */
typedef enum {
    THREAD_CREATED,       /**< Thread created but not started */
    THREAD_RUNNING,       /**< Thread is currently running */
    THREAD_READY,         /**< Thread is ready to run */
    THREAD_BLOCKED,       /**< Thread is blocked waiting for a resource */
    THREAD_SUSPENDED,     /**< Thread is suspended by user or system */
    THREAD_TERMINATED,    /**< Thread has terminated */
    THREAD_QUANTUM        /**< Thread is in quantum superposition state */
} ThreadState;

/**
 * @brief Process priority levels
 */
typedef enum {
    PRIORITY_LOWEST = 0,  /**< Lowest priority */
    PRIORITY_LOW = 1,      /**< Low priority */
    PRIORITY_NORMAL = 2,   /**< Normal priority */
    PRIORITY_HIGH = 3,     /**< High priority */
    PRIORITY_HIGHEST = 4,  /**< Highest priority */
    PRIORITY_REALTIME = 5, /**< Real-time priority */
    PRIORITY_QUANTUM = 6   /**< Quantum priority (highest) */
} PriorityLevel;

/**
 * @brief Process entanglement types
 */
typedef enum {
    ENTANGLE_NONE,        /**< No entanglement */
    ENTANGLE_MEMORY,      /**< Memory entanglement */
    ENTANGLE_STATE,       /**< Process state entanglement */
    ENTANGLE_EXECUTION,   /**< Execution entanglement (lockstep) */
    ENTANGLE_RESONANCE    /**< Resonance entanglement (quantum) */
} ProcessEntanglementType;

/**
 * @brief Process entanglement structure
 */
typedef struct {
    uint64_t id;                   /**< Entanglement ID */
    ProcessId first_process;       /**< First process ID */
    ProcessId second_process;      /**< Second process ID */
    ProcessEntanglementType type;  /**< Entanglement type */
    NodeLevel resonance_level;     /**< Resonance level */
    double stability;              /**< Stability (0.0 to 1.0) */
    bool is_synchronized;          /**< Whether processes are synchronized */
} ProcessEntanglement;

/**
 * @brief Thread context structure
 * 
 * This is architecture-dependent and will be expanded
 * in architecture-specific implementations.
 */
typedef struct {
    uint64_t registers[16];       /**< General purpose registers */
    uint64_t program_counter;     /**< Program counter */
    uint64_t stack_pointer;       /**< Stack pointer */
    uint64_t flags;               /**< CPU flags */
} ThreadContext;

/**
 * @brief Thread structure
 */
typedef struct Thread {
    ThreadId id;                   /**< Thread ID */
    ProcessId process_id;          /**< Parent process ID */
    ThreadState state;             /**< Thread state */
    PriorityLevel priority;        /**< Thread priority */
    ThreadContext context;         /**< Thread execution context */
    HalVirtualAddr stack_base;     /**< Base of thread stack */
    uint64_t stack_size;           /**< Size of thread stack */
    uint64_t quantum_time;         /**< Quantum time slice in nanoseconds */
    uint64_t execution_time;       /**< Total execution time in nanoseconds */
    uint64_t last_scheduled;       /**< Last scheduled timestamp */
    void* entry_point;             /**< Thread entry point */
    void* user_data;               /**< User data pointer */
    NodeLevel resonance_level;     /**< Thread resonance level */
    struct Thread* next;           /**< Next thread in list */
    struct Thread* prev;           /**< Previous thread in list */
} Thread;

/**
 * @brief Process structure
 */
typedef struct Process {
    ProcessId id;                  /**< Process ID */
    char name[64];                 /**< Process name */
    ProcessState state;            /**< Process state */
    PriorityLevel priority;        /**< Process priority */
    HalVirtualAddr memory_map;     /**< Memory map base address */
    uint64_t memory_size;          /**< Total memory size */
    Thread* threads;               /**< List of threads */
    uint32_t thread_count;         /**< Number of threads */
    uint64_t entanglement_id;      /**< Entanglement ID (if entangled) */
    HalVirtualAddr code_segment;   /**< Code segment base */
    HalVirtualAddr data_segment;   /**< Data segment base */
    HalVirtualAddr heap;           /**< Heap base */
    uint64_t exit_code;            /**< Exit code */
    uint64_t creation_time;        /**< Creation timestamp */
    uint64_t execution_time;       /**< Total execution time in nanoseconds */
    NodeLevel resonance_level;     /**< Process resonance level */
    struct Process* next;          /**< Next process in list */
    struct Process* prev;          /**< Previous process in list */
} Process;

/**
 * @brief Process statistics structure
 */
typedef struct {
    uint32_t total_processes;       /**< Total number of processes */
    uint32_t running_processes;     /**< Number of running processes */
    uint32_t blocked_processes;     /**< Number of blocked processes */
    uint32_t quantum_processes;     /**< Number of quantum processes */
    uint32_t total_threads;         /**< Total number of threads */
    uint32_t total_entanglements;   /**< Total number of process entanglements */
    uint64_t total_context_switches;/**< Total number of context switches */
    uint64_t total_quantum_ops;     /**< Total number of quantum operations */
} ProcessStats;

/**
 * @brief Process creation parameters
 */
typedef struct {
    char name[64];                 /**< Process name */
    HalVirtualAddr entry_point;    /**< Process entry point */
    uint64_t stack_size;           /**< Stack size for main thread */
    uint64_t heap_size;            /**< Initial heap size */
    PriorityLevel priority;        /**< Process priority */
    bool quantum_capable;          /**< Whether process supports quantum operations */
    NodeLevel resonance_level;     /**< Process resonance level */
} ProcessParams;

/**
 * @brief Thread creation parameters
 */
typedef struct {
    ProcessId process_id;          /**< Parent process ID */
    HalVirtualAddr entry_point;    /**< Thread entry point */
    void* arg;                     /**< Argument to pass to thread */
    uint64_t stack_size;           /**< Thread stack size */
    PriorityLevel priority;        /**< Thread priority */
    bool quantum_capable;          /**< Whether thread supports quantum operations */
} ThreadParams;

/**
 * @brief Initialize the process management system
 * 
 * @param max_processes Maximum number of processes
 * @return true if initialization succeeded, false otherwise
 */
bool pm_init(uint32_t max_processes);

/**
 * @brief Shutdown the process management system
 */
void pm_shutdown(void);

/**
 * @brief Create a new process
 * 
 * @param params Process creation parameters
 * @param process_id Pointer to store the new process ID
 * @return true if creation succeeded, false otherwise
 */
bool pm_create_process(const ProcessParams* params, ProcessId* process_id);

/**
 * @brief Terminate a process
 * 
 * @param process_id Process ID to terminate
 * @param exit_code Exit code
 * @return true if termination succeeded, false otherwise
 */
bool pm_terminate_process(ProcessId process_id, uint64_t exit_code);

/**
 * @brief Create a new thread in a process
 * 
 * @param params Thread creation parameters
 * @param thread_id Pointer to store the new thread ID
 * @return true if creation succeeded, false otherwise
 */
bool pm_create_thread(const ThreadParams* params, ThreadId* thread_id);

/**
 * @brief Terminate a thread
 * 
 * @param thread_id Thread ID to terminate
 * @param exit_code Exit code
 * @return true if termination succeeded, false otherwise
 */
bool pm_terminate_thread(ThreadId thread_id, uint64_t exit_code);

/**
 * @brief Get information about a process
 * 
 * @param process_id Process ID
 * @return Pointer to the process or NULL if not found
 */
Process* pm_get_process(ProcessId process_id);

/**
 * @brief Get information about a thread
 * 
 * @param thread_id Thread ID
 * @return Pointer to the thread or NULL if not found
 */
Thread* pm_get_thread(ThreadId thread_id);

/**
 * @brief Set process priority
 * 
 * @param process_id Process ID
 * @param priority New priority level
 * @return true if priority change succeeded, false otherwise
 */
bool pm_set_process_priority(ProcessId process_id, PriorityLevel priority);

/**
 * @brief Set thread priority
 * 
 * @param thread_id Thread ID
 * @param priority New priority level
 * @return true if priority change succeeded, false otherwise
 */
bool pm_set_thread_priority(ThreadId thread_id, PriorityLevel priority);

/**
 * @brief Create quantum entanglement between two processes
 * 
 * @param first_process_id First process ID
 * @param second_process_id Second process ID
 * @param type Entanglement type
 * @param resonance_level Quantum resonance level
 * @return Entanglement ID or 0 on failure
 */
uint64_t pm_create_process_entanglement(ProcessId first_process_id, ProcessId second_process_id,
                                        ProcessEntanglementType type, NodeLevel resonance_level);

/**
 * @brief Break process entanglement
 * 
 * @param entanglement_id Entanglement ID
 * @return true if breaking succeeded, false otherwise
 */
bool pm_break_process_entanglement(uint64_t entanglement_id);

/**
 * @brief Synchronize entangled processes
 * 
 * @param entanglement_id Entanglement ID
 * @return true if synchronization succeeded, false otherwise
 */
bool pm_sync_process_entanglement(uint64_t entanglement_id);

/**
 * @brief Get a list of all processes
 * 
 * @param processes Array to fill with process pointers
 * @param max_count Maximum number of processes to return
 * @return Number of processes returned
 */
uint32_t pm_get_all_processes(Process** processes, uint32_t max_count);

/**
 * @brief Get a list of all threads for a process
 * 
 * @param process_id Process ID
 * @param threads Array to fill with thread pointers
 * @param max_count Maximum number of threads to return
 * @return Number of threads returned
 */
uint32_t pm_get_process_threads(ProcessId process_id, Thread** threads, uint32_t max_count);

/**
 * @brief Get process statistics
 * 
 * @param stats Pointer to ProcessStats structure to fill
 */
void pm_get_stats(ProcessStats* stats);

/**
 * @brief Print process statistics
 */
void pm_print_stats(void);

#endif /* CTRLXT_PROCESS_MANAGER_H */