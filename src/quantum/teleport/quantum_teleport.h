/**
 * @file quantum_teleport.h
 * @brief Quantum Teleportation System Interface
 *
 * This file defines the interface for the Quantum Teleportation System,
 * which provides advanced teleportation capabilities using quantum entanglement
 * for the CTRLxT OS.
 */

#ifndef CTRLXT_QUANTUM_TELEPORT_H
#define CTRLXT_QUANTUM_TELEPORT_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "../ocular/quantum_ocular.h"
#include "../resonance/resonant_frequencies.h"
#include "../portals/portal_gun.h"

/**
 * @brief Teleportation methods
 */
typedef enum {
    TELEPORT_STANDARD,       /**< Standard quantum teleportation */
    TELEPORT_INSTANT,        /**< Instantaneous zero-time teleportation */
    TELEPORT_SEQUENTIAL,     /**< Sequential multi-stage teleportation */
    TELEPORT_PARALLEL,       /**< Parallel reality traversal teleportation */
    TELEPORT_TEMPORAL        /**< Temporal shift teleportation (time + space) */
} TeleportMethod;

/**
 * @brief Visual effects for teleportation
 */
typedef enum {
    EFFECT_NONE,             /**< No visual effect */
    EFFECT_FADE,             /**< Fade effect */
    EFFECT_DISSOLVE,         /**< Dissolve particles effect */
    EFFECT_BEAM,             /**< Energy beam effect */
    EFFECT_PORTAL,           /**< Portal transition effect */
    EFFECT_QUANTUM           /**< Quantum superposition effect */
} VisualEffect;

/**
 * @brief Blink spot teleportation target
 */
typedef struct {
    uint64_t id;                 /**< Unique identifier */
    char *name;                  /**< Location name */
    char *description;           /**< Location description */
    double latitude;             /**< Latitude */
    double longitude;            /**< Longitude */
    double altitude;             /**< Altitude in meters */
    time_t creation_time;        /**< Creation timestamp */
    NodeLevel resonance_level;   /**< Resonant frequency node level */
    void *satellite_imagery;     /**< Pointer to satellite imagery (if available) */
    uint64_t imagery_size;       /**< Size of satellite imagery data */
    bool is_favorite;            /**< Whether this is a favorite location */
    double stability;            /**< Location stability (0.0 to 1.0) */
} BlinkSpotTarget;

/**
 * @brief Teleportation result structure
 */
typedef struct {
    bool success;                /**< Whether teleportation was successful */
    double energy_used;          /**< Energy used for teleportation */
    double duration;             /**< Duration of teleportation in seconds */
    char *error_message;         /**< Error message if failed */
    double stability;            /**< Stability of the teleportation (0.0 to 1.0) */
    BlinkSpotTarget *source;     /**< Source location (NULL for current location) */
    BlinkSpotTarget *destination;/**< Destination location */
} TeleportResult;

/**
 * @brief Teleportation settings
 */
typedef struct {
    TeleportMethod method;       /**< Teleportation method */
    VisualEffect visual_effect;  /**< Visual effect to use */
    double energy_limit;         /**< Maximum energy to use (0.0 for unlimited) */
    double speed_factor;         /**< Speed factor (1.0 is normal) */
    bool maintain_consciousness; /**< Whether to maintain consciousness during teleport */
    bool create_return_anchor;   /**< Whether to create return anchor at source */
    NodeLevel resonance_level;   /**< Preferred resonance level for teleportation */
    bool allow_time_shift;       /**< Whether to allow temporal displacement */
    bool auto_stabilize;         /**< Whether to auto-stabilize after teleport */
} TeleportSettings;

/**
 * @brief Initialize the quantum teleportation system
 * 
 * @param qopu Pointer to an initialized Quantum Ocular Processing Unit
 * @return true if initialization succeeded, false otherwise
 */
bool qteleport_init(void *qopu);

/**
 * @brief Create a new blink spot target
 * 
 * @param name Location name
 * @param description Location description
 * @param latitude Latitude
 * @param longitude Longitude
 * @param altitude Altitude in meters
 * @param resonance_level Resonant frequency node level
 * @return New BlinkSpotTarget structure if successful, NULL otherwise
 */
BlinkSpotTarget *qteleport_create_blink_spot(const char *name, const char *description,
                                          double latitude, double longitude, double altitude,
                                          NodeLevel resonance_level);

/**
 * @brief List all available blink spot targets
 * 
 * @param count Pointer to store the number of targets
 * @return Array of BlinkSpotTarget pointers (must be freed by the caller)
 */
BlinkSpotTarget **qteleport_list_blink_spots(uint32_t *count);

/**
 * @brief Find blink spot targets by criteria
 * 
 * @param search_term Search term (NULL to ignore)
 * @param near_latitude Center latitude for proximity search (0 to ignore)
 * @param near_longitude Center longitude for proximity search (0 to ignore)
 * @param radius_km Search radius in kilometers (0 to ignore)
 * @param favorites_only Whether to return only favorites
 * @param count Pointer to store the number of targets
 * @return Array of matching BlinkSpotTarget pointers (must be freed by the caller)
 */
BlinkSpotTarget **qteleport_find_blink_spots(const char *search_term,
                                          double near_latitude, double near_longitude,
                                          double radius_km, bool favorites_only,
                                          uint32_t *count);

/**
 * @brief Teleport to a blink spot target
 * 
 * @param target_id Target blink spot identifier
 * @param settings Teleportation settings
 * @return TeleportResult structure with teleportation results
 */
TeleportResult qteleport_to_blink_spot(uint64_t target_id, TeleportSettings settings);

/**
 * @brief Teleport to specific coordinates
 * 
 * @param latitude Destination latitude
 * @param longitude Destination longitude
 * @param altitude Destination altitude in meters
 * @param settings Teleportation settings
 * @return TeleportResult structure with teleportation results
 */
TeleportResult qteleport_to_coordinates(double latitude, double longitude, 
                                      double altitude, TeleportSettings settings);

/**
 * @brief Mark a blink spot as favorite
 * 
 * @param target_id Target blink spot identifier
 * @param is_favorite Whether to mark as favorite
 * @return true if operation succeeded, false otherwise
 */
bool qteleport_set_favorite(uint64_t target_id, bool is_favorite);

/**
 * @brief Update a blink spot's information
 * 
 * @param target_id Target blink spot identifier
 * @param name New name (NULL to keep current)
 * @param description New description (NULL to keep current)
 * @param resonance_level New resonance level (-1 to keep current)
 * @return true if update succeeded, false otherwise
 */
bool qteleport_update_blink_spot(uint64_t target_id, const char *name,
                                const char *description, int resonance_level);

/**
 * @brief Delete a blink spot target
 * 
 * @param target_id Target blink spot identifier
 * @return true if deletion succeeded, false otherwise
 */
bool qteleport_delete_blink_spot(uint64_t target_id);

/**
 * @brief Get the default teleportation settings
 * 
 * @return Default TeleportSettings structure
 */
TeleportSettings qteleport_get_default_settings(void);

/**
 * @brief Get the last teleportation result
 * 
 * @return Last TeleportResult structure
 */
TeleportResult qteleport_get_last_result(void);

/**
 * @brief Shutdown the quantum teleportation system
 * 
 * @return true if shutdown succeeded, false otherwise
 */
bool qteleport_shutdown(void);

#endif /* CTRLXT_QUANTUM_TELEPORT_H */