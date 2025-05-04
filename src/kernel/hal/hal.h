/**
 * @file hal.h
 * @brief Hardware Abstraction Layer for CTRLxT OS
 *
 * This file defines the Hardware Abstraction Layer (HAL) interface
 * that provides a unified way to interact with different hardware
 * architectures in CTRLxT OS.
 */

#ifndef CTRLXT_HAL_H
#define CTRLXT_HAL_H

#include <stdint.h>
#include <stdbool.h>
#include "../../quantum/resonance/resonant_frequencies.h"

/**
 * @brief Memory permission flags
 */
#define HAL_MEM_READ      0x01    /**< Memory is readable */
#define HAL_MEM_WRITE     0x02    /**< Memory is writable */
#define HAL_MEM_EXEC      0x04    /**< Memory is executable */
#define HAL_MEM_QUANTUM   0x08    /**< Memory allows quantum operations */
#define HAL_MEM_ENTANGLED 0x10    /**< Memory is quantum-entangled */
#define HAL_MEM_SECURED   0x20    /**< Memory is secured against tampering */

/**
 * @brief Processor information structure
 */
typedef struct {
    char vendor[16];          /**< Vendor identification string */
    char model[64];           /**< Model identification string */
    uint32_t core_count;      /**< Number of processing cores */
    uint64_t frequency_mhz;   /**< Processor base frequency in MHz */
    bool has_quantum_unit;    /**< Whether processor has quantum processing capabilities */
    uint32_t cache_l1;        /**< L1 cache size in KB */
    uint32_t cache_l2;        /**< L2 cache size in KB */
    uint32_t cache_l3;        /**< L3 cache size in KB */
    uint32_t feature_flags;   /**< Processor feature flags */
    NodeLevel resonance_level; /**< Processor's resonance level */
} HalProcessorInfo;

/**
 * @brief Memory information structure
 */
typedef struct {
    uint64_t total_physical;     /**< Total physical memory in bytes */
    uint64_t available_physical; /**< Available physical memory in bytes */
    uint64_t total_quantum;      /**< Total quantum memory in qubits */
    uint64_t available_quantum;  /**< Available quantum memory in qubits */
    uint32_t page_size;          /**< Memory page size in bytes */
    bool supports_entanglement;  /**< Whether memory supports quantum entanglement */
    uint32_t entanglement_limit; /**< Maximum entangled memory regions */
    NodeLevel resonance_level;   /**< Memory's resonance level */
} HalMemoryInfo;

/**
 * @brief Interrupt handler function type
 */
typedef void (*HalInterruptHandler)(uint32_t interrupt_number, void* context);

/**
 * @brief Generic physical address type
 */
typedef uint64_t HalPhysicalAddr;

/**
 * @brief Generic virtual address type
 */
typedef void* HalVirtualAddr;

/**
 * @brief Operations provided by the HAL
 */
typedef struct {
    /* Initialization functions */
    bool (*init)(void);          /**< Initialize the HAL */
    void (*shutdown)(void);      /**< Shutdown the HAL */
    
    /* Processor functions */
    void (*get_processor_info)(HalProcessorInfo* info);  /**< Get processor information */
    void (*halt_processor)(void);                       /**< Halt the processor */
    void (*reset_processor)(void);                      /**< Reset the processor */
    
    /* Memory functions */
    void (*get_memory_info)(HalMemoryInfo* info);       /**< Get memory information */
    HalPhysicalAddr (*alloc_physical_page)(void);       /**< Allocate a physical memory page */
    void (*free_physical_page)(HalPhysicalAddr addr);   /**< Free a physical memory page */
    HalVirtualAddr (*map_physical_memory)(HalPhysicalAddr phys, uint64_t size, uint32_t permissions); /**< Map physical memory to virtual memory */
    void (*unmap_physical_memory)(HalVirtualAddr virt, uint64_t size);  /**< Unmap physical memory */
    void (*set_memory_permissions)(HalVirtualAddr addr, uint64_t size, uint32_t permissions); /**< Set memory region permissions */
    
    /* Interrupt functions */
    void (*enable_interrupts)(void);                    /**< Enable interrupts */
    void (*disable_interrupts)(void);                   /**< Disable interrupts */
    void (*set_interrupt_handler)(uint32_t interrupt_number, HalInterruptHandler handler, void* context); /**< Set interrupt handler */
    
    /* I/O functions */
    uint8_t (*in_byte)(uint16_t port);                  /**< Read a byte from an I/O port */
    void (*out_byte)(uint16_t port, uint8_t value);     /**< Write a byte to an I/O port */
    uint16_t (*in_word)(uint16_t port);                 /**< Read a word from an I/O port */
    void (*out_word)(uint16_t port, uint16_t value);    /**< Write a word to an I/O port */
    uint32_t (*in_dword)(uint16_t port);                /**< Read a double word from an I/O port */
    void (*out_dword)(uint16_t port, uint32_t value);   /**< Write a double word to an I/O port */
    
    /* Timer functions */
    uint64_t (*get_timestamp)(void);                    /**< Get current timestamp in nanoseconds */
    void (*sleep)(uint64_t nanoseconds);                /**< Sleep for specified nanoseconds */
    
    /* Quantum functions */
    bool (*has_quantum_support)(void);                  /**< Check if hardware has quantum support */
    bool (*init_quantum_unit)(void);                    /**< Initialize quantum processing unit */
    bool (*allocate_qubits)(uint32_t count);            /**< Allocate qubits from quantum memory */
    void (*free_qubits)(uint32_t qubit_id, uint32_t count); /**< Free previously allocated qubits */
    bool (*entangle_memory)(HalVirtualAddr addr1, HalVirtualAddr addr2, uint64_t size); /**< Quantum-entangle two memory regions */
    
    /* Architecture-specific functions */
    void (*arch_specific_op)(uint32_t op_code, void* params); /**< Architecture-specific operation */
} HalOperations;

/**
 * @brief Get the HAL operations for the current architecture
 * 
 * @return Pointer to the HalOperations structure for the current architecture
 */
const HalOperations* hal_get_operations(void);

/**
 * @brief Initialize the HAL for the current architecture
 * 
 * @return true if initialization succeeded, false otherwise
 */
bool hal_init(void);

/**
 * @brief Shutdown the HAL
 */
void hal_shutdown(void);

/**
 * @brief Get the architecture name
 * 
 * @return String with the architecture name
 */
const char* hal_get_architecture_name(void);

/**
 * @brief Get the resonance level of the hardware
 * 
 * @return NodeLevel representing the hardware's resonance level
 */
NodeLevel hal_get_resonance_level(void);

#endif /* CTRLXT_HAL_H */