/**
 * @file memory_manager.h
 * @brief Kernel Memory Management System
 *
 * This file defines the interface for the kernel memory management system,
 * which handles physical and virtual memory allocation, paging, and
 * quantum memory entanglement.
 */

#ifndef CTRLXT_MEMORY_MANAGER_H
#define CTRLXT_MEMORY_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "../hal/hal.h"
#include "../../quantum/resonance/resonant_frequencies.h"

/**
 * @brief Memory region flags
 */
#define MM_FLAG_READ       0x01    /**< Memory is readable */
#define MM_FLAG_WRITE      0x02    /**< Memory is writable */
#define MM_FLAG_EXEC       0x04    /**< Memory is executable */
#define MM_FLAG_QUANTUM    0x08    /**< Memory allows quantum operations */
#define MM_FLAG_ENTANGLED  0x10    /**< Memory is quantum-entangled */
#define MM_FLAG_CACHED     0x20    /**< Memory is cached */
#define MM_FLAG_USER       0x40    /**< Memory is accessible by user mode */
#define MM_FLAG_SYSTEM     0x80    /**< Memory is reserved for system use */

/**
 * @brief Memory region types
 */
typedef enum {
    MEMORY_TYPE_RAM,           /**< Standard RAM */
    MEMORY_TYPE_QUANTUM,       /**< Quantum memory */
    MEMORY_TYPE_DEVICE,        /**< Device memory */
    MEMORY_TYPE_SHARED,        /**< Shared memory */
    MEMORY_TYPE_ENTANGLED      /**< Quantum-entangled memory */
} MemoryType;

/**
 * @brief Memory region structure
 */
typedef struct MemoryRegion {
    HalVirtualAddr start;         /**< Start address */
    uint64_t size;                /**< Size in bytes */
    MemoryType type;              /**< Memory type */
    uint32_t flags;               /**< Memory flags */
    HalPhysicalAddr physical;     /**< Physical address (if mapped) */
    uint64_t entanglement_id;     /**< Entanglement ID (if entangled) */
    struct MemoryRegion* next;    /**< Next region in list */
    struct MemoryRegion* prev;    /**< Previous region in list */
    NodeLevel resonance_level;    /**< Resonance level for quantum operations */
} MemoryRegion;

/**
 * @brief Entanglement information structure
 */
typedef struct {
    uint64_t id;                   /**< Entanglement ID */
    MemoryRegion* first_region;    /**< First entangled region */
    MemoryRegion* second_region;   /**< Second entangled region */
    NodeLevel resonance_level;     /**< Resonance level of entanglement */
    double stability;              /**< Entanglement stability (0.0 to 1.0) */
    bool is_synchronized;          /**< Whether regions are currently synchronized */
} EntanglementInfo;

/**
 * @brief Memory allocation information for tracking
 */
typedef struct {
    uint64_t total_physical;      /**< Total physical memory */
    uint64_t used_physical;       /**< Used physical memory */
    uint64_t free_physical;       /**< Free physical memory */
    uint64_t total_virtual;       /**< Total virtual address space */
    uint64_t used_virtual;        /**< Used virtual address space */
    uint64_t free_virtual;        /**< Free virtual address space */
    uint32_t total_regions;       /**< Total number of memory regions */
    uint32_t total_entanglements; /**< Total number of memory entanglements */
    uint64_t total_quantum;       /**< Total quantum memory in qubits */
    uint64_t used_quantum;        /**< Used quantum memory in qubits */
} MemoryStats;

/**
 * @brief Initialize the memory management system
 * 
 * @param memory_limit Maximum memory to use in bytes (0 for unlimited)
 * @return true if initialization succeeded, false otherwise
 */
bool mm_init(uint64_t memory_limit);

/**
 * @brief Shutdown the memory management system
 */
void mm_shutdown(void);

/**
 * @brief Allocate physical memory
 * 
 * @param size Size to allocate in bytes
 * @param alignment Required alignment (power of 2, 0 for default)
 * @return Physical address or 0 on failure
 */
HalPhysicalAddr mm_alloc_physical(uint64_t size, uint32_t alignment);

/**
 * @brief Free physical memory
 * 
 * @param addr Physical address to free
 * @param size Size to free in bytes
 * @return true if freeing succeeded, false otherwise
 */
bool mm_free_physical(HalPhysicalAddr addr, uint64_t size);

/**
 * @brief Allocate virtual memory
 * 
 * @param size Size to allocate in bytes
 * @param type Memory type
 * @param flags Memory flags
 * @return Virtual address or NULL on failure
 */
HalVirtualAddr mm_alloc_virtual(uint64_t size, MemoryType type, uint32_t flags);

/**
 * @brief Free virtual memory
 * 
 * @param addr Virtual address to free
 * @return true if freeing succeeded, false otherwise
 */
bool mm_free_virtual(HalVirtualAddr addr);

/**
 * @brief Map physical memory to virtual memory
 * 
 * @param physical Physical address to map
 * @param size Size to map in bytes
 * @param flags Memory flags
 * @return Mapped virtual address or NULL on failure
 */
HalVirtualAddr mm_map_physical(HalPhysicalAddr physical, uint64_t size, uint32_t flags);

/**
 * @brief Get information about a memory region
 * 
 * @param addr Address within the region
 * @return Pointer to the memory region or NULL if not found
 */
MemoryRegion* mm_get_region_info(HalVirtualAddr addr);

/**
 * @brief Create quantum entanglement between two memory regions
 * 
 * @param first_addr First memory region address
 * @param second_addr Second memory region address
 * @param resonance_level Quantum resonance level for entanglement
 * @return Entanglement ID or 0 on failure
 */
uint64_t mm_create_entanglement(HalVirtualAddr first_addr, HalVirtualAddr second_addr, NodeLevel resonance_level);

/**
 * @brief Break quantum entanglement
 * 
 * @param entanglement_id Entanglement ID to break
 * @return true if breaking succeeded, false otherwise
 */
bool mm_break_entanglement(uint64_t entanglement_id);

/**
 * @brief Get information about an entanglement
 * 
 * @param entanglement_id Entanglement ID
 * @param info Pointer to EntanglementInfo structure to fill
 * @return true if information was retrieved, false otherwise
 */
bool mm_get_entanglement_info(uint64_t entanglement_id, EntanglementInfo* info);

/**
 * @brief Synchronize quantum-entangled memory regions
 * 
 * @param entanglement_id Entanglement ID to synchronize
 * @return true if synchronization succeeded, false otherwise
 */
bool mm_sync_entanglement(uint64_t entanglement_id);

/**
 * @brief Get memory statistics
 * 
 * @param stats Pointer to MemoryStats structure to fill
 */
void mm_get_stats(MemoryStats* stats);

/**
 * @brief Print memory statistics
 */
void mm_print_stats(void);

#endif /* CTRLXT_MEMORY_MANAGER_H */