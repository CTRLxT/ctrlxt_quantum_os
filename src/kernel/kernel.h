/**
 * @file kernel.h
 * @brief Main CTRLxT OS kernel interface
 *
 * This file defines the main interface for the CTRLxT OS unified kernel,
 * which supports multiple architectures and integrates quantum computing
 * principles with classical computing.
 */

#ifndef CTRLXT_KERNEL_H
#define CTRLXT_KERNEL_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Architecture types supported by the kernel
 */
typedef enum {
    ARCH_X86,       /**< x86/x86_64 architecture */
    ARCH_ARM,       /**< ARM architecture */
    ARCH_QPU,       /**< Quantum Processing Unit */
    ARCH_HYBRID     /**< Hybrid classical-quantum */
} ArchitectureType;

/**
 * @brief System resource limits
 */
typedef struct {
    uint64_t total_memory;     /**< Total system memory in bytes */
    uint32_t max_processes;    /**< Maximum number of processes */
    uint32_t max_threads;      /**< Maximum number of threads per process */
    uint32_t max_file_handles; /**< Maximum number of open file handles */
    uint32_t max_devices;      /**< Maximum number of connected devices */
} SystemLimits;

/**
 * @brief Initialize the kernel
 * 
 * @param arch Architecture type
 * @param memory_limit Memory limit in bytes (0 for automatic)
 * @return true if initialization succeeded, false otherwise
 */
bool kernel_init(ArchitectureType arch, uint64_t memory_limit);

/**
 * @brief Get current system limits
 * 
 * @return SystemLimits structure with current limits
 */
SystemLimits kernel_get_system_limits(void);

/**
 * @brief Initialize quantum entanglement primitives
 * 
 * @param qpu_available Whether a QPU is available
 * @return true if initialization succeeded, false otherwise
 */
bool kernel_init_quantum_primitives(bool qpu_available);

/**
 * @brief Shutdown the kernel
 */
void kernel_shutdown(void);

#endif /* CTRLXT_KERNEL_H */