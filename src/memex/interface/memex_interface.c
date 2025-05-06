/**
 * @file memex_interface.c
 * @brief Implementation of Memex integration interface
 */

#include "memex_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Forward declarations of internal components */
static bool init_search_engine(const MemexInitOptions *options);
static bool init_knowledge_graph(const MemexInitOptions *options);
static bool init_context_engine(const MemexInitOptions *options);
static bool register_with_quantum_bus(const MemexInitOptions *options);

/* Memex state */
static bool memex_initialized = false;
static MemexInitOptions memex_options = {0};
static MemexContext *current_contexts[MEMEX_CONTEXT_QUANTUM + 1] = {NULL};

/* Next ID for items and relations */
static uint64_t next_item_id = 1;
static uint64_t next_relation_id = 1;

/* In-memory storage for demonstration - would be persistent in a real implementation */
#define MAX_ITEMS 1000
#define MAX_RELATIONS 5000
static MemexDataItem *items[MAX_ITEMS] = {NULL};
static MemexRelation *relations[MAX_RELATIONS] = {NULL};

/**
 * @brief Create a deep copy of a data item
 */
static MemexDataItem *clone_data_item(const MemexDataItem *item) {
    if (!item) return NULL;
    
    MemexDataItem *clone = (MemexDataItem *)malloc(sizeof(MemexDataItem));
    if (!clone) return NULL;
    
    /* Copy basic fields */
    clone->id = item->id;
    clone->type = item->type;
    clone->data_size = item->data_size;
    clone->creation_time = item->creation_time;
    clone->update_time = item->update_time;
    clone->relevance = item->relevance;
    clone->resonance_level = item->resonance_level;
    
    /* Copy name if present */
    if (item->name) {
        clone->name = strdup(item->name);
        if (!clone->name) {
            free(clone);
            return NULL;
        }
    } else {
        clone->name = NULL;
    }
    
    /* Copy data if present */
    if (item->data && item->data_size > 0) {
        clone->data = malloc(item->data_size);
        if (!clone->data) {
            free(clone->name);
            free(clone);
            return NULL;
        }
        memcpy(clone->data, item->data, item->data_size);
    } else {
        clone->data = NULL;
    }
    
    /* Copy metadata if present */
    if (item->metadata) {
        clone->metadata = strdup(item->metadata);
        if (!clone->metadata) {
            free(clone->data);
            free(clone->name);
            free(clone);
            return NULL;
        }
    } else {
        clone->metadata = NULL;
    }
    
    return clone;
}

/**
 * @brief Create a deep copy of a relation
 */
static MemexRelation *clone_relation(const MemexRelation *relation) {
    if (!relation) return NULL;
    
    MemexRelation *clone = (MemexRelation *)malloc(sizeof(MemexRelation));
    if (!clone) return NULL;
    
    /* Copy basic fields */
    clone->id = relation->id;
    clone->source_id = relation->source_id;
    clone->target_id = relation->target_id;
    clone->type = relation->type;
    clone->weight = relation->weight;
    clone->resonance_level = relation->resonance_level;
    clone->is_bidirectional = relation->is_bidirectional;
    
    /* Copy metadata if present */
    if (relation->metadata) {
        clone->metadata = strdup(relation->metadata);
        if (!clone->metadata) {
            free(clone);
            return NULL;
        }
    } else {
        clone->metadata = NULL;
    }
    
    return clone;
}

/**
 * @brief Free a context
 */
static void free_context(MemexContext *context) {
    if (!context) return;
    
    free(context->name);
    free(context->data);
    free(context);
}

/**
 * @brief Message handler for quantum bus messages
 */
void memex_handle_quantum_message(QMessage *message, void *context) {
    if (!memex_initialized || !message) return;
    
    printf("Memex received quantum message: Type=%u\n", message->header.type);
    
    /* Process the message based on type */
    switch (message->header.type) {
        case QMSG_SYSTEM_STARTUP:
            /* System is starting up */
            printf("Memex: System startup notification received\n");
            break;
            
        case QMSG_SYSTEM_SHUTDOWN:
            /* System is shutting down */
            printf("Memex: System shutdown notification received\n");
            break;
            
        default:
            /* Handle other message types as needed */
            break;
    }
}

