/**
 * @file memex_interface.h
 * @brief Interface for Memex integration with CTRLxT OS
 *
 * This file defines the core interface for integrating Memex technology
 * with CTRLxT OS, providing advanced search, semantic analysis,
 * knowledge networking, and context-aware computing capabilities.
 */

#ifndef CTRLXT_MEMEX_INTERFACE_H
#define CTRLXT_MEMEX_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../../quantum/resonance/resonant_frequencies.h"
#include "../../quantum/messaging/quantum_message_bus.h"

/**
 * @brief Memex search result relevance score
 */
typedef float MemexRelevance; /* 0.0 to 1.0 */

/**
 * @brief Memex data types
 */
typedef enum {
    MEMEX_TYPE_TEXT,          /**< Text data */
    MEMEX_TYPE_BINARY,         /**< Binary data */
    MEMEX_TYPE_LINK,           /**< Link to other data */
    MEMEX_TYPE_CONCEPT,        /**< Abstract concept */
    MEMEX_TYPE_ENTITY,         /**< Named entity */
    MEMEX_TYPE_RELATION,       /**< Relationship between entities */
    MEMEX_TYPE_QUANTUM_STATE,  /**< Quantum state information */
    MEMEX_TYPE_KNOWLEDGE_NODE  /**< Knowledge graph node */
} MemexDataType;

/**
 * @brief Memex search flags
 */
typedef enum {
    MEMEX_SEARCH_EXACT    = 0x01,  /**< Exact match search */
    MEMEX_SEARCH_FUZZY    = 0x02,  /**< Fuzzy match search */
    MEMEX_SEARCH_SEMANTIC = 0x04,  /**< Semantic search */
    MEMEX_SEARCH_QUANTUM  = 0x08,  /**< Quantum state search */
    MEMEX_SEARCH_PATTERN  = 0x10,  /**< Pattern matching search */
    MEMEX_SEARCH_CONTEXT  = 0x20   /**< Context-aware search */
} MemexSearchFlags;

/**
 * @brief Memex context type
 */
typedef enum {
    MEMEX_CONTEXT_USER,        /**< User context */
    MEMEX_CONTEXT_SYSTEM,      /**< System context */
    MEMEX_CONTEXT_APPLICATION, /**< Application context */
    MEMEX_CONTEXT_DEVICE,      /**< Device context */
    MEMEX_CONTEXT_LOCATION,    /**< Location context */
    MEMEX_CONTEXT_TEMPORAL,    /**< Time-based context */
    MEMEX_CONTEXT_QUANTUM      /**< Quantum state context */
} MemexContextType;

/**
 * @brief Memex data item structure
 */
typedef struct {
    uint64_t id;               /**< Unique identifier */
    MemexDataType type;        /**< Data type */
    char *name;                /**< Item name */
    void *data;                /**< Data content */
    uint64_t data_size;        /**< Data size in bytes */
    char *metadata;            /**< JSON metadata string */
    uint64_t creation_time;    /**< Creation timestamp */
    uint64_t update_time;      /**< Last update timestamp */
    MemexRelevance relevance;  /**< Relevance score (for search results) */
    NodeLevel resonance_level; /**< Associated resonance level */
} MemexDataItem;

/**
 * @brief Memex search query structure
 */
typedef struct {
    char *query_text;          /**< Text query */
    void *query_data;          /**< Binary query data (if applicable) */
    uint64_t query_data_size;  /**< Binary query data size */
    MemexSearchFlags flags;    /**< Search flags */
    uint32_t max_results;      /**< Maximum number of results (0 for no limit) */
    MemexRelevance min_relevance; /**< Minimum relevance score (0.0 to 1.0) */
    NodeLevel min_resonance;   /**< Minimum resonance level */
    char *filter_metadata;     /**< JSON filter metadata */
} MemexSearchQuery;

/**
 * @brief Memex search results structure
 */
typedef struct {
    MemexDataItem **items;     /**< Array of result items */
    uint32_t count;            /**< Number of results */
    uint32_t total_available;  /**< Total available results (may be more than returned) */
    uint64_t search_time;      /**< Search execution time in microseconds */
    char *summary;             /**< Search results summary */
} MemexSearchResults;

/**
 * @brief Memex relation types
 */
typedef enum {
    MEMEX_RELATION_UNDEFINED = 0,
    MEMEX_RELATION_IS_A,         /**< Taxonomic relation */
    MEMEX_RELATION_PART_OF,      /**< Meronymic relation */
    MEMEX_RELATION_HAS_PROPERTY, /**< Attributive relation */
    MEMEX_RELATION_CAUSES,       /**< Causal relation */
    MEMEX_RELATION_PRECEDES,     /**< Temporal relation */
    MEMEX_RELATION_LOCATED_IN,   /**< Spatial relation */
    MEMEX_RELATION_SIMILAR_TO,   /**< Similarity relation */
    MEMEX_RELATION_OPPOSITE_OF,  /**< Antonymic relation */
    MEMEX_RELATION_ENTANGLED     /**< Quantum entanglement relation */
} MemexRelationType;

/**
 * @brief Memex knowledge relation structure
 */
