/**
 * @file knowledge_network.c
 * @brief Implementation of Memex Knowledge Networking integration
 *
 * This file implements the Memex Knowledge Networking integration with CTRLxT OS,
 * combined with quantum entanglement principles.
 */

#include "knowledge_network.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/**
 * @brief Internal knowledge node structure
 */
typedef struct {
    KnowledgeNode public_data;        /**< Public node data */
    bool is_active;                    /**< Whether this node is active */
    void *private_data;                /**< Private implementation data */
    uint64_t create_time;              /**< Creation timestamp */
    uint64_t update_time;              /**< Last update timestamp */
    uint32_t access_count;             /**< Access counter */
} KnowledgeNodeInternal;

/**
 * @brief Internal knowledge relation structure
 */
typedef struct {
    KnowledgeRelation public_data;     /**< Public relation data */
    bool is_active;                    /**< Whether this relation is active */
    uint64_t create_time;              /**< Creation timestamp */
    uint64_t update_time;              /**< Last update timestamp */
    uint32_t traverse_count;           /**< Traversal counter */
} KnowledgeRelationInternal;

/* Static variables */
static KnowledgeNodeInternal *node_registry = NULL;
static KnowledgeRelationInternal *relation_registry = NULL;
static uint32_t max_nodes = 1000;          // Default max nodes
static uint32_t active_nodes = 0;
static uint32_t max_relations = 5000;      // Default max relations
static uint32_t active_relations = 0;
static uint64_t next_node_id = 1;
static uint64_t next_relation_id = 1;
static bool use_quantum_by_default = false;
static bool is_initialized = false;

/**
 * @brief Get available slot in node registry
 * 
 * @return Index of available slot, or -1 if none available
 */
static int32_t get_available_node_slot() {
    if (!is_initialized || node_registry == NULL) {
        return -1;
    }
    
    for (uint32_t i = 0; i < max_nodes; i++) {
        if (!node_registry[i].is_active) {
            return i;
        }
    }
    
    return -1; // No slots available
}

/**
 * @brief Get available slot in relation registry
 * 
 * @return Index of available slot, or -1 if none available
 */
static int32_t get_available_relation_slot() {
    if (!is_initialized || relation_registry == NULL) {
        return -1;
    }
    
    for (uint32_t i = 0; i < max_relations; i++) {
        if (!relation_registry[i].is_active) {
            return i;
        }
    }
    
    return -1; // No slots available
}

/**
 * @brief Find a node in the registry by ID
 * 
 * @param node_id Node ID to find
 * @return Index in registry, or -1 if not found
 */
static int32_t find_node(uint64_t node_id) {
    if (!is_initialized || node_registry == NULL) {
        return -1;
    }
    
    for (uint32_t i = 0; i < max_nodes; i++) {
        if (node_registry[i].is_active && 
            node_registry[i].public_data.id == node_id) {
            return i;
        }
    }
    
    return -1; // Not found
}

/**
 * @brief Find a relation in the registry by ID
 * 
 * @param relation_id Relation ID to find
 * @return Index in registry, or -1 if not found
 */
static int32_t find_relation(uint64_t relation_id) {
    if (!is_initialized || relation_registry == NULL) {
        return -1;
    }
    
    for (uint32_t i = 0; i < max_relations; i++) {
        if (relation_registry[i].is_active && 
            relation_registry[i].public_data.id == relation_id) {
            return i;
        }
    }
    
    return -1; // Not found
}

/**
 * @brief Check if a relation exists between two nodes
 * 
 * @param source_id Source node ID
 * @param target_id Target node ID
 * @param relation_type Relation type (-1 for any type)
 * @return Relation ID if exists, 0 otherwise
 */
static uint64_t relation_exists(uint64_t source_id, uint64_t target_id, int relation_type) {
    if (!is_initialized || relation_registry == NULL) {
        return 0;
    }
    
    for (uint32_t i = 0; i < max_relations; i++) {
        if (relation_registry[i].is_active && 
            relation_registry[i].public_data.source_node_id == source_id &&
            relation_registry[i].public_data.target_node_id == target_id) {
            
            // Check relation type if specified
            if (relation_type < 0 || 
                relation_registry[i].public_data.type == (KnowledgeRelationType)relation_type) {
                return relation_registry[i].public_data.id;
            }
        }
    }
    
    return 0; // Not found
}

/**
 * @brief Initialize the Memex Knowledge Network integration
 * 
 * @param use_quantum Whether to use quantum entanglement
 * @return true if initialization succeeded, false otherwise
 */
