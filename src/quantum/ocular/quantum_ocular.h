/**
 * @file quantum_ocular.h
 * @brief Quantum Ocular Processing Unit (Q-OPU) Interface
 *
 * This file defines the interface for the Quantum Ocular Processing Unit,
 * which serves as the visual and perceptual processing component of CTRLxT OS.
 * It integrates with the Unified Quantum Reality Engine and Portal Gun systems.
 */

#ifndef CTRLXT_QUANTUM_OCULAR_H
#define CTRLXT_QUANTUM_OCULAR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../resonance/resonant_frequencies.h"
#include "../portals/portal_gun.h"
#include "../../memex/knowledge/knowledge_network.h"

/**
 * @brief Q-OPU composition types
 */
typedef enum {
    COMP_STANDARD,                /**< Standard processing unit */
    COMP_COSMIC_DUST,             /**< Cosmic dust enhanced */
    COMP_QUANTUM_CRYSTAL,         /**< Quantum crystal based */
    COMP_NEURAL_ORGANIC,          /**< Neural-organic hybrid */
    COMP_MULTIDIMENSIONAL         /**< Multidimensional processing */
} OcularComposition;

/**
 * @brief Q-OPU processing models
 */
typedef enum {
    MODEL_LOGICAL,                /**< Logical processing model */
    MODEL_QUANTUM,                /**< Quantum processing model */
    MODEL_BIO_QUANTUM,            /**< Bio-quantum hybrid model */
    MODEL_NEURAL_QUANTUM,         /**< Neural-quantum model */
    MODEL_COSMIC_QUANTUM          /**< Cosmic quantum model */
} ProcessingModel;

/**
 * @brief Q-OPU zero point interfaces
 */
typedef enum {
    INTERFACE_STANDARD,           /**< Standard interface */
    INTERFACE_NEURAL,             /**< Neural interface */
    INTERFACE_QUANTUM_FIELD,      /**< Quantum field interface */
    INTERFACE_DIMENSIONAL,        /**< Dimensional interface */
    INTERFACE_COSMIC              /**< Cosmic interface */
} ZeroPointInterface;

/**
 * @brief Q-OPU Reality Engine modes
 */
typedef enum {
    QOPU_REALITY_EXISTING,             /**< Existing reality observation */
    QOPU_REALITY_AUGMENTED,            /**< Augmented reality */
    QOPU_REALITY_SIMULATED,            /**< Fully simulated reality */
    QOPU_REALITY_ALTERNATIVE,          /**< Alternative reality observation */
    QOPU_REALITY_QUANTUM_SUPERPOSITION /**< Superposition of multiple realities */
} RealityEngineMode;

/**
 * @brief Light spectrum channel
 */
typedef struct {
    const char *name;             /**< Channel name */
    const char *associated_entity; /**< Associated entity name */
    NodeColorProfile color;        /**< Color profile */
    double integrity;              /**< Channel integrity (0.0 to 1.0) */
    double frequency;              /**< Channel frequency in Hz */
    bool enabled;                  /**< Whether channel is enabled */
} LightSpectrumChannel;

/**
 * @brief Blink spot (teleportation location)
 */
typedef struct {
    uint64_t id;                  /**< Unique identifier */
    char *name;                   /**< Location name */
    double latitude;              /**< Latitude */
    double longitude;             /**< Longitude */
    double altitude;              /**< Altitude in meters */
    RealityEngineMode reality_mode; /**< Reality mode at this location */
    uint64_t timestamp;           /**< Creation/update timestamp */
} BlinkSpot;

/**
 * @brief Q-OPU configuration
 */
typedef struct {
    OcularComposition composition;     /**< Q-OPU composition */
    ProcessingModel processing_model;   /**< Processing model */
    ZeroPointInterface interface;       /**< Zero point interface */
    bool quantum_tunneling_enabled;     /**< Whether quantum tunneling is enabled */
    RealityEngineMode reality_mode;     /**< Current reality engine mode */
    LightSpectrumChannel *channels;     /**< Light spectrum channels */
    uint32_t channel_count;             /**< Number of channels */
    BlinkSpot *blink_spots;             /**< Blink spots */
    uint32_t blink_spot_count;          /**< Number of blink spots */
    double zero_point_frequency;        /**< Zero point control frequency */
    bool teleportation_enabled;         /**< Whether teleportation is enabled */
    uint32_t current_audio_level;       /**< Current audio upgrade level */
} OcularConfig;

/**
 * @brief Visual processing modes
 */
typedef enum {
    VISUAL_MODE_STANDARD,           /**< Standard visual processing */
    VISUAL_MODE_QUANTUM_ENHANCED,   /**< Quantum-enhanced processing */
    VISUAL_MODE_REALITY_AUGMENTED,  /**< Reality augmentation mode */
    VISUAL_MODE_QUANTUM_FUSION,     /**< Quantum state fusion mode */
    VISUAL_MODE_MULTIDIMENSIONAL    /**< Multi-dimensional processing */
} VisualProcessingMode;

/**
 * @brief Quantum visual enhancement types
 */
typedef enum {
    ENHANCE_NONE = 0,
    ENHANCE_QUANTUM_CLARITY = 1,    /**< Quantum clarity enhancement */
    ENHANCE_REALITY_OVERLAY = 2,    /**< Reality overlay enhancement */
    ENHANCE_QUANTUM_FILTER = 4,     /**< Quantum filtering */
    ENHANCE_DIMENSIONAL_SHIFT = 8,  /**< Dimensional shift enhancement */
    ENHANCE_QUANTUM_FUSION = 16     /**< Quantum state fusion */
} QuantumEnhancementType;

/**
 * @brief Visual processing parameters
 */
