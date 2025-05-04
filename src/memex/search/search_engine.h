/**
 * @file search_engine.h
 * @brief Memex Search Engine integration interface
 *
 * This file defines the interface for integrating Memex's advanced search
 * capabilities with CTRLxT OS.
 */

#ifndef CTRLXT_MEMEX_SEARCH_H
#define CTRLXT_MEMEX_SEARCH_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Search result types
 */
typedef enum {
    RESULT_FILE,         /**< File result */
    RESULT_APP,          /**< Application result */
    RESULT_CONTACT,      /**< Contact result */
    RESULT_WEB,          /**< Web result */
    RESULT_KNOWLEDGE,    /**< Knowledge result */
    RESULT_QUANTUM       /**< Quantum-entangled result */
} SearchResultType;

/**
 * @brief Search result structure
 */
typedef struct {
    uint64_t id;               /**< Unique identifier */
    SearchResultType type;     /**< Type of result */
    char *title;               /**< Result title */
    char *description;         /**< Result description */
    char *uri;                 /**< Uniform Resource Identifier */
    float relevance;           /**< Relevance score (0.0 to 1.0) */
    bool is_quantum_entangled; /**< Whether result is quantum-entangled */
} SearchResult;

/**
 * @brief Search options structure
 */
typedef struct {
    bool include_files;        /**< Include file results */
    bool include_apps;         /**< Include application results */
    bool include_contacts;     /**< Include contact results */
    bool include_web;          /**< Include web results */
    bool include_knowledge;    /**< Include knowledge results */
    bool quantum_prioritize;   /**< Prioritize quantum-entangled results */
    uint32_t max_results;      /**< Maximum number of results to return */
    char *language;            /**< Preferred language for results */
} SearchOptions;

/**
 * @brief Initialize the Memex Search Engine integration
 * 
 * @return true if initialization succeeded, false otherwise
 */
bool memex_search_init(void);

/**
 * @brief Perform a search using Memex's advanced search capabilities
 * 
 * @param query Search query string
 * @param options Search options
 * @param result_count Pointer to store the number of results
 * @return Array of SearchResult structures (must be freed by the caller)
 */
SearchResult *memex_search_query(const char *query, SearchOptions options, 
                               uint32_t *result_count);

/**
 * @brief Free search results memory
 * 
 * @param results Array of search results to free
 * @param result_count Number of results in the array
 */
void memex_search_free_results(SearchResult *results, uint32_t result_count);

/**
 * @brief Get suggested search queries based on a partial query
 * 
 * @param partial_query Partial search query string
 * @param max_suggestions Maximum number of suggestions to return
 * @param suggestion_count Pointer to store the number of suggestions
 * @return Array of suggested query strings (must be freed by the caller)
 */
char **memex_search_get_suggestions(const char *partial_query, 
                                  uint32_t max_suggestions,
                                  uint32_t *suggestion_count);

/**
 * @brief Shutdown the Memex Search Engine integration
 */
void memex_search_shutdown(void);

#endif /* CTRLXT_MEMEX_SEARCH_H */