/**
 * @brief Initialize the Memex subsystem
 */
bool memex_init(const MemexInitOptions *options) {
    if (memex_initialized) {
        printf("Memex already initialized\n");
        return true;
    }
    
    if (!options) {
        printf("Invalid Memex initialization options\n");
        return false;
    }
    
    /* Store the options */
    memex_options.data_directory = options->data_directory ? strdup(options->data_directory) : NULL;
    memex_options.cache_size_mb = options->cache_size_mb;
    memex_options.enable_quantum = options->enable_quantum;
    memex_options.max_resonance = options->max_resonance;
    memex_options.component_id = options->component_id;
    memex_options.custom_config = options->custom_config;
    
    /* Initialize components */
    if (!init_search_engine(options)) {
        printf("Failed to initialize Memex search engine\n");
        goto cleanup;
    }
    
    if (!init_knowledge_graph(options)) {
        printf("Failed to initialize Memex knowledge graph\n");
        goto cleanup;
    }
    
    if (!init_context_engine(options)) {
        printf("Failed to initialize Memex context engine\n");
        goto cleanup;
    }
    
    /* Register with quantum message bus if enabled */
    if (options->enable_quantum) {
        if (!register_with_quantum_bus(options)) {
            printf("Warning: Failed to register with quantum message bus\n");
            /* Continue anyway, as non-quantum features will still work */
        }
    }
    
    /* Initialize storage */
    memset(items, 0, sizeof(items));
    memset(relations, 0, sizeof(relations));
    
    memex_initialized = true;
    printf("Memex subsystem initialized successfully\n");
    return true;
    
cleanup:
    free(memex_options.data_directory);
    memex_options.data_directory = NULL;
    return false;
}

/**
 * @brief Initialize the search engine component
 */
static bool init_search_engine(const MemexInitOptions *options) {
    printf("Initializing Memex search engine...\n");
    /* In a real implementation, this would initialize a sophisticated search engine */
    /* For now, we'll just simulate success */
    return true;
}

/**
 * @brief Initialize the knowledge graph component
 */
static bool init_knowledge_graph(const MemexInitOptions *options) {
    printf("Initializing Memex knowledge graph...\n");
    /* In a real implementation, this would initialize a knowledge graph database */
    /* For now, we'll just simulate success */
    return true;
}

/**
 * @brief Initialize the context engine component
 */
static bool init_context_engine(const MemexInitOptions *options) {
    printf("Initializing Memex context engine...\n");
    /* Initialize context array */
    for (int i = 0; i <= MEMEX_CONTEXT_QUANTUM; i++) {
        current_contexts[i] = NULL;
    }
    return true;
}

/**
 * @brief Register with the quantum message bus
 */
static bool register_with_quantum_bus(const MemexInitOptions *options) {
    printf("Registering Memex with quantum message bus...\n");
    
    /* Register component with the quantum message bus */
    QComponentInfo component_info = {
        .id = options->component_id,
        .name = "Memex Integration",
        .resonance_level = options->max_resonance,
        .context = NULL
    };
    
    bool result = qbus_register_component(&component_info);
    if (!result) {
        printf("Failed to register Memex with quantum message bus\n");
        return false;
    }
    
    /* Subscribe to relevant message types */
    QSubscription subscription = {
        .component_id = options->component_id,
        .message_type = -1, /* All message types */
        .handler = memex_handle_quantum_message,
        .context = NULL,
        .min_resonance = NODE_ZERO_POINT
    };
    
    result = qbus_subscribe(&subscription);
    if (!result) {
        printf("Failed to subscribe Memex to quantum messages\n");
        qbus_unregister_component(options->component_id);
        return false;
    }
    
    printf("Memex registered with quantum message bus successfully\n");
    return true;
}

/**
 * @brief Shutdown the Memex subsystem
 */
