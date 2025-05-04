/**
 * @file hal.c
 * @brief Hardware Abstraction Layer implementation
 *
 * This file implements the generic HAL functions that determine and use
 * the appropriate architecture-specific HAL.
 */

#include "hal.h"
#include "arch/x86/x86_hal.h"
/* Include other architectures as they are implemented */
/* #include "arch/arm/arm_hal.h" */
/* #include "arch/riscv/riscv_hal.h" */

/* Current architecture HAL operations */
static const HalOperations* current_hal_ops = NULL;

/* Current architecture name */
static const char* current_architecture = "unknown";

/**
 * @brief Detect the current architecture
 * 
 * @return Pointer to the appropriate HAL operations for the detected architecture
 */
static const HalOperations* detect_architecture(void) {
    /* For now, we'll just assume x86 architecture */
    /* In a real implementation, this would actually detect the hardware */
    current_architecture = "x86";
    return x86_get_hal_operations();
    
    /* ARM detection would look like this:
    current_architecture = "arm";
    return arm_get_hal_operations();
    */
    
    /* RISC-V detection would look like this:
    current_architecture = "riscv";
    return riscv_get_hal_operations();
    */
}

/**
 * @brief Get the HAL operations for the current architecture
 */
const HalOperations* hal_get_operations(void) {
    if (current_hal_ops == NULL) {
        /* Detect architecture on first call */
        current_hal_ops = detect_architecture();
    }
    
    return current_hal_ops;
}

/**
 * @brief Initialize the HAL for the current architecture
 */
bool hal_init(void) {
    /* Get the HAL operations for the current architecture */
    const HalOperations* ops = hal_get_operations();
    
    /* Check if we have valid operations */
    if (ops == NULL || ops->init == NULL) {
        return false;
    }
    
    /* Initialize the HAL */
    return ops->init();
}

/**
 * @brief Shutdown the HAL
 */
void hal_shutdown(void) {
    /* Get the HAL operations for the current architecture */
    const HalOperations* ops = hal_get_operations();
    
    /* Check if we have valid operations */
    if (ops == NULL || ops->shutdown == NULL) {
        return;
    }
    
    /* Shutdown the HAL */
    ops->shutdown();
}

/**
 * @brief Get the architecture name
 */
const char* hal_get_architecture_name(void) {
    /* Make sure we've detected the architecture */
    hal_get_operations();
    
    return current_architecture;
}

/**
 * @brief Get the resonance level of the hardware
 */
NodeLevel hal_get_resonance_level(void) {
    /* Get the HAL operations for the current architecture */
    const HalOperations* ops = hal_get_operations();
    
    /* Check if we have valid operations */
    if (ops == NULL || ops->get_processor_info == NULL) {
        return NODE_ZERO_POINT; /* Default to base level */
    }
    
    /* Get processor info to determine resonance level */
    HalProcessorInfo proc_info;
    ops->get_processor_info(&proc_info);
    
    return proc_info.resonance_level;
}