bool memex_knowledge_init(bool use_quantum) {
    if (is_initialized) {
        return false; // Already initialized
    }
    
    // Allocate node registry
    node_registry = (KnowledgeNodeInternal*)calloc(max_nodes, sizeof(KnowledgeNodeInternal));
    if (node_registry == NULL) {
        return false; // Memory allocation failed
    }
    
    // Allocate relation registry
    relation_registry = (KnowledgeRelationInternal*)calloc(max_relations, sizeof(KnowledgeRelationInternal));
    if (relation_registry == NULL) {
        // Free node registry if relation allocation fails
        free(node_registry);
        node_registry = NULL;
        return false;
    }
    
    // Initialize registries
    for (uint32_t i = 0; i < max_nodes; i++) {
        node_registry[i].is_active = false;
    }
    
    for (uint32_t i = 0; i < max_relations; i++) {
        relation_registry[i].is_active = false;
    }
    
    // Store settings
    use_quantum_by_default = use_quantum;
    
    // Mark as initialized
    is_initialized = true;
    active_nodes = 0;
    active_relations = 0;
    next_node_id = 1;
    next_relation_id = 1;
    
    return true;
}

/**
 * @brief Create a new knowledge node
 * 
 * @param type Type of node
 * @param name Node name
 * @param description Node description
 * @param use_quantum Whether to create quantum entanglement
 * @return New KnowledgeNode structure
 */
KnowledgeNode memex_knowledge_create_node(KnowledgeNodeType type, 
                                        const char *name,
                                        const char *description,
                                        bool use_quantum) {
    KnowledgeNode empty_node = {0};
    
    // Check initialization
    if (!is_initialized) {
        return empty_node;
    }
    
    // Check parameters
    if (name == NULL) {
        return empty_node;
    }
    
    // Get available slot
    int32_t slot = get_available_node_slot();
    if (slot < 0) {
        return empty_node; // No slots available
    }
    
    // Initialize node
    KnowledgeNodeInternal *node = &node_registry[slot];
    
    // Set basic properties
    node->public_data.id = next_node_id++;
    node->public_data.type = type;
    
    // Copy name
    node->public_data.name = strdup(name);
    if (node->public_data.name == NULL) {
        return empty_node; // Memory allocation failed
    }
    
    // Copy description if provided
    if (description != NULL) {
        node->public_data.description = strdup(description);
        if (node->public_data.description == NULL) {
            // Free name if description allocation fails
            free(node->public_data.name);
            return empty_node;
        }
    } else {
        node->public_data.description = NULL;
    }
    
    // Initialize related nodes array (empty)
    node->public_data.related_nodes = NULL;
    node->public_data.related_node_count = 0;
    
    // Set up quantum entanglement if requested
    if (use_quantum) {
        // Create a node entanglement
        EntanglementId entanglement = qem_create_entanglement(
            ENTANGLE_MEMORY,
            (uint64_t)&node->public_data,
            0,  // No target yet
            4    // Use 4 qubits
        );
        
        // Store entanglement if successful
        if (entanglement.is_active) {
            node->public_data.entanglement = (EntanglementId*)malloc(sizeof(EntanglementId));
            if (node->public_data.entanglement != NULL) {
                *node->public_data.entanglement = entanglement;
            }
        } else {
            node->public_data.entanglement = NULL;
        }
    } else {
        node->public_data.entanglement = NULL;
    }
    
    // Set tracking data
    node->is_active = true;
    node->private_data = NULL; // No private data yet
    node->create_time = (uint64_t)time(NULL);
    node->update_time = node->create_time;
    node->access_count = 0;
    
    // Increment active nodes count
    active_nodes++;
    
    return node->public_data;
}

/**
 * @brief Create a relation between two knowledge nodes
 * 
 * @param type Type of relation
 * @param source_node_id Source node ID
 * @param target_node_id Target node ID
 * @param strength Relation strength (0.0 to 1.0)
 * @param use_quantum Whether to create quantum entanglement
 * @return New KnowledgeRelation structure
 */
