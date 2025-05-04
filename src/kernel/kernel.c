/**
 * @file kernel.c
 * @brief Main CTRLxT OS kernel implementation
 *
 * This file implements the main CTRLxT OS kernel functionality,
 * using the Hardware Abstraction Layer for platform-specific operations.
 */

#include "kernel.h"
#include "hal/hal.h"
#include <stdio.h>

/* Kernel state variables */
static bool kernel_initialized = false;
static ArchitectureType kernel_arch = ARCH_X86;
static uint64_t kernel_memory_limit = 0;
static SystemLimits kernel_limits = {0};

/**
 * @brief Initialize the kernel
 */
bool kernel_init(ArchitectureType arch, uint64_t memory_limit) {
    /* Check if kernel is already initialized */
    if (kernel_initialized) {
        printf("Kernel already initialized\n");
        return true;
    }
    
    /* Store kernel parameters */
    kernel_arch = arch;
    kernel_memory_limit = memory_limit;
    
    /* Initialize the HAL */
    if (!hal_init()) {
        printf("Failed to initialize HAL\n");
        return false;
    }
    
    /* Get HAL operations */
    const HalOperations* hal_ops = hal_get_operations();
    if (!hal_ops) {
        printf("Failed to get HAL operations\n");
        return false;
    }
    
    /* Detect available memory */
    HalMemoryInfo mem_info;
    if (hal_ops->get_memory_info) {
        hal_ops->get_memory_info(&mem_info);
        
        /* Setup memory limits based on available memory and provided limit */
        if (kernel_memory_limit == 0 || kernel_memory_limit > mem_info.total_physical) {
            kernel_memory_limit = mem_info.total_physical;
        }
    } else {
        /* Default 8GB target if can't get memory info */
        kernel_memory_limit = 8ULL * 1024 * 1024 * 1024;
    }
    
    /* Initialize system limits */
    kernel_limits.total_memory = kernel_memory_limit;
    kernel_limits.max_processes = 1024;
    kernel_limits.max_threads = 64;
    kernel_limits.max_file_handles = 1024;
    kernel_limits.max_devices = 256;
    
    /* Check for quantum capabilities */
    bool qpu_available = false;
    if (hal_ops->has_quantum_support && hal_ops->has_quantum_support()) {
        qpu_available = true;
        printf("Quantum Processing Unit detected\n");
    }
    
    /* Initialize quantum primitives if available */
    if (qpu_available) {
        if (!kernel_init_quantum_primitives(qpu_available)) {
            printf("Warning: Failed to initialize quantum primitives\n");
            /* Continue anyway, as classical computing will still work */
        }
    }
    
    /* Print kernel information */
    printf("CTRLxT OS Kernel initialized\n");
    printf("Architecture: %s\n", hal_get_architecture_name());
    printf("Memory Limit: %llu bytes\n", (unsigned long long)kernel_memory_limit);
    printf("Resonance Level: %d\n", hal_get_resonance_level());
    
    kernel_initialized = true;
    return true;
}

/**
 * @brief Get current system limits
 */
SystemLimits kernel_get_system_limits(void) {
    return kernel_limits;
}

/**
 * @brief Initialize quantum entanglement primitives
 */
bool kernel_init_quantum_primitives(bool qpu_available) {
    /* Check if HAL has quantum support */
    const HalOperations* hal_ops = hal_get_operations();
    
    if (!qpu_available || !hal_ops->has_quantum_support || !hal_ops->has_quantum_support()) {
        printf("Quantum capabilities not available\n");
        return false;
    }
    
    /* Initialize quantum unit if possible */
    if (hal_ops->init_quantum_unit) {
        if (!hal_ops->init_quantum_unit()) {
            printf("Failed to initialize quantum unit\n");
            return false;
        }
    } else {
        printf("Quantum unit initialization not supported by HAL\n");
        return false;
    }
    
    printf("Quantum entanglement primitives initialized\n");
    return true;
}

/**
 * @brief Shutdown the kernel
 */
void kernel_shutdown(void) {
    if (!kernel_initialized) {
        return;
    }
    
    /* Shutdown the HAL */
    hal_shutdown();
    
    printf("CTRLxT OS Kernel shutdown complete\n");
    kernel_initialized = false;
}