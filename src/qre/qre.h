/**
 * @file qre.h
 * @brief Unified Quantum Reality Engine interface
 *
 * This file defines the interface for the Unified Quantum Reality Engine (QRE),
 * which provides mixed reality interfaces, quantum-enhanced simulations, and
 * multi-dimensional data visualization.
 */

#ifndef CTRLXT_QRE_H
#define CTRLXT_QRE_H

#include <stdint.h>
#include <stdbool.h>
#include "../quantum/entanglement/entanglement_manager.h"
#include "../memex/knowledge/knowledge_network.h"

/**
 * @brief Reality mode types
 */
typedef enum {
    REALITY_PHYSICAL,    /**< Physical reality (no augmentation) */
    REALITY_AUGMENTED,   /**< Augmented reality */
    REALITY_VIRTUAL,     /**< Virtual reality */
    REALITY_MIXED,       /**< Mixed reality */
    REALITY_QUANTUM      /**< Quantum reality (entanglement-based) */
} RealityModeType;

/**
 * @brief Visualization dimension types
 */
typedef enum {
    DIM_2D,         /**< 2D visualization */
    DIM_3D,         /**< 3D visualization */
    DIM_4D,         /**< 4D visualization (3D + time) */
    DIM_MULTI,      /**< Multi-dimensional visualization */
    DIM_QUANTUM     /**< Quantum dimensions (superposition-based) */
} VisualizationDimType;

/**
 * @brief Reality object structure
 */
typedef struct {
    uint64_t id;                     /**< Unique identifier */
    char *name;                      /**< Object name */
    void *geometry_data;             /**< Geometry data */
    uint64_t geometry_size;          /**< Geometry data size */
    void *material_data;             /**< Material data */
    uint64_t material_size;          /**< Material data size */
    bool is_interactive;             /**< Whether object is interactive */
    EntanglementId *entanglement;    /**< Quantum entanglement (if applicable) */
    KnowledgeNode *knowledge_node;   /**< Associated knowledge node (if any) */
} RealityObject;

/**
 * @brief Reality space structure
 */
typedef struct {
    uint64_t id;                     /**< Unique identifier */
    RealityModeType mode;            /**< Reality mode */
    VisualizationDimType dimensions; /**< Visualization dimensions */
    RealityObject **objects;         /**< Array of objects in the space */
    uint32_t object_count;           /**< Number of objects */
    EntanglementId *entanglement;    /**< Quantum entanglement (if applicable) */
} RealitySpace;

/**
 * @brief Initialize the Unified Quantum Reality Engine
 * 
 * @param mode Initial reality mode
 * @param dimensions Initial visualization dimensions
 * @param use_quantum Whether to use quantum entanglement
 * @return true if initialization succeeded, false otherwise
 */
bool qre_init(RealityModeType mode, VisualizationDimType dimensions, bool use_quantum);

/**
 * @brief Create a new reality space
 * 
 * @param mode Reality mode
 * @param dimensions Visualization dimensions
 * @param use_quantum Whether to use quantum entanglement
 * @return New RealitySpace structure
 */
RealitySpace qre_create_space(RealityModeType mode, 
                            VisualizationDimType dimensions,
                            bool use_quantum);

/**
 * @brief Create a new reality object
 * 
 * @param space_id Space ID to add the object to
 * @param name Object name
 * @param geometry_data Geometry data
 * @param geometry_size Geometry data size
 * @param material_data Material data
 * @param material_size Material data size
 * @param is_interactive Whether object is interactive
 * @param use_quantum Whether to use quantum entanglement
 * @param knowledge_node_id Associated knowledge node ID (0 for none)
 * @return New RealityObject structure
 */
RealityObject qre_create_object(uint64_t space_id,
                              const char *name,
                              void *geometry_data,
                              uint64_t geometry_size,
                              void *material_data,
                              uint64_t material_size,
                              bool is_interactive,
                              bool use_quantum,
                              uint64_t knowledge_node_id);

/**
 * @brief Synchronize a reality space across entangled devices
 * 
 * @param space_id Space ID to synchronize
 * @return true if synchronization succeeded, false otherwise
 */
bool qre_sync_space(uint64_t space_id);

/**
 * @brief Render a reality space
 * 
 * @param space_id Space ID to render
 * @param output_buffer Output buffer for rendering
 * @param buffer_size Output buffer size
 * @return true if rendering succeeded, false otherwise
 */
bool qre_render_space(uint64_t space_id, void *output_buffer, uint64_t buffer_size);

/**
 * @brief Shutdown the Unified Quantum Reality Engine
 */
void qre_shutdown(void);

#endif /* CTRLXT_QRE_H */