KnowledgeRelation memex_knowledge_create_relation(KnowledgeRelationType type,
                                                uint64_t source_node_id,
                                                uint64_t target_node_id,
                                                float strength,
                                                bool use_quantum) {
    KnowledgeRelation empty_relation = {0};
    
    // Check initialization
    if (!is_initialized) {
        return empty_relation;
    }
    
    // Validate parameters
    if (source_node_id == 0 || target_node_id == 0 || 
        source_node_id == target_node_id) {
        return empty_relation;
    }
    
    // Ensure both nodes exist
    int32_t source_slot = find_node(source_node_id);
    int32_t target_slot = find_node(target_node_id);
    if (source_slot < 0 || target_slot < 0) {
        return empty_relation; // One or both nodes don't exist
    }
    
    // Check if relation already exists
    if (relation_exists(source_node_id, target_node_id, type)) {
        return empty_relation; // Relation already exists
    }
    
    // Get available slot
    int32_t slot = get_available_relation_slot();
    if (slot < 0) {
        return empty_relation; // No slots available
    }
    
    // Initialize relation
    KnowledgeRelationInternal *relation = &relation_registry[slot];
    
    // Set basic properties
    relation->public_data.id = next_relation_id++;
    relation->public_data.type = type;
    relation->public_data.source_node_id = source_node_id;
    relation->public_data.target_node_id = target_node_id;
    
    // Clamp strength to valid range
    if (strength < 0.0f) strength = 0.0f;
    if (strength > 1.0f) strength = 1.0f;
    relation->public_data.strength = strength;
    
    // Set up quantum entanglement if requested
    if (use_quantum) {
        // Create a relation entanglement
        EntanglementId entanglement = qem_create_entanglement(
            ENTANGLE_MEMORY,
            (uint64_t)&relation->public_data,
            0,  // No target yet
            2    // Use 2 qubits
        );
        
        // Store entanglement if successful
        if (entanglement.is_active) {
            relation->public_data.entanglement = (EntanglementId*)malloc(sizeof(EntanglementId));
            if (relation->public_data.entanglement != NULL) {
                *relation->public_data.entanglement = entanglement;
            }
        } else {
            relation->public_data.entanglement = NULL;
        }
    } else {
        relation->public_data.entanglement = NULL;
    }
    
    // Set tracking data
    relation->is_active = true;
    relation->create_time = (uint64_t)time(NULL);
    relation->update_time = relation->create_time;
    relation->traverse_count = 0;
    
    // Update the related_nodes arrays in both nodes
    KnowledgeNodeInternal *source_node = &node_registry[source_slot];
    KnowledgeNodeInternal *target_node = &node_registry[target_slot];
    
    // Update source node's related_nodes array
    uint32_t new_count = source_node->public_data.related_node_count + 1;
    uint64_t *new_related = (uint64_t*)realloc(
        source_node->public_data.related_nodes,
        new_count * sizeof(uint64_t));
        
    if (new_related != NULL) {
        new_related[source_node->public_data.related_node_count] = target_node_id;
        source_node->public_data.related_nodes = new_related;
        source_node->public_data.related_node_count = new_count;
        source_node->update_time = relation->create_time;
    }
    
    // Update target node's related_nodes array (for bidirectional access)
    new_count = target_node->public_data.related_node_count + 1;
    new_related = (uint64_t*)realloc(
        target_node->public_data.related_nodes,
        new_count * sizeof(uint64_t));
        
    if (new_related != NULL) {
        new_related[target_node->public_data.related_node_count] = source_node_id;
        target_node->public_data.related_nodes = new_related;
        target_node->public_data.related_node_count = new_count;
        target_node->update_time = relation->create_time;
    }
    
    // Increment active relations count
    active_relations++;
    
    return relation->public_data;
}

/**
 * @brief Find knowledge nodes matching a query
 * 
 * @param query Query string
 * @param max_results Maximum number of results
 * @param result_count Pointer to store number of results
 * @return Array of KnowledgeNode structures (must be freed by caller)
 */
KnowledgeNode *memex_knowledge_find_nodes(const char *query,
                                        uint32_t max_results,
                                        uint32_t *result_count) {
    // Initialize result count
    if (result_count != NULL) {
        *result_count = 0;
    }
    
    // Check initialization and parameters
    if (!is_initialized || query == NULL || max_results == 0 || result_count == NULL) {
        return NULL;
    }
    
    // Allocate array for results (we'll realloc if needed)
    KnowledgeNode *results = (KnowledgeNode*)malloc(max_results * sizeof(KnowledgeNode));
    if (results == NULL) {
        return NULL; // Memory allocation failed
    }
    
    uint32_t found_count = 0;
    
    // Simple substring search in node names and descriptions
    for (uint32_t i = 0; i < max_nodes && found_count < max_results; i++) {
        if (node_registry[i].is_active) {
            bool match = false;
            
            // Check name
            if (node_registry[i].public_data.name != NULL && 
                strstr(node_registry[i].public_data.name, query) != NULL) {
                match = true;
            }
            
            // Check description
            if (!match && node_registry[i].public_data.description != NULL && 
                strstr(node_registry[i].public_data.description, query) != NULL) {
                match = true;
            }
            
            if (match) {
                // Copy node data to results
                results[found_count] = node_registry[i].public_data;
                
                // Update node access count
                node_registry[i].access_count++;
                
                found_count++;
            }
        }
    }
    
    // Set result count
    *result_count = found_count;
    
    // If no results found, free the array and return NULL
    if (found_count == 0) {
        free(results);
        return NULL;
    }
    
    // Resize the array to the actual count
    if (found_count < max_results) {
        KnowledgeNode *resized = (KnowledgeNode*)realloc(results, found_count * sizeof(KnowledgeNode));
        if (resized != NULL) {
            results = resized;
        }
    }
    
    return results;
}

