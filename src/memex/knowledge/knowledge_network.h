/**
 * @file knowledge_network.h
 * @brief Memex Knowledge Networking integration interface
 *
 * This file defines the interface for integrating Memex's knowledge networking
 * capabilities with CTRLxT OS, combined with quantum entanglement principles.
 */

#ifndef CTRLXT_MEMEX_KNOWLEDGE_H
#define CTRLXT_MEMEX_KNOWLEDGE_H

#include <stdint.h>
#include <stdbool.h>
#include "../../quantum/entanglement/entanglement_manager.h"

/**
 * @brief Knowledge node types
 */
typedef enum {
    NODE_CONCEPT,       /**< Conceptual knowledge node */
    NODE_ENTITY,        /**< Entity knowledge node */
    NODE_EVENT,         /**< Event knowledge node */
    NODE_RESOURCE,      /**< Resource knowledge node */
    NODE_QUANTUM        /**< Quantum-entangled knowledge node */
} KnowledgeNodeType;

/**
 * @brief Knowledge relation types
 */
typedef enum {
    RELATION_IS_A,          /**< Hierarchical relation */
    RELATION_PART_OF,       /**< Compositional relation */
    RELATION_RELATED_TO,    /**< General relation */
    RELATION_CAUSES,        /**< Causal relation */
    RELATION_ENTANGLED      /**< Quantum entanglement relation */
} KnowledgeRelationType;

/**
 * @brief Knowledge node structure
 */
typedef struct {
    uint64_t id;                  /**< Unique identifier */
    KnowledgeNodeType type;       /**< Type of node */
    char *name;                   /**< Node name */
    char *description;            /**< Node description */
    uint64_t *related_nodes;      /**< Array of related node IDs */
    uint32_t related_node_count;  /**< Number of related nodes */
    EntanglementId *entanglement; /**< Quantum entanglement (if applicable) */
} KnowledgeNode;

/**
 * @brief Knowledge relation structure
 */
typedef struct {
    uint64_t id;                     /**< Unique identifier */
    KnowledgeRelationType type;      /**< Type of relation */
    uint64_t source_node_id;         /**< Source node ID */
    uint64_t target_node_id;         /**< Target node ID */
    float strength;                  /**< Relation strength (0.0 to 1.0) */
    EntanglementId *entanglement;    /**< Quantum entanglement (if applicable) */
} KnowledgeRelation;

/**
 * @brief Initialize the Memex Knowledge Network integration
 * 
 * @param use_quantum Whether to use quantum entanglement
 * @return true if initialization succeeded, false otherwise
 */
bool memex_knowledge_init(bool use_quantum);

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
                                        bool use_quantum);

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
                                                bool use_quantum);

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
                                        uint32_t *result_count);

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
                                         uint32_t *result_count);

/**
 * @brief Shutdown the Memex Knowledge Network integration
 */
void memex_knowledge_shutdown(void);

#endif /* CTRLXT_MEMEX_KNOWLEDGE_H */