void memex_shutdown(void) {
    if (!memex_initialized) {
        return;
    }
    
    /* Unregister from quantum message bus if enabled */
    if (memex_options.enable_quantum) {
        qbus_unregister_component(memex_options.component_id);
    }
    
    /* Free all stored items */
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i]) {
            memex_free_item(items[i]);
            items[i] = NULL;
        }
    }
    
    /* Free all stored relations */
    for (int i = 0; i < MAX_RELATIONS; i++) {
        if (relations[i]) {
            free(relations[i]->metadata);
            free(relations[i]);
            relations[i] = NULL;
        }
    }
    
    /* Free all contexts */
    for (int i = 0; i <= MEMEX_CONTEXT_QUANTUM; i++) {
        if (current_contexts[i]) {
            free_context(current_contexts[i]);
            current_contexts[i] = NULL;
        }
    }
    
    /* Free options */
    free(memex_options.data_directory);
    memex_options.data_directory = NULL;
    
    memex_initialized = false;
    printf("Memex subsystem shutdown complete\n");
}

/**
 * @brief Perform a search query
 */
MemexSearchResults *memex_search(const MemexSearchQuery *query) {
    if (!memex_initialized || !query) {
        return NULL;
    }
    
    printf("Executing Memex search: '%s'\n", query->query_text ? query->query_text : "<binary>");
    
    /* Create results structure */
    MemexSearchResults *results = (MemexSearchResults *)malloc(sizeof(MemexSearchResults));
    if (!results) {
        return NULL;
    }
    
    /* For demonstration, we'll implement a simple in-memory search */
    uint32_t max_results = query->max_results > 0 ? query->max_results : MAX_ITEMS;
    MemexDataItem **result_items = (MemexDataItem **)malloc(sizeof(MemexDataItem *) * max_results);
    if (!result_items) {
        free(results);
        return NULL;
    }
    
    uint32_t count = 0;
    
    /* Simulate search through items */
    for (int i = 0; i < MAX_ITEMS && count < max_results; i++) {
        if (items[i]) {
            bool match = false;
            
            /* Simple text matching for demonstration */
            if (query->query_text && items[i]->name && 
                strstr(items[i]->name, query->query_text) != NULL) {
                match = true;
            }
            
            /* Check resonance level */
            if (match && items[i]->resonance_level < query->min_resonance) {
                match = false;
            }
            
            if (match) {
                /* Clone the item for the results */
                result_items[count] = clone_data_item(items[i]);
                if (result_items[count]) {
                    /* Set relevance based on some criteria */
                    result_items[count]->relevance = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
                    count++;
                }
            }
        }
    }
    
    /* Fill in results */
    results->items = result_items;
    results->count = count;
    results->total_available = count; /* In a real implementation, this could be larger */
    results->search_time = 100; /* Simulated 100 microseconds */
    
    /* Create a summary */
    const char *summary_template = "Found %u results for query '%s'";
    int summary_size = snprintf(NULL, 0, summary_template, count, 
                              query->query_text ? query->query_text : "<binary>");
    results->summary = (char *)malloc(summary_size + 1);
    if (results->summary) {
        snprintf(results->summary, summary_size + 1, summary_template, count, 
                query->query_text ? query->query_text : "<binary>");
    } else {
        results->summary = NULL;
    }
    
    return results;
}

/**
 * @brief Free search results
 */
void memex_free_search_results(MemexSearchResults *results) {
    if (!results) {
        return;
    }
    
    /* Free all result items */
    if (results->items) {
        for (uint32_t i = 0; i < results->count; i++) {
            if (results->items[i]) {
                memex_free_item(results->items[i]);
            }
        }
        free(results->items);
    }
    
    /* Free summary */
    free(results->summary);
    
    /* Free results structure */
    free(results);
}

/**
 * @brief Store a data item
 */
uint64_t memex_store_item(const MemexDataItem *item) {
    if (!memex_initialized || !item) {
        return 0;
    }
    
    /* Find a free slot */
    int free_slot = -1;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i]) {
            free_slot = i;
            break;
        }
    }
    
    if (free_slot == -1) {
        printf("Memex storage full\n");
        return 0;
    }
    
    /* Clone the item */
    MemexDataItem *new_item = clone_data_item(item);
    if (!new_item) {
        return 0;
    }
    
    /* Assign a new ID and timestamps */
    new_item->id = next_item_id++;
    new_item->creation_time = time(NULL);
    new_item->update_time = new_item->creation_time;
    
    /* Store the item */
    items[free_slot] = new_item;
    
    printf("Stored Memex item %llu: %s\n", 
           (unsigned long long)new_item->id, new_item->name ? new_item->name : "<unnamed>");
    
    return new_item->id;
}