/**
 * @brief Get related knowledge nodes
 * 
 * @param node_id Node ID to find relations for
 * @param relation_type Type of relation (or -1 for all)
 * @param max_results Maximum number of results
 * @param result_count Pointer to store number of results
 * @return Array of KnowledgeNode structures (must be freed by caller)
 */
KnowledgeNode *memex_knowledge_get_related(uint64_t node_id,
                                         int relation_type,
                                         uint32_t max_results,
                                         uint32_t *result_count) {
    // Initialize result count
    if (result_count != NULL) {
        *result_count = 0;
    }
    
    // Check initialization and parameters
    if (!is_initialized || node_id == 0 || max_results == 0 || result_count == NULL) {
        return NULL;
    }
    
    // Find the source node
    int32_t node_slot = find_node(node_id);
    if (node_slot < 0) {
        return NULL; // Node not found
    }
    
    // Get the node
    KnowledgeNodeInternal *node = &node_registry[node_slot];
    
    // If node has no relations, return NULL
    if (node->public_data.related_node_count == 0) {
        return NULL;
    }
    
    // Update access count
    node->access_count++;
    
    // Allocate array for results
    KnowledgeNode *results = (KnowledgeNode*)malloc(max_results * sizeof(KnowledgeNode));
    if (results == NULL) {
        return NULL; // Memory allocation failed
    }
    
    uint32_t found_count = 0;
    
    // Iterate through the node's related nodes
    for (uint32_t i = 0; i < node->public_data.related_node_count && found_count < max_results; i++) {
        uint64_t related_id = node->public_data.related_nodes[i];
        
        // Check if there's a relation of the requested type
        bool include = (relation_type < 0); // Include all types if relation_type is -1
        
        if (!include) {
            // Check if relation of specified type exists
            uint64_t relation_id = relation_exists(node_id, related_id, relation_type);
            include = (relation_id != 0);
            
            // If not found in one direction, try the other direction
            if (!include) {
                relation_id = relation_exists(related_id, node_id, relation_type);
                include = (relation_id != 0);
            }
        }
        
        if (include) {
            // Find the related node
            int32_t related_slot = find_node(related_id);
            if (related_slot >= 0) {
                // Copy node data to results
                results[found_count] = node_registry[related_slot].public_data;
                
                // Update node access count
                node_registry[related_slot].access_count++;
                
                found_count++;
            }
        }
    }
    
    // Set result count
    *result_count = found_count;
    
    // If no results found, free the array and return NULL
    if (found_count == 0) {
        free(results);
        return NULL;
    }
    
    // Resize the array to the actual count
    if (found_count < max_results) {
        KnowledgeNode *resized = (KnowledgeNode*)realloc(results, found_count * sizeof(KnowledgeNode));
        if (resized != NULL) {
            results = resized;
        }
    }
    
    return results;
}

/**
 * @brief Shutdown the Memex Knowledge Network integration
 */
void memex_knowledge_shutdown(void) {
    if (!is_initialized) {
        return;
    }
    
    // Clean up nodes
    for (uint32_t i = 0; i < max_nodes; i++) {
        if (node_registry[i].is_active) {
            // Free name
            if (node_registry[i].public_data.name != NULL) {
                free(node_registry[i].public_data.name);
            }
            
            // Free description
            if (node_registry[i].public_data.description != NULL) {
                free(node_registry[i].public_data.description);
            }
            
            // Free related nodes array
            if (node_registry[i].public_data.related_nodes != NULL) {
                free(node_registry[i].public_data.related_nodes);
            }
            
            // Destroy entanglement
            if (node_registry[i].public_data.entanglement != NULL) {
                if (node_registry[i].public_data.entanglement->is_active) {
                    qem_destroy_entanglement(node_registry[i].public_data.entanglement->id);
                }
                free(node_registry[i].public_data.entanglement);
            }
            
            // Free private data
            if (node_registry[i].private_data != NULL) {
                free(node_registry[i].private_data);
            }
        }
    }
    
    // Clean up relations
    for (uint32_t i = 0; i < max_relations; i++) {
        if (relation_registry[i].is_active) {
            // Destroy entanglement
            if (relation_registry[i].public_data.entanglement != NULL) {
                if (relation_registry[i].public_data.entanglement->is_active) {
                    qem_destroy_entanglement(relation_registry[i].public_data.entanglement->id);
                }
                free(relation_registry[i].public_data.entanglement);
            }
        }
    }
    
    // Free registries
    free(node_registry);
    free(relation_registry);
    
    // Reset state
    node_registry = NULL;
    relation_registry = NULL;
    max_nodes = 1000;
    active_nodes = 0;
    max_relations = 5000;
    active_relations = 0;
    next_node_id = 1;
    next_relation_id = 1;
    use_quantum_by_default = false;
    is_initialized = false;
}