typedef struct {
    uint64_t id;               /**< Relation ID */
    uint64_t source_id;        /**< Source entity ID */
    uint64_t target_id;        /**< Target entity ID */
    MemexRelationType type;    /**< Relation type */
    float weight;              /**< Relation weight (0.0 to 1.0) */
    char *metadata;            /**< JSON metadata */
    NodeLevel resonance_level; /**< Relation resonance level */
    bool is_bidirectional;     /**< Whether relation is bidirectional */
} MemexRelation;

/**
 * @brief Memex context structure
 */
typedef struct {
    uint64_t id;               /**< Context ID */
    MemexContextType type;     /**< Context type */
    char *name;                /**< Context name */
    char *data;                /**< JSON context data */
    uint64_t timestamp;        /**< Context timestamp */
    float relevance;           /**< Context relevance (0.0 to 1.0) */
    NodeLevel resonance_level; /**< Context resonance level */
} MemexContext;

/**
 * @brief Memex initialization options
 */
typedef struct {
    char *data_directory;      /**< Data storage directory */
    uint32_t cache_size_mb;    /**< Cache size in megabytes */
    bool enable_quantum;       /**< Whether to enable quantum features */
    NodeLevel max_resonance;   /**< Maximum resonance level to use */
    QComponentId component_id; /**< Quantum message bus component ID */
    void *custom_config;       /**< Custom configuration (if needed) */
} MemexInitOptions;

/**
 * @brief Initialize the Memex subsystem
 * 
 * @param options Initialization options
 * @return true if initialization succeeded, false otherwise
 */
bool memex_init(const MemexInitOptions *options);

/**
 * @brief Shutdown the Memex subsystem
 */
void memex_shutdown(void);

/**
 * @brief Perform a search query
 * 
 * @param query Search query
 * @return Search results (must be freed with memex_free_search_results)
 */
MemexSearchResults *memex_search(const MemexSearchQuery *query);

/**
 * @brief Free search results
 * 
 * @param results Search results to free
 */
void memex_free_search_results(MemexSearchResults *results);

/**
 * @brief Store a data item
 * 
 * @param item Data item to store
 * @return Stored item ID or 0 on failure
 */
uint64_t memex_store_item(const MemexDataItem *item);

/**
 * @brief Retrieve a data item by ID
 * 
 * @param id Item ID
 * @return Data item (must be freed with memex_free_item) or NULL if not found
 */
MemexDataItem *memex_get_item(uint64_t id);

/**
 * @brief Update a data item
 * 
 * @param item Data item to update
 * @return true if update succeeded, false otherwise
 */
bool memex_update_item(const MemexDataItem *item);

/**
 * @brief Delete a data item
 * 
 * @param id Item ID to delete
 * @return true if deletion succeeded, false otherwise
 */
bool memex_delete_item(uint64_t id);

/**
 * @brief Free a data item
 * 
 * @param item Data item to free
 */
void memex_free_item(MemexDataItem *item);

/**
 * @brief Create a new knowledge relation
 * 
 * @param relation Relation to create
 * @return Relation ID or 0 on failure
 */
uint64_t memex_create_relation(const MemexRelation *relation);

/**
 * @brief Delete a knowledge relation
 * 
 * @param relation_id Relation ID to delete
 * @return true if deletion succeeded, false otherwise
 */
bool memex_delete_relation(uint64_t relation_id);

/**
 * @brief Get relations for an entity
 * 
 * @param entity_id Entity ID
 * @param relation_type Relation type or MEMEX_RELATION_UNDEFINED for all
 * @param max_relations Maximum number of relations to return
 * @param count Pointer to store the number of relations
 * @return Array of relations (must be freed by caller) or NULL on failure
 */
MemexRelation *memex_get_relations(uint64_t entity_id, MemexRelationType relation_type,
                                   uint32_t max_relations, uint32_t *count);

/**
 * @brief Set the current context
 * 
 * @param context Context to set
 * @return true if context was set, false otherwise
 */
bool memex_set_context(const MemexContext *context);

/**
 * @brief Get the current context
 * 
 * @param context_type Context type
 * @return Current context (must be freed by caller) or NULL if not set
 */
MemexContext *memex_get_context(MemexContextType context_type);

/**
 * @brief Create a data item from quantum state
 * 
 * @param quantum_state Quantum state data
 * @param state_size Quantum state data size
 * @param name Item name
 * @param resonance_level Resonance level
 * @return New data item (must be freed with memex_free_item) or NULL on failure
 */
MemexDataItem *memex_create_quantum_item(const void *quantum_state, uint64_t state_size,
                                        const char *name, NodeLevel resonance_level);

/**
 * @brief Create a quantum entanglement between two data items
 * 
 * @param item1_id First item ID
 * @param item2_id Second item ID
 * @param resonance_level Resonance level
 * @return Relation ID or 0 on failure
 */
uint64_t memex_entangle_items(uint64_t item1_id, uint64_t item2_id, NodeLevel resonance_level);

/**
 * @brief Generate a semantic summary of knowledge
 * 
 * @param entity_ids Array of entity IDs to summarize
 * @param entity_count Number of entities
 * @param max_length Maximum summary length
 * @return Summary string (must be freed by caller) or NULL on failure
 */
char *memex_generate_summary(const uint64_t *entity_ids, uint32_t entity_count, uint32_t max_length);

/**
 * @brief Handle a quantum message (internal use)
 * 
 * @param message Quantum message
 * @param context Context
 */
void memex_handle_quantum_message(QMessage *message, void *context);

#endif /* CTRLXT_MEMEX_INTERFACE_H */