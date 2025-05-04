/**
 * @file memory_manager.c
 * @brief Kernel Memory Management System implementation
 */

#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Memory management state */
static bool mm_initialized = false;
static uint64_t mm_memory_limit = 0;
static MemoryStats mm_stats = {0};

/* Memory region linked list */
static MemoryRegion* mm_regions_head = NULL;
static uint32_t mm_next_region_id = 1;

/* Entanglement table */
#define MAX_ENTANGLEMENTS 256
static EntanglementInfo mm_entanglements[MAX_ENTANGLEMENTS];
static uint64_t mm_next_entanglement_id = 1;

/**
 * @brief Find a memory region containing the given address
 */
static MemoryRegion* find_region(HalVirtualAddr addr) {
    MemoryRegion* region = mm_regions_head;
    
    while (region) {
        if (addr >= region->start && addr < (HalVirtualAddr)((uint8_t*)region->start + region->size)) {
            return region;
        }
        region = region->next;
    }
    
    return NULL;
}

/**
 * @brief Find an entanglement by ID
 */
static EntanglementInfo* find_entanglement(uint64_t entanglement_id) {
    for (int i = 0; i < MAX_ENTANGLEMENTS; i++) {
        if (mm_entanglements[i].id == entanglement_id) {
            return &mm_entanglements[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Find the next free entanglement slot
 */
static EntanglementInfo* find_free_entanglement_slot(void) {
    for (int i = 0; i < MAX_ENTANGLEMENTS; i++) {
        if (mm_entanglements[i].id == 0) {
            return &mm_entanglements[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Add a memory region to the linked list
 */
static void add_region(MemoryRegion* region) {
    /* Add to the start of the list for simplicity */
    region->next = mm_regions_head;
    region->prev = NULL;
    
    if (mm_regions_head) {
        mm_regions_head->prev = region;
    }
    
    mm_regions_head = region;
    mm_stats.total_regions++;
}

/**
 * @brief Remove a memory region from the linked list
 */
static void remove_region(MemoryRegion* region) {
    if (region->prev) {
        region->prev->next = region->next;
    } else {
        mm_regions_head = region->next;
    }
    
    if (region->next) {
        region->next->prev = region->prev;
    }
    
    mm_stats.total_regions--;
}

/**
 * @brief Update memory statistics after allocation/freeing
 */
static void update_stats_after_alloc(uint64_t size, MemoryType type) {
    mm_stats.used_virtual += size;
    mm_stats.free_virtual -= size;
    
    if (type == MEMORY_TYPE_RAM || type == MEMORY_TYPE_SHARED) {
        mm_stats.used_physical += size;
        mm_stats.free_physical -= size;
    } else if (type == MEMORY_TYPE_QUANTUM || type == MEMORY_TYPE_ENTANGLED) {
        /* Quantum memory is tracked separately */
        uint64_t qubits_needed = (size * 8) / 32; /* Approximate conversion */
        mm_stats.used_quantum += qubits_needed;
    }
}

/**
 * @brief Update memory statistics after deallocation
 */
static void update_stats_after_free(MemoryRegion* region) {
    mm_stats.used_virtual -= region->size;
    mm_stats.free_virtual += region->size;
    
    if (region->type == MEMORY_TYPE_RAM || region->type == MEMORY_TYPE_SHARED) {
        mm_stats.used_physical -= region->size;
        mm_stats.free_physical += region->size;
    } else if (region->type == MEMORY_TYPE_QUANTUM || region->type == MEMORY_TYPE_ENTANGLED) {
        /* Quantum memory is tracked separately */
        uint64_t qubits_freed = (region->size * 8) / 32; /* Approximate conversion */
        mm_stats.used_quantum -= qubits_freed;
    }
}

/**
 * @brief Initialize the memory management system
 */
bool mm_init(uint64_t memory_limit) {
    if (mm_initialized) {
        return true;
    }
    
    /* Get the HAL operations */
    const HalOperations* hal_ops = hal_get_operations();
    if (!hal_ops) {
        printf("Failed to get HAL operations\n");
        return false;
    }
    
    /* Get memory information from HAL */
    HalMemoryInfo mem_info;
    if (hal_ops->get_memory_info) {
        hal_ops->get_memory_info(&mem_info);
        
        /* Set memory limit based on available memory */
        if (memory_limit == 0 || memory_limit > mem_info.total_physical) {
            mm_memory_limit = mem_info.total_physical;
        } else {
            mm_memory_limit = memory_limit;
        }
        
        /* Initialize memory statistics */
        mm_stats.total_physical = mem_info.total_physical;
        mm_stats.free_physical = mem_info.available_physical;
        mm_stats.used_physical = mem_info.total_physical - mem_info.available_physical;
        
        mm_stats.total_virtual = 0xFFFFFFFFFFFFFFFFULL; /* Full 64-bit address space */
        mm_stats.free_virtual = mm_stats.total_virtual;
        mm_stats.used_virtual = 0;
        
        mm_stats.total_regions = 0;
        mm_stats.total_entanglements = 0;
        
        /* Quantum memory stats */
        mm_stats.total_quantum = mem_info.total_quantum;
        mm_stats.used_quantum = 0;
    } else {
        /* Default to 8GB if memory info not available */
        mm_memory_limit = 8ULL * 1024 * 1024 * 1024;
        
        mm_stats.total_physical = mm_memory_limit;
        mm_stats.free_physical = mm_memory_limit;
        mm_stats.used_physical = 0;
        
        mm_stats.total_virtual = 0xFFFFFFFFFFFFFFFFULL;
        mm_stats.free_virtual = mm_stats.total_virtual;
        mm_stats.used_virtual = 0;
        
        mm_stats.total_regions = 0;
        mm_stats.total_entanglements = 0;
        
        mm_stats.total_quantum = 0;
        mm_stats.used_quantum = 0;
    }
    
    /* Initialize entanglement table */
    memset(mm_entanglements, 0, sizeof(mm_entanglements));
    
    printf("Memory Manager initialized\n");
    printf("Total Physical Memory: %llu bytes\n", (unsigned long long)mm_stats.total_physical);
    printf("Total Quantum Memory: %llu qubits\n", (unsigned long long)mm_stats.total_quantum);
    
    mm_initialized = true;
    return true;
}

/**
 * @brief Shutdown the memory management system
 */
void mm_shutdown(void) {
    if (!mm_initialized) {
        return;
    }
    
    /* Free all memory regions */
    while (mm_regions_head) {
        MemoryRegion* next = mm_regions_head->next;
        mm_free_virtual(mm_regions_head->start);
        mm_regions_head = next;
    }
    
    /* Clear entanglement table */
    memset(mm_entanglements, 0, sizeof(mm_entanglements));
    
    /* Reset statistics */
    memset(&mm_stats, 0, sizeof(mm_stats));
    
    mm_initialized = false;
    printf("Memory Manager shutdown complete\n");
}

/**
 * @brief Allocate physical memory
 */
HalPhysicalAddr mm_alloc_physical(uint64_t size, uint32_t alignment) {
    if (!mm_initialized) {
        return 0;
    }
    
    /* Check if we have enough free physical memory */
    if (size > mm_stats.free_physical) {
        printf("Not enough free physical memory\n");
        return 0;
    }
    
    /* Get HAL operations */
    const HalOperations* hal_ops = hal_get_operations();
    if (!hal_ops || !hal_ops->alloc_physical_page) {
        printf("Physical memory allocation not supported by HAL\n");
        return 0;
    }
    
    /* Calculate number of pages needed */
    HalMemoryInfo mem_info;
    hal_ops->get_memory_info(&mem_info);
    uint32_t page_size = mem_info.page_size;
    uint32_t pages_needed = (size + page_size - 1) / page_size;
    
    /* Allocate pages */
    /* In a real implementation, this would handle alignment and contiguous allocation */
    /* For now, we'll just simulate a successful allocation */
    
    HalPhysicalAddr physical_addr = 0x1000000; /* Simulated address */
    
    /* Update statistics */
    mm_stats.used_physical += size;
    mm_stats.free_physical -= size;
    
    return physical_addr;
}

/**
 * @brief Free physical memory
 */
bool mm_free_physical(HalPhysicalAddr addr, uint64_t size) {
    if (!mm_initialized) {
        return false;
    }
    
    /* Get HAL operations */
    const HalOperations* hal_ops = hal_get_operations();
    if (!hal_ops || !hal_ops->free_physical_page) {
        printf("Physical memory deallocation not supported by HAL\n");
        return false;
    }
    
    /* In a real implementation, this would free the actual physical pages */
    /* For now, we'll just simulate success */
    
    /* Update statistics */
    mm_stats.used_physical -= size;
    mm_stats.free_physical += size;
    
    return true;
}

/**
 * @brief Allocate virtual memory
 */
HalVirtualAddr mm_alloc_virtual(uint64_t size, MemoryType type, uint32_t flags) {
    if (!mm_initialized) {
        return NULL;
    }
    
    /* Create a new memory region */
    MemoryRegion* region = (MemoryRegion*)malloc(sizeof(MemoryRegion));
    if (!region) {
        return NULL;
    }
    
    /* In a real implementation, this would allocate actual memory */
    /* For simulation, we'll just allocate a dummy buffer */
    void* memory = malloc(size);
    if (!memory) {
        free(region);
        return NULL;
    }
    
    /* Initialize the region */
    region->start = memory;
    region->size = size;
    region->type = type;
    region->flags = flags;
    region->physical = 0; /* No physical mapping yet */
    region->entanglement_id = 0; /* Not entangled */
    region->next = NULL;
    region->prev = NULL;
    
    /* Set resonance level based on memory type */
    switch (type) {
        case MEMORY_TYPE_QUANTUM:
            region->resonance_level = NODE_QUANTUM_GUARDIAN;
            break;
        case MEMORY_TYPE_ENTANGLED:
            region->resonance_level = NODE_MATRIX_ARCHITECT;
            break;
        default:
            region->resonance_level = NODE_ZERO_POINT;
            break;
    }
    
    /* Add to region list */
    add_region(region);
    
    /* Update statistics */
    update_stats_after_alloc(size, type);
    
    return region->start;
}

/**
 * @brief Free virtual memory
 */
bool mm_free_virtual(HalVirtualAddr addr) {
    if (!mm_initialized) {
        return false;
    }
    
    /* Find the region */
    MemoryRegion* region = find_region(addr);
    if (!region) {
        printf("Attempt to free invalid memory address\n");
        return false;
    }
    
    /* Check if the region is entangled */
    if (region->entanglement_id != 0) {
        /* Break entanglement before freeing */
        mm_break_entanglement(region->entanglement_id);
    }
    
    /* Update statistics */
    update_stats_after_free(region);
    
    /* Remove from region list */
    remove_region(region);
    
    /* Free the actual memory */
    free(region->start);
    free(region);
    
    return true;
}

/**
 * @brief Map physical memory to virtual memory
 */
HalVirtualAddr mm_map_physical(HalPhysicalAddr physical, uint64_t size, uint32_t flags) {
    if (!mm_initialized) {
        return NULL;
    }
    
    /* Get HAL operations */
    const HalOperations* hal_ops = hal_get_operations();
    if (!hal_ops || !hal_ops->map_physical_memory) {
        printf("Physical memory mapping not supported by HAL\n");
        return NULL;
    }
    
    /* Map the physical memory */
    HalVirtualAddr virtual_addr = hal_ops->map_physical_memory(physical, size, flags);
    if (!virtual_addr) {
        printf("Failed to map physical memory\n");
        return NULL;
    }
    
    /* Create a memory region for the mapping */
    MemoryRegion* region = (MemoryRegion*)malloc(sizeof(MemoryRegion));
    if (!region) {
        /* Unmap the memory */
        hal_ops->unmap_physical_memory(virtual_addr, size);
        return NULL;
    }
    
    /* Initialize the region */
    region->start = virtual_addr;
    region->size = size;
    region->type = MEMORY_TYPE_RAM;
    region->flags = flags;
    region->physical = physical;
    region->entanglement_id = 0; /* Not entangled */
    region->next = NULL;
    region->prev = NULL;
    region->resonance_level = NODE_ZERO_POINT;
    
    /* Add to region list */
    add_region(region);
    
    /* Update statistics */
    update_stats_after_alloc(size, MEMORY_TYPE_RAM);
    
    return virtual_addr;
}

/**
 * @brief Get information about a memory region
 */
MemoryRegion* mm_get_region_info(HalVirtualAddr addr) {
    if (!mm_initialized) {
        return NULL;
    }
    
    return find_region(addr);
}

/**
 * @brief Create quantum entanglement between two memory regions
 */
uint64_t mm_create_entanglement(HalVirtualAddr first_addr, HalVirtualAddr second_addr, NodeLevel resonance_level) {
    if (!mm_initialized) {
        return 0;
    }
    
    /* Get HAL operations */
    const HalOperations* hal_ops = hal_get_operations();
    if (!hal_ops || !hal_ops->has_quantum_support || !hal_ops->has_quantum_support()) {
        printf("Quantum operations not supported by hardware\n");
        return 0;
    }
    
    /* Find the memory regions */
    MemoryRegion* first_region = find_region(first_addr);
    MemoryRegion* second_region = find_region(second_addr);
    
    if (!first_region || !second_region) {
        printf("Invalid memory regions for entanglement\n");
        return 0;
    }
    
    /* Check if regions are already entangled */
    if (first_region->entanglement_id != 0 || second_region->entanglement_id != 0) {
        printf("Memory regions are already entangled\n");
        return 0;
    }
    
    /* Find a free entanglement slot */
    EntanglementInfo* slot = find_free_entanglement_slot();
    if (!slot) {
        printf("Maximum number of entanglements reached\n");
        return 0;
    }
    
    /* Set up the entanglement */
    uint64_t entanglement_id = mm_next_entanglement_id++;
    slot->id = entanglement_id;
    slot->first_region = first_region;
    slot->second_region = second_region;
    slot->resonance_level = resonance_level;
    slot->stability = 0.95; /* Initial stability */
    slot->is_synchronized = true;
    
    /* Update the regions */
    first_region->entanglement_id = entanglement_id;
    first_region->type = MEMORY_TYPE_ENTANGLED;
    first_region->resonance_level = resonance_level;
    
    second_region->entanglement_id = entanglement_id;
    second_region->type = MEMORY_TYPE_ENTANGLED;
    second_region->resonance_level = resonance_level;
    
    /* Update statistics */
    mm_stats.total_entanglements++;
    
    /* In a real implementation, this would perform actual quantum entanglement */
    /* For simulation, we'll create a simple memory copy to simulate synchronization */
    memcpy(second_region->start, first_region->start, 
           (first_region->size < second_region->size) ? first_region->size : second_region->size);
    
    printf("Created quantum entanglement (ID: %llu) between memory regions at %p and %p\n",
           (unsigned long long)entanglement_id, first_region->start, second_region->start);
    
    return entanglement_id;
}

/**
 * @brief Break quantum entanglement
 */
bool mm_break_entanglement(uint64_t entanglement_id) {
    if (!mm_initialized) {
        return false;
    }
    
    /* Find the entanglement */
    EntanglementInfo* entanglement = find_entanglement(entanglement_id);
    if (!entanglement) {
        printf("Invalid entanglement ID\n");
        return false;
    }
    
    /* Update the regions */
    if (entanglement->first_region) {
        entanglement->first_region->entanglement_id = 0;
        entanglement->first_region->type = MEMORY_TYPE_RAM;
        entanglement->first_region->resonance_level = NODE_ZERO_POINT;
    }
    
    if (entanglement->second_region) {
        entanglement->second_region->entanglement_id = 0;
        entanglement->second_region->type = MEMORY_TYPE_RAM;
        entanglement->second_region->resonance_level = NODE_ZERO_POINT;
    }
    
    /* Clear the entanglement slot */
    memset(entanglement, 0, sizeof(EntanglementInfo));
    
    /* Update statistics */
    mm_stats.total_entanglements--;
    
    printf("Broke quantum entanglement (ID: %llu)\n", (unsigned long long)entanglement_id);
    
    return true;
}

/**
 * @brief Get information about an entanglement
 */
bool mm_get_entanglement_info(uint64_t entanglement_id, EntanglementInfo* info) {
    if (!mm_initialized || !info) {
        return false;
    }
    
    /* Find the entanglement */
    EntanglementInfo* entanglement = find_entanglement(entanglement_id);
    if (!entanglement) {
        return false;
    }
    
    /* Copy the information */
    memcpy(info, entanglement, sizeof(EntanglementInfo));
    
    return true;
}

/**
 * @brief Synchronize quantum-entangled memory regions
 */
bool mm_sync_entanglement(uint64_t entanglement_id) {
    if (!mm_initialized) {
        return false;
    }
    
    /* Find the entanglement */
    EntanglementInfo* entanglement = find_entanglement(entanglement_id);
    if (!entanglement) {
        printf("Invalid entanglement ID\n");
        return false;
    }
    
    /* Check if the regions are valid */
    if (!entanglement->first_region || !entanglement->second_region) {
        printf("Invalid entangled regions\n");
        return false;
    }
    
    /* In a real implementation, this would perform proper quantum synchronization */
    /* For simulation, we'll create a simple memory copy to simulate synchronization */
    
    /* Determine which region is the source based on resonance level */
    if (entanglement->first_region->resonance_level >= entanglement->second_region->resonance_level) {
        /* First region is source */
        memcpy(entanglement->second_region->start, entanglement->first_region->start, 
               (entanglement->first_region->size < entanglement->second_region->size) ? 
               entanglement->first_region->size : entanglement->second_region->size);
    } else {
        /* Second region is source */
        memcpy(entanglement->first_region->start, entanglement->second_region->start, 
               (entanglement->first_region->size < entanglement->second_region->size) ? 
               entanglement->first_region->size : entanglement->second_region->size);
    }
    
    /* Mark as synchronized */
    entanglement->is_synchronized = true;
    
    /* Reduce stability slightly due to synchronization stress */
    entanglement->stability *= 0.99;
    if (entanglement->stability < 0.5) {
        printf("Warning: Entanglement stability is low (%.2f)\n", entanglement->stability);
    }
    
    return true;
}

/**
 * @brief Get memory statistics
 */
void mm_get_stats(MemoryStats* stats) {
    if (!mm_initialized || !stats) {
        return;
    }
    
    /* Copy the statistics */
    memcpy(stats, &mm_stats, sizeof(MemoryStats));
}

/**
 * @brief Print memory statistics
 */
void mm_print_stats(void) {
    if (!mm_initialized) {
        return;
    }
    
    printf("\nMemory Manager Statistics:\n");
    printf("Total Physical Memory: %llu bytes\n", (unsigned long long)mm_stats.total_physical);
    printf("Used Physical Memory: %llu bytes (%.2f%%)\n", 
           (unsigned long long)mm_stats.used_physical,
           (double)mm_stats.used_physical * 100.0 / mm_stats.total_physical);
    printf("Free Physical Memory: %llu bytes (%.2f%%)\n", 
           (unsigned long long)mm_stats.free_physical,
           (double)mm_stats.free_physical * 100.0 / mm_stats.total_physical);
    
    printf("Total Memory Regions: %u\n", mm_stats.total_regions);
    printf("Total Entanglements: %u\n", mm_stats.total_entanglements);
    
    if (mm_stats.total_quantum > 0) {
        printf("Total Quantum Memory: %llu qubits\n", (unsigned long long)mm_stats.total_quantum);
        printf("Used Quantum Memory: %llu qubits (%.2f%%)\n", 
               (unsigned long long)mm_stats.used_quantum,
               (double)mm_stats.used_quantum * 100.0 / mm_stats.total_quantum);
    }
}