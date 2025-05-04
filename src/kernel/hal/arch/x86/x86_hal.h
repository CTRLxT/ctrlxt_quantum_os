/**
 * @file x86_hal.h
 * @brief x86 Architecture-specific HAL declarations
 *
 * This file contains declarations for x86-specific HAL functions.
 */

#ifndef CTRLXT_X86_HAL_H
#define CTRLXT_X86_HAL_H

#include "../../hal.h"

/**
 * @brief x86-specific operations
 */
enum X86SpecificOps {
    X86_OP_CPUID = 0,        /**< Execute CPUID instruction */
    X86_OP_RDMSR,            /**< Read Model-Specific Register */
    X86_OP_WRMSR,            /**< Write Model-Specific Register */
    X86_OP_INVLPG,           /**< Invalidate TLB entry */
    X86_OP_SET_CR0,          /**< Set CR0 register */
    X86_OP_GET_CR0,          /**< Get CR0 register */
    X86_OP_SET_CR3,          /**< Set CR3 register (page directory) */
    X86_OP_GET_CR3,          /**< Get CR3 register */
    X86_OP_SET_CR4,          /**< Set CR4 register */
    X86_OP_GET_CR4           /**< Get CR4 register */
};

/**
 * @brief CPUID operation parameters
 */
typedef struct {
    uint32_t eax_input;      /**< EAX input value */
    uint32_t ecx_input;      /**< ECX input value */
    uint32_t eax_output;     /**< EAX output value */
    uint32_t ebx_output;     /**< EBX output value */
    uint32_t ecx_output;     /**< ECX output value */
    uint32_t edx_output;     /**< EDX output value */
} X86CpuidParams;

/**
 * @brief MSR operation parameters
 */
typedef struct {
    uint32_t msr_index;      /**< MSR index */
    uint32_t value_low;      /**< Lower 32 bits of value */
    uint32_t value_high;     /**< Upper 32 bits of value */
} X86MsrParams;

/**
 * @brief Get the x86 HAL operations
 * 
 * @return Pointer to the HalOperations structure for x86
 */
const HalOperations* x86_get_hal_operations(void);

/**
 * @brief Initialize the x86 HAL
 * 
 * @return true if initialization succeeded, false otherwise
 */
bool x86_hal_init(void);

/**
 * @brief Shutdown the x86 HAL
 */
void x86_hal_shutdown(void);

/**
 * @brief Get processor information for x86
 * 
 * @param info Pointer to HalProcessorInfo structure to fill
 */
void x86_get_processor_info(HalProcessorInfo* info);

/**
 * @brief Get memory information for x86
 * 
 * @param info Pointer to HalMemoryInfo structure to fill
 */
void x86_get_memory_info(HalMemoryInfo* info);

/**
 * @brief Check if x86 hardware has quantum support
 * 
 * @return true if quantum support is available, false otherwise
 */
bool x86_has_quantum_support(void);

#endif /* CTRLXT_X86_HAL_H */