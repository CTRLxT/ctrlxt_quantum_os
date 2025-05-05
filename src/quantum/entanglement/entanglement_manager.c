/**
 * @file entanglement_manager.c
 * @brief Implementation of the Quantum Entanglement Manager
 *
 * This file implements the Quantum Entanglement Manager, which creates and manages
 * entangled states between processes, devices, memory regions, and files in CTRLxT OS.
 */

#include "entanglement_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Structure for storing entanglement information internally
 */
typedef struct {
    EntanglementId id_info;        /**< Public entanglement information */
    void* source_state;             /**< Pointer to source state */
    void* target_state;             /**< Pointer to target state */
    uint32_t state_size;            /**< Size of the state in bytes */
    bool is_valid;                  /**< Whether this slot is valid */
} EntanglementRecord;

/* Static variables */
static EntanglementRecord* entanglement_registry = NULL;
static uint32_t max_entanglements = 0;
static uint32_t current_entanglements = 0;
static uint64_t next_entanglement_id = 1;
static bool is_initialized = false;

/**
 * @brief Initialize the Quantum Entanglement Manager
 * 
 * @param max_entanglements Maximum number of simultaneous entanglements
 * @return true if initialization succeeded, false otherwise
 */
bool qem_init(uint32_t max_entanglements_count) {
    if (is_initialized) {
        return false; // Already initialized
    }
    
    if (max_entanglements_count == 0) {
        return false; // Invalid parameter
    }
    
    entanglement_registry = (EntanglementRecord*)calloc(max_entanglements_count, sizeof(EntanglementRecord));
    if (entanglement_registry == NULL) {
        return false; // Memory allocation failed
    }
    
    max_entanglements = max_entanglements_count;
    current_entanglements = 0;
    next_entanglement_id = 1;
    is_initialized = true;
    
    return true;
}

/**
 * @brief Get an available slot in the entanglement registry
 * 
 * @return Index of an available slot, or -1 if no slots are available
 */
static int32_t get_available_slot() {
    if (!is_initialized) {
        return -1;
    }
    
    for (uint32_t i = 0; i < max_entanglements; i++) {
        if (!entanglement_registry[i].is_valid) {
            return i;
        }
    }
    
    return -1; // No slots available
}

/**
 * @brief Find an entanglement in the registry by ID
 * 
 * @param entanglement_id The ID to search for
 * @return Index in the registry, or -1 if not found
 */
static int32_t find_entanglement(uint64_t entanglement_id) {
    if (!is_initialized) {
        return -1;
    }
    
    for (uint32_t i = 0; i < max_entanglements; i++) {
        if (entanglement_registry[i].is_valid && 
            entanglement_registry[i].id_info.id == entanglement_id) {
            return i;
        }
    }
    
    return -1; // Not found
}

/**
 * @brief Create a new entanglement between two entities
 * 
 * @param type Type of entanglement
 * @param source_id Source entity ID
 * @param target_id Target entity ID
 * @param qubit_count Number of qubits to use
 * @return EntanglementId structure with the new entanglement's information
 */