/**
 * @brief Retrieve a data item by ID
 */
MemexDataItem *memex_get_item(uint64_t id) {
    if (!memex_initialized || id == 0) {
        return NULL;
    }
    
    /* Search for the item */
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i] && items[i]->id == id) {
            /* Return a clone of the item */
            return clone_data_item(items[i]);
        }
    }
    
    return NULL;
}

/**
 * @brief Update a data item
 */
bool memex_update_item(const MemexDataItem *item) {
    if (!memex_initialized || !item || item->id == 0) {
        return false;
    }
    
    /* Find the item */
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i] && items[i]->id == item->id) {
            /* Create an updated clone */
            MemexDataItem *updated_item = clone_data_item(item);
            if (!updated_item) {
                return false;
            }
            
            /* Update the timestamp */
            updated_item->update_time = time(NULL);
            
            /* Replace the old item */
            memex_free_item(items[i]);
            items[i] = updated_item;
            
            printf("Updated Memex item %llu\n", (unsigned long long)item->id);
            return true;
        }
    }
    
    return false;
}

/**
 * @brief Delete a data item
 */
bool memex_delete_item(uint64_t id) {
    if (!memex_initialized || id == 0) {
        return false;
    }
    
    /* Find the item */
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i] && items[i]->id == id) {
            /* Free the item */
            memex_free_item(items[i]);
            items[i] = NULL;
            
            printf("Deleted Memex item %llu\n", (unsigned long long)id);
            return true;
        }
    }
    
    return false;
}

/**
 * @brief Free a data item
 */
void memex_free_item(MemexDataItem *item) {
    if (!item) {
        return;
    }
    
    /* Free allocated resources */
    free(item->name);
    free(item->data);
    free(item->metadata);
    
    /* Free the item structure */
    free(item);
}

/**
 * @brief Create a new knowledge relation
 */
uint64_t memex_create_relation(const MemexRelation *relation) {
    if (!memex_initialized || !relation) {
        return 0;
    }
    
    /* Verify source and target exist */
    bool source_exists = false;
    bool target_exists = false;
    
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i]) {
            if (items[i]->id == relation->source_id) {
                source_exists = true;
            }
            if (items[i]->id == relation->target_id) {
                target_exists = true;
            }
        }
        if (source_exists && target_exists) {
            break;
        }
    }
    
    if (!source_exists || !target_exists) {
        printf("Memex relation source or target does not exist\n");
        return 0;
    }
    
    /* Find a free slot */
    int free_slot = -1;
    for (int i = 0; i < MAX_RELATIONS; i++) {
        if (!relations[i]) {
            free_slot = i;
            break;
        }
    }
    
    if (free_slot == -1) {
        printf("Memex relation storage full\n");
        return 0;
    }
    
    /* Clone the relation */
    MemexRelation *new_relation = clone_relation(relation);
    if (!new_relation) {
        return 0;
    }
    
    /* Assign a new ID */
    new_relation->id = next_relation_id++;
    
    /* Store the relation */
    relations[free_slot] = new_relation;
    
    printf("Created Memex relation %llu: %llu -> %llu (type: %d)\n", 
           (unsigned long long)new_relation->id, 
           (unsigned long long)new_relation->source_id,
           (unsigned long long)new_relation->target_id,
           new_relation->type);
    
    return new_relation->id;
}

/**
 * @brief Delete a knowledge relation
 */
bool memex_delete_relation(uint64_t relation_id) {
    if (!memex_initialized || relation_id == 0) {
        return false;
    }
    
    /* Find the relation */
    for (int i = 0; i < MAX_RELATIONS; i++) {
        if (relations[i] && relations[i]->id == relation_id) {
            /* Free the relation */
            free(relations[i]->metadata);
            free(relations[i]);
            relations[i] = NULL;
            
            printf("Deleted Memex relation %llu\n", (unsigned long long)relation_id);
            return true;
        }
    }
    
    return false;
}

/**
 * @brief Get relations for an entity
 */
