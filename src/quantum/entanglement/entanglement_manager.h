/**
 * @file entanglement_manager.h
 * @brief Quantum Entanglement Manager interface
 *
 * This file defines the interface for the Quantum Entanglement Manager,
 * which is responsible for creating and managing entangled states
 * across devices and processes in CTRLxT OS.
 */

#ifndef CTRLXT_ENTANGLEMENT_MANAGER_H
#define CTRLXT_ENTANGLEMENT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Entanglement types supported by the manager
 */
typedef enum {
    ENTANGLE_PROCESS,  /**< Process-to-process entanglement */
    ENTANGLE_DEVICE,   /**< Device-to-device entanglement */
    ENTANGLE_MEMORY,   /**< Memory-to-memory entanglement */
    ENTANGLE_FILE      /**< File-to-file entanglement */
} EntanglementType;

/**
 * @brief Entanglement identifier
 */
typedef struct {
    uint64_t id;               /**< Unique identifier */
    EntanglementType type;     /**< Type of entanglement */
    uint64_t source_id;        /**< Source entity ID */
    uint64_t target_id;        /**< Target entity ID */
    uint32_t qubit_count;      /**< Number of qubits involved */
    bool is_active;            /**< Whether entanglement is active */
} EntanglementId;

/**
 * @brief Initialize the Quantum Entanglement Manager
 * 
 * @param max_entanglements Maximum number of simultaneous entanglements
 * @return true if initialization succeeded, false otherwise
 */
bool qem_init(uint32_t max_entanglements);

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
                                     uint64_t target_id, uint32_t qubit_count);

/**
 * @brief Destroy an existing entanglement
 * 
 * @param entanglement_id ID of the entanglement to destroy
 * @return true if destruction succeeded, false otherwise
 */
bool qem_destroy_entanglement(uint64_t entanglement_id);

/**
 * @brief Synchronize state across an entanglement
 * 
 * @param entanglement_id ID of the entanglement to synchronize
 * @return true if synchronization succeeded, false otherwise
 */
bool qem_sync_entanglement(uint64_t entanglement_id);

/**
 * @brief Get information about an entanglement
 * 
 * @param entanglement_id ID of the entanglement
 * @return EntanglementId structure with the entanglement's information
 */
EntanglementId qem_get_entanglement_info(uint64_t entanglement_id);

/**
 * @brief Shutdown the Quantum Entanglement Manager
 */
void qem_shutdown(void);

#endif /* CTRLXT_ENTANGLEMENT_MANAGER_H */