EntanglementId qem_create_entanglement(EntanglementType type, uint64_t source_id, 
                                     uint64_t target_id, uint32_t qubit_count) {
    EntanglementId invalid_result = {0};
    invalid_result.is_active = false;
    
    if (!is_initialized) {
        return invalid_result;
    }
    
    // Basic parameter validation
    if (source_id == 0 || target_id == 0 || qubit_count == 0) {
        return invalid_result;
    }
    
    // Check for available slots
    int32_t slot = get_available_slot();
    if (slot < 0) {
        return invalid_result;
    }
    
    // Calculate state size based on qubit count
    uint32_t state_size = (1 << qubit_count) * sizeof(double);
    
    // Allocate memory for quantum states
    void* source_state = malloc(state_size);
    void* target_state = malloc(state_size);
    
    if (source_state == NULL || target_state == NULL) {
        // Clean up on failure
        if (source_state) free(source_state);
        if (target_state) free(target_state);
        return invalid_result;
    }
    
    // Initialize quantum states with an entangled pattern
    // For simplicity, we're setting up a basic state: |00⟩ + |11⟩ pattern
    memset(source_state, 0, state_size);
    memset(target_state, 0, state_size);
    
    // Create actual entanglement record
    EntanglementRecord new_record;
    new_record.id_info.id = next_entanglement_id++;
    new_record.id_info.type = type;
    new_record.id_info.source_id = source_id;
    new_record.id_info.target_id = target_id;
    new_record.id_info.qubit_count = qubit_count;
    new_record.id_info.is_active = true;
    new_record.source_state = source_state;
    new_record.target_state = target_state;
    new_record.state_size = state_size;
    new_record.is_valid = true;
    
    // Store the record
    entanglement_registry[slot] = new_record;
    current_entanglements++;
    
    return new_record.id_info;
}

/**
 * @brief Destroy an existing entanglement
 * 
 * @param entanglement_id ID of the entanglement to destroy
 * @return true if destruction succeeded, false otherwise
 */
bool qem_destroy_entanglement(uint64_t entanglement_id) {
    if (!is_initialized) {
        return false;
    }
    
    int32_t slot = find_entanglement(entanglement_id);
    if (slot < 0) {
        return false;
    }
    
    // Free allocated memory
    free(entanglement_registry[slot].source_state);
    free(entanglement_registry[slot].target_state);
    
    // Clear record
    entanglement_registry[slot].is_valid = false;
    entanglement_registry[slot].source_state = NULL;
    entanglement_registry[slot].target_state = NULL;
    entanglement_registry[slot].id_info.is_active = false;
    
    current_entanglements--;
    return true;
}

/**
 * @brief Synchronize state across an entanglement
 * 
 * @param entanglement_id ID of the entanglement to synchronize
 * @return true if synchronization succeeded, false otherwise
 */
bool qem_sync_entanglement(uint64_t entanglement_id) {
    if (!is_initialized) {
        return false;
    }
    
    int32_t slot = find_entanglement(entanglement_id);
    if (slot < 0) {
        return false;
    }
    
    // Ensure the entanglement is active
    if (!entanglement_registry[slot].id_info.is_active) {
        return false;
    }
    
    // For a real quantum system, this would involve complex quantum operations
    // Here we simulate the synchronization with a simple memory copy
    memcpy(entanglement_registry[slot].target_state, 
           entanglement_registry[slot].source_state, 
           entanglement_registry[slot].state_size);
    
    return true;
}

/**
 * @brief Get information about an entanglement
 * 
 * @param entanglement_id ID of the entanglement
 * @return EntanglementId structure with the entanglement's information
 */
EntanglementId qem_get_entanglement_info(uint64_t entanglement_id) {
    EntanglementId invalid_result = {0};
    invalid_result.is_active = false;
    
    if (!is_initialized) {
        return invalid_result;
    }
    
    int32_t slot = find_entanglement(entanglement_id);
    if (slot < 0) {
        return invalid_result;
    }
    
    return entanglement_registry[slot].id_info;
}

/**
 * @brief Shutdown the Quantum Entanglement Manager
 */
void qem_shutdown(void) {
    if (!is_initialized) {
        return;
    }
    
    // Clean up all active entanglements
    for (uint32_t i = 0; i < max_entanglements; i++) {
        if (entanglement_registry[i].is_valid) {
            free(entanglement_registry[i].source_state);
            free(entanglement_registry[i].target_state);
            entanglement_registry[i].is_valid = false;
        }
    }
    
    // Free the registry itself
    free(entanglement_registry);
    entanglement_registry = NULL;
    
    max_entanglements = 0;
    current_entanglements = 0;
    next_entanglement_id = 1;
    is_initialized = false;
}