MemexRelation *memex_get_relations(uint64_t entity_id, MemexRelationType relation_type,
                                  uint32_t max_relations, uint32_t *count) {
    if (!memex_initialized || entity_id == 0 || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    /* First, count matching relations */
    uint32_t matching_count = 0;
    for (int i = 0; i < MAX_RELATIONS; i++) {
        if (relations[i] && 
            (relations[i]->source_id == entity_id || relations[i]->target_id == entity_id) &&
            (relation_type == MEMEX_RELATION_UNDEFINED || relations[i]->type == relation_type)) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        *count = 0;
        return NULL;
    }
    
    /* Limit to max_relations if specified */
    uint32_t result_count = max_relations > 0 && max_relations < matching_count ? 
                          max_relations : matching_count;
    
    /* Allocate result array */
    MemexRelation *result = (MemexRelation *)malloc(sizeof(MemexRelation) * result_count);
    if (!result) {
        *count = 0;
        return NULL;
    }
    
    /* Fill in results */
    uint32_t result_index = 0;
    for (int i = 0; i < MAX_RELATIONS && result_index < result_count; i++) {
        if (relations[i] && 
            (relations[i]->source_id == entity_id || relations[i]->target_id == entity_id) &&
            (relation_type == MEMEX_RELATION_UNDEFINED || relations[i]->type == relation_type)) {
            /* Copy the relation */
            MemexRelation *relation_copy = clone_relation(relations[i]);
            if (relation_copy) {
                memcpy(&result[result_index], relation_copy, sizeof(MemexRelation));
                free(relation_copy); /* Just free the structure since we copied its contents */
                result_index++;
            }
        }
    }
    
    *count = result_index;
    return result;
}

/**
 * @brief Set the current context
 */
bool memex_set_context(const MemexContext *context) {
    if (!memex_initialized || !context) {
        return false;
    }
    
    /* Validate context type */
    if (context->type > MEMEX_CONTEXT_QUANTUM) {
        return false;
    }
    
    /* Check if we already have a context of this type */
    if (current_contexts[context->type]) {
        /* Free the existing context */
        free_context(current_contexts[context->type]);
    }
    
    /* Create a new context */
    MemexContext *new_context = (MemexContext *)malloc(sizeof(MemexContext));
    if (!new_context) {
        return false;
    }
    
    /* Copy basic fields */
    new_context->id = context->id > 0 ? context->id : (uint64_t)time(NULL);
    new_context->type = context->type;
    new_context->timestamp = context->timestamp > 0 ? context->timestamp : (uint64_t)time(NULL);
    new_context->relevance = context->relevance;
    new_context->resonance_level = context->resonance_level;
    
    /* Copy name if present */
    if (context->name) {
        new_context->name = strdup(context->name);
        if (!new_context->name) {
            free(new_context);
            return false;
        }
    } else {
        new_context->name = NULL;
    }
    
    /* Copy data if present */
    if (context->data) {
        new_context->data = strdup(context->data);
        if (!new_context->data) {
            free(new_context->name);
            free(new_context);
            return false;
        }
    } else {
        new_context->data = NULL;
    }
    
    /* Store the context */
    current_contexts[context->type] = new_context;
    
    printf("Set Memex context: type=%d, name=%s\n", 
           context->type, context->name ? context->name : "<unnamed>");
    
    return true;
}

/**
 * @brief Get the current context
 */
MemexContext *memex_get_context(MemexContextType context_type) {
    if (!memex_initialized) {
        return NULL;
    }
    
    /* Validate context type */
    if (context_type > MEMEX_CONTEXT_QUANTUM) {
        return NULL;
    }
    
    /* Check if we have a context of this type */
    if (!current_contexts[context_type]) {
        return NULL;
    }
    
    /* Create a copy of the context */
    MemexContext *context_copy = (MemexContext *)malloc(sizeof(MemexContext));
    if (!context_copy) {
        return NULL;
    }
    
    /* Copy basic fields */
    context_copy->id = current_contexts[context_type]->id;
    context_copy->type = current_contexts[context_type]->type;
    context_copy->timestamp = current_contexts[context_type]->timestamp;
    context_copy->relevance = current_contexts[context_type]->relevance;
    context_copy->resonance_level = current_contexts[context_type]->resonance_level;
    
    /* Copy name if present */
    if (current_contexts[context_type]->name) {
        context_copy->name = strdup(current_contexts[context_type]->name);
        if (!context_copy->name) {
            free(context_copy);
            return NULL;
        }
    } else {
        context_copy->name = NULL;
    }
    
    /* Copy data if present */
    if (current_contexts[context_type]->data) {
        context_copy->data = strdup(current_contexts[context_type]->data);
        if (!context_copy->data) {
            free(context_copy->name);
            free(context_copy);
            return NULL;
        }
    } else {
        context_copy->data = NULL;
    }
    
    return context_copy;
}

/**
 * @brief Create a data item from quantum state
 */
MemexDataItem *memex_create_quantum_item(const void *quantum_state, uint64_t state_size,
                                       const char *name, NodeLevel resonance_level) {
    if (!memex_initialized || !quantum_state || state_size == 0) {
        return NULL;
    }
    
    /* Create a new data item */
    MemexDataItem *item = (MemexDataItem *)malloc(sizeof(MemexDataItem));
    if (!item) {
        return NULL;
    }
    
    /* Initialize the item */
    memset(item, 0, sizeof(MemexDataItem));
    item->type = MEMEX_TYPE_QUANTUM_STATE;
    item->resonance_level = resonance_level;
    
    /* Set the name if provided */
    if (name) {
        item->name = strdup(name);
        if (!item->name) {
            free(item);
            return NULL;
        }
    }
    
    /* Copy the quantum state data */
    item->data = malloc(state_size);
    if (!item->data) {
        free(item->name);
        free(item);
        return NULL;
    }
    
    memcpy(item->data, quantum_state, state_size);
    item->data_size = state_size;
    
    return item;
}

/**
 * @brief Create a quantum entanglement between two data items
 */
uint64_t memex_entangle_items(uint64_t item1_id, uint64_t item2_id, NodeLevel resonance_level) {
    if (!memex_initialized || item1_id == 0 || item2_id == 0) {
        return 0;
    }
    
    /* Check if both items exist */
    bool item1_exists = false;
    bool item2_exists = false;
    
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i]) {
            if (items[i]->id == item1_id) {
                item1_exists = true;
            }
            if (items[i]->id == item2_id) {
                item2_exists = true;
            }
        }
        if (item1_exists && item2_exists) {
            break;
        }
    }
    
    if (!item1_exists || !item2_exists) {
        printf("Cannot entangle: one or both items do not exist\n");
        return 0;
    }
    
    /* Create a relation representing the entanglement */
    MemexRelation relation = {
        .id = 0, /* Will be assigned by memex_create_relation */
        .source_id = item1_id,
        .target_id = item2_id,
        .type = MEMEX_RELATION_ENTANGLED,
        .weight = 1.0, /* Maximum weight for entanglement */
        .metadata = NULL,
        .resonance_level = resonance_level,
        .is_bidirectional = true /* Entanglement is always bidirectional */
    };
    
    uint64_t relation_id = memex_create_relation(&relation);
    if (relation_id == 0) {
        printf("Failed to create entanglement relation\n");
        return 0;
    }
    
    printf("Created quantum entanglement between items %llu and %llu\n",
           (unsigned long long)item1_id, (unsigned long long)item2_id);
    
    return relation_id;
}

/**
 * @brief Generate a semantic summary of knowledge
 */
char *memex_generate_summary(const uint64_t *entity_ids, uint32_t entity_count, uint32_t max_length) {
    if (!memex_initialized || !entity_ids || entity_count == 0) {
        return NULL;
    }
    
    /* In a real implementation, this would perform sophisticated summarization */
    /* For demonstration, we'll just create a simple summary */
    
    /* Allocate a buffer for the summary */
    char *summary = (char *)malloc(max_length + 1);
    if (!summary) {
        return NULL;
    }
    
    /* Start with a header */
    int offset = snprintf(summary, max_length, "Summary of %u entities:\n", entity_count);
    
    /* Add information about each entity */
    for (uint32_t i = 0; i < entity_count && offset < max_length; i++) {
        /* Find the entity */
        MemexDataItem *item = memex_get_item(entity_ids[i]);
        if (item) {
            /* Add entity information */
            offset += snprintf(summary + offset, max_length - offset, 
                             "Entity %llu: %s\n", 
                             (unsigned long long)item->id,
                             item->name ? item->name : "<unnamed>");
            
            /* Free the item */
            memex_free_item(item);
        }
    }
    
    return summary;
}