typedef struct {
    VisualProcessingMode mode;              /**< Processing mode */
    uint32_t enhancement_flags;             /**< Enhancement flags */
    float quantum_clarity_factor;           /**< Quantum clarity factor (0.0 to 1.0) */
    float reality_overlay_strength;         /**< Reality overlay strength (0.0 to 1.0) */
    float quantum_filter_threshold;         /**< Quantum filter threshold */
    float dimensional_shift_factor;         /**< Dimensional shift factor */
    NodeLevel resonance_level;              /**< Resonance level for processing */
    bool preserve_quantum_state;            /**< Whether to preserve quantum state */
    char *reality_template;                 /**< Reality template for augmentation */
} VisualProcessingParams;

/**
 * @brief Quantum visual data structure
 */
typedef struct {
    void *raw_data;                         /**< Raw visual data */
    uint32_t raw_size;                      /**< Size of raw data */
    void *quantum_state;                    /**< Associated quantum state */
    uint32_t quantum_state_size;            /**< Size of quantum state */
    float *enhancement_weights;             /**< Enhancement weights */
    uint32_t weight_count;                  /**< Number of weights */
    NodeLevel resonance_level;              /**< Resonance level */
    bool is_quantum_entangled;              /**< Whether data is quantum entangled */
    char *metadata;                         /**< JSON metadata */
} QuantumVisualData;

/**
 * @brief Initialize the Quantum Ocular Processing Unit
 * 
 * @param config Initial configuration
 * @return true if initialization succeeded, false otherwise
 */
bool qopu_init(OcularConfig config);

/**
 * @brief Set the Q-OPU's light spectrum channel integrity
 * 
 * @param channel_name Channel name
 * @param integrity Integrity value (0.0 to 1.0)
 * @return true if the operation succeeded, false otherwise
 */
bool qopu_set_channel_integrity(const char *channel_name, double integrity);

/**
 * @brief Create a new blink spot for teleportation
 * 
 * @param name Location name
 * @param latitude Latitude
 * @param longitude Longitude
 * @param altitude Altitude in meters
 * @param reality_mode Reality mode at this location
 * @return New BlinkSpot structure if successful, NULL otherwise
 */
BlinkSpot *qopu_create_blink_spot(const char *name, double latitude, 
                               double longitude, double altitude,
                               RealityEngineMode reality_mode);

/**
 * @brief Teleport to a blink spot
 * 
 * @param blink_spot_id Blink spot identifier
 * @return true if teleportation succeeded, false otherwise
 */
bool qopu_teleport(uint64_t blink_spot_id);

/**
 * @brief Create a wormhole between blink spots
 * 
 * @param entry_spot_id Entry blink spot identifier
 * @param exit_spot_id Exit blink spot identifier
 * @param portal Portal structure to populate
 * @return true if wormhole creation succeeded, false otherwise
 */
bool qopu_create_wormhole(uint64_t entry_spot_id, uint64_t exit_spot_id, Portal *portal);

/**
 * @brief Upgrade quantum audio capabilities
 * 
 * @param level Upgrade level (1-5)
 * @return true if upgrade succeeded, false otherwise
 */
bool qopu_upgrade_audio(uint32_t level);

/**
 * @brief Change the reality engine mode
 * 
 * @param mode New reality engine mode
 * @return true if the change succeeded, false otherwise
 */
bool qopu_set_reality_mode(RealityEngineMode mode);

/**
 * @brief Get data from a quantum data source
 * 
 * @param source_name Source name
 * @param data_buffer Buffer to store data
 * @param buffer_size Buffer size
 * @return Number of bytes written to the buffer, or -1 on error
 */
int32_t qopu_get_quantum_data(const char *source_name, void *data_buffer, uint32_t buffer_size);

/**
 * @brief Process visual input through the Q-OPU
 * 
 * @param input_buffer Input visual data
 * @param input_size Input buffer size
 * @param output_buffer Output processed data
 * @param output_size Output buffer size
 * @return Number of bytes written to the output buffer, or -1 on error
 */
int32_t qopu_process_visual(const void *input_buffer, uint32_t input_size,
                          void *output_buffer, uint32_t output_size);

/**
 * @brief Process visual input through the Q-OPU with enhanced capabilities
 * 
 * @param input_data Input visual data
 * @param params Processing parameters
 * @param output_buffer Output processed data
 * @param output_size Output buffer size
 * @return Number of bytes written to the output buffer, or -1 on error
 */
int32_t qopu_process_visual_enhanced(const QuantumVisualData *input_data,
                                   const VisualProcessingParams *params,
                                   void *output_buffer,
                                   uint32_t output_size);

/**
 * @brief Apply quantum enhancements to visual data
 * 
 * @param visual_data Visual data to enhance
 * @param enhancement_type Type of enhancement to apply
 * @param strength Enhancement strength (0.0 to 1.0)
 * @return true if enhancement succeeded, false otherwise
 */
bool qopu_apply_quantum_enhancement(QuantumVisualData *visual_data,
                                  QuantumEnhancementType enhancement_type,
                                  float strength);

/**
 * @brief Fuse quantum states with visual data
 * 
 * @param visual_data Visual data to fuse
 * @param quantum_state Quantum state to fuse with
 * @param fusion_strength Fusion strength (0.0 to 1.0)
 * @return true if fusion succeeded, false otherwise
 */
bool qopu_fuse_quantum_state(QuantumVisualData *visual_data,
                           const void *quantum_state,
                           float fusion_strength);

/**
 * @brief Shut down the Quantum Ocular Processing Unit
 * 
 * @return true if shutdown succeeded, false otherwise
 */
bool qopu_shutdown(void);

#endif /* CTRLXT_QUANTUM_OCULAR_H */