/**
 * @file knowledge_graph.h
 * @brief Knowledge Graph for Memex integration
 *
 * This file defines the Knowledge Graph component of the Memex integration,
 * which manages structured knowledge representation and quantum-entangled
 * knowledge relationships.
 */

#ifndef CTRLXT_MEMEX_KNOWLEDGE_GRAPH_H
#define CTRLXT_MEMEX_KNOWLEDGE_GRAPH_H

#include <stdint.h>
#include <stdbool.h>
#include "../interface/memex_interface.h"
#include "../../quantum/resonance/resonant_frequencies.h"

/**
 * @brief Knowledge node types
 */
typedef enum {
    KG_NODE_ENTITY,      /**< Named entity (person, place, thing) */
    KG_NODE_CONCEPT,      /**< Abstract concept */
    KG_NODE_EVENT,        /**< Event or occurrence */
    KG_NODE_ATTRIBUTE,    /**< Attribute or property */
    KG_NODE_PROCESS,      /**< Process or action */
    KG_NODE_QUANTUM       /**< Quantum state node */
} KnowledgeNodeType;

/**
 * @brief Knowledge graph node structure
 */
typedef struct {
    uint64_t id;               /**< Node ID (maps to MemexDataItem ID) */
    KnowledgeNodeType type;    /**< Node type */
    char *label;               /**< Node label */
    char *description;         /**< Node description */
    char *properties;          /**< JSON string of additional properties */
    NodeLevel resonance_level; /**< Node resonance level */
    uint64_t creation_time;    /**< Creation timestamp */
} KnowledgeNode;

/**
 * @brief Knowledge path structure (sequence of nodes and relations)
 */
typedef struct {
    uint64_t *node_ids;        /**< Array of node IDs in the path */
    uint64_t *relation_ids;    /**< Array of relation IDs in the path */
    uint32_t length;           /**< Path length (number of nodes - 1 = number of relations) */
    float relevance;           /**< Path relevance score */
} KnowledgePath;

/**
 * @brief Initialize the Knowledge Graph component
 * 
 * @param data_directory Data directory for storage
 * @param enable_quantum Whether to enable quantum operations
 * @param max_resonance Maximum resonance level
 * @return true if initialization succeeded, false otherwise
 */
bool kg_init(const char *data_directory, bool enable_quantum, NodeLevel max_resonance);

/**
 * @brief Shutdown the Knowledge Graph component
 */
void kg_shutdown(void);

/**
 * @brief Create a new knowledge node
 * 
 * @param type Node type
 * @param label Node label
 * @param description Node description
 * @param properties JSON string of additional properties
 * @param resonance_level Node resonance level
 * @return New node ID or 0 on failure
 */
uint64_t kg_create_node(KnowledgeNodeType type, const char *label, const char *description,
                       const char *properties, NodeLevel resonance_level);

/**
 * @brief Get a knowledge node by ID
 * 
 * @param node_id Node ID
 * @return Node structure (must be freed with kg_free_node) or NULL if not found
 */
KnowledgeNode *kg_get_node(uint64_t node_id);

/**
 * @brief Update a knowledge node
 * 
 * @param node_id Node ID
 * @param label New label (NULL to keep current)
 * @param description New description (NULL to keep current)
 * @param properties New properties (NULL to keep current)
 * @param resonance_level New resonance level (-1 to keep current)
 * @return true if update succeeded, false otherwise
 */
bool kg_update_node(uint64_t node_id, const char *label, const char *description,
                   const char *properties, int resonance_level);

/**
 * @brief Delete a knowledge node
 * 
 * @param node_id Node ID to delete
 * @param delete_relations Whether to also delete all relations involving this node
 * @return true if deletion succeeded, false otherwise
 */
bool kg_delete_node(uint64_t node_id, bool delete_relations);

/**
 * @brief Free a knowledge node
 * 
 * @param node Node to free
 */
void kg_free_node(KnowledgeNode *node);

/**
 * @brief Create a relation between two nodes
 * 
 * @param source_id Source node ID
 * @param target_id Target node ID
 * @param relation_type Relation type
 * @param bidirectional Whether the relation is bidirectional
 * @param weight Relation weight (0.0 to 1.0)
 * @param metadata Relation metadata (JSON string)
 * @param resonance_level Relation resonance level
 * @return Relation ID or 0 on failure
 */
uint64_t kg_create_relation(uint64_t source_id, uint64_t target_id, MemexRelationType relation_type,
                           bool bidirectional, float weight, const char *metadata,
                           NodeLevel resonance_level);

/**
 * @brief Find the shortest path between two nodes
 * 
 * @param start_node_id Start node ID
 * @param end_node_id End node ID
 * @param max_depth Maximum path depth to search
 * @param min_resonance_level Minimum resonance level for nodes and relations
 * @return Path structure (must be freed with kg_free_path) or NULL if no path found
 */
KnowledgePath *kg_find_path(uint64_t start_node_id, uint64_t end_node_id, 
                          uint32_t max_depth, NodeLevel min_resonance_level);

/**
 * @brief Free a knowledge path
 * 
 * @param path Path to free
 */
void kg_free_path(KnowledgePath *path);

/**
 * @brief Find related nodes based on node ID and relation type
 * 
 * @param node_id Base node ID
 * @param relation_type Relation type (MEMEX_RELATION_UNDEFINED for any)
 * @param max_nodes Maximum number of nodes to return
 * @param count Pointer to store the number of nodes found
 * @return Array of node IDs (must be freed by caller) or NULL if none found
 */
uint64_t *kg_find_related_nodes(uint64_t node_id, MemexRelationType relation_type,
                               uint32_t max_nodes, uint32_t *count);

/**
 * @brief Create a quantum-entangled node pair
 * 
 * @param label1 First node label
 * @param label2 Second node label
 * @param description Description of the entanglement
 * @param resonance_level Resonance level for the entanglement
 * @param node1_id Pointer to store first node ID
 * @param node2_id Pointer to store second node ID
 * @return Relation ID or 0 on failure
 */
uint64_t kg_create_entangled_nodes(const char *label1, const char *label2,
                                  const char *description, NodeLevel resonance_level,
                                  uint64_t *node1_id, uint64_t *node2_id);

/**
 * @brief Convert a knowledge node to a Memex data item
 * 
 * @param node Knowledge node to convert
 * @return Memex data item (must be freed with memex_free_item) or NULL on failure
 */
MemexDataItem *kg_node_to_data_item(const KnowledgeNode *node);

/**
 * @brief Perform semantic reasoning on the knowledge graph
 * 
 * @param start_node_id Starting node ID
 * @param query_text Natural language query
 * @param max_results Maximum number of results
 * @param count Pointer to store the number of results
 * @return Array of node IDs representing inferred knowledge (must be freed by caller) or NULL on failure
 */
uint64_t *kg_semantic_reasoning(uint64_t start_node_id, const char *query_text,
                               uint32_t max_results, uint32_t *count);

#endif /* CTRLXT_MEMEX_KNOWLEDGE_GRAPH_H */