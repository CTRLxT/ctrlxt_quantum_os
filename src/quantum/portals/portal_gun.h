/**
 * @file portal_gun.h
 * @brief Portal Gun Interface for Interdimensional Transportation
 *
 * This file defines the interface for the Portal Gun, a quantum device that
 * creates wormholes between different points in spacetime or between
 * different realities in the multiverse.
 */

#ifndef CTRLXT_PORTAL_GUN_H
#define CTRLXT_PORTAL_GUN_H

#include <stdint.h>
#include <stdbool.h>
#include "../resonance/resonant_frequencies.h"
#include "../entanglement/entanglement_manager.h"

/**
 * @brief Portal types supported by the Portal Gun
 */
typedef enum {
    PORTAL_SPATIAL,         /**< Portal between two spatial locations */
    PORTAL_TEMPORAL,         /**< Portal between two points in time */
    PORTAL_DIMENSIONAL,      /**< Portal between two dimensions/realities */
    PORTAL_MENTAL,           /**< Portal between two mind states */
    PORTAL_QUANTUM_STATE     /**< Portal between two quantum states */
} PortalType;

/**
 * @brief Portal stability levels
 */
typedef enum {
    STABILITY_UNSTABLE,     /**< Highly unstable, temporary portal */
    STABILITY_FLUCTUATING,   /**< Fluctuating stability, needs maintenance */
    STABILITY_STABLE,        /**< Stable portal, reliable operation */
    STABILITY_RESONANT,      /**< Resonant stability, self-sustaining */
    STABILITY_PERMANENT      /**< Permanent, fixed portal */
} PortalStability;

/**
 * @brief Portal appearance characteristics
 */
typedef struct {
    NodeColorProfile entry_color;    /**< Color of entry portal */
    NodeColorProfile exit_color;     /**< Color of exit portal */
    double diameter;                 /**< Portal diameter in meters */
    bool has_event_horizon;          /**< Whether portal has visible event horizon */
    bool has_energy_field;           /**< Whether portal has visible energy field */
    const char *custom_appearance;   /**< Custom appearance description (if any) */
} PortalAppearance;

/**
 * @brief Portal coordinates structure
 */
typedef struct {
    double x;                    /**< X coordinate */
    double y;                    /**< Y coordinate */
    double z;                    /**< Z coordinate */
    double t;                    /**< Temporal coordinate */
    uint64_t dimension_id;       /**< Dimension/reality identifier */
    double quantum_state[64];    /**< Quantum state vector (if applicable) */
} PortalCoordinates;

/**
 * @brief Portal structure
 */
typedef struct {
    uint64_t id;                     /**< Unique portal identifier */
    PortalType type;                 /**< Portal type */
    PortalStability stability;       /**< Portal stability level */
    PortalAppearance appearance;     /**< Portal appearance */
    PortalCoordinates entry;         /**< Entry portal coordinates */
    PortalCoordinates exit;          /**< Exit portal coordinates */
    EntanglementId entanglement;     /**< Associated quantum entanglement */
    double power_level;              /**< Power level (0.0 to 100.0) */
    uint64_t creator_id;             /**< Creator identifier */
    NodeLevel resonance_level;       /**< Resonant frequency node level */
} Portal;

/**
 * @brief Portal Gun settings structure
 */
typedef struct {
    PortalType default_type;             /**< Default portal type */
    PortalStability default_stability;   /**< Default stability level */
    NodeColorProfile entry_color;        /**< Default entry portal color */
    NodeColorProfile exit_color;         /**< Default exit portal color */
    double default_diameter;             /**< Default portal diameter */
    double max_distance;                 /**< Maximum portal distance */
    uint32_t max_portals;                /**< Maximum active portals */
    bool auto_stabilize;                 /**< Whether to auto-stabilize portals */
    double power_efficiency;             /**< Power efficiency (0.0 to 1.0) */
    NodeLevel resonance_level;           /**< Operating resonance level */
} PortalGunSettings;

/**
 * @brief Initialize the Portal Gun
 * 
 * @param settings Initial settings for the Portal Gun
 * @param user_id User identifier
 * @return true if initialization succeeded, false otherwise
 */
bool portal_gun_init(PortalGunSettings settings, uint64_t user_id);

/**
 * @brief Create a new portal
 * 
 * @param type Portal type
 * @param entry_coordinates Entry coordinates
 * @param exit_coordinates Exit coordinates
 * @param appearance Portal appearance (NULL for default)
 * @return New Portal structure if successful, NULL otherwise
 */
Portal *portal_gun_create_portal(PortalType type,
                               PortalCoordinates entry_coordinates,
                               PortalCoordinates exit_coordinates,
                               PortalAppearance *appearance);

/**
 * @brief Close an existing portal
 * 
 * @param portal_id Portal identifier
 * @return true if closing succeeded, false otherwise
 */
bool portal_gun_close_portal(uint64_t portal_id);

/**
 * @brief Get information about an existing portal
 * 
 * @param portal_id Portal identifier
 * @return Portal structure with information
 */
Portal portal_gun_get_portal_info(uint64_t portal_id);

/**
 * @brief Modify an existing portal's properties
 * 
 * @param portal_id Portal identifier
 * @param new_exit_coordinates New exit coordinates (NULL to keep current)
 * @param new_appearance New appearance (NULL to keep current)
 * @param new_stability New stability level (or -1 to keep current)
 * @return true if modification succeeded, false otherwise
 */
bool portal_gun_modify_portal(uint64_t portal_id,
                             PortalCoordinates *new_exit_coordinates,
                             PortalAppearance *new_appearance,
                             int new_stability);

/**
 * @brief Check if travel through a portal is currently safe
 * 
 * @param portal_id Portal identifier
 * @return true if travel is safe, false otherwise
 */
bool portal_gun_is_travel_safe(uint64_t portal_id);

/**
 * @brief Get currently active portals
 * 
 * @param max_count Maximum number of portals to return
 * @param actual_count Pointer to store actual count
 * @return Array of Portal structures (must be freed by caller)
 */
Portal *portal_gun_get_active_portals(uint32_t max_count, uint32_t *actual_count);

/**
 * @brief Update Portal Gun settings
 * 
 * @param new_settings New settings
 * @return true if update succeeded, false otherwise
 */
bool portal_gun_update_settings(PortalGunSettings new_settings);

/**
 * @brief Calibrate the Portal Gun to a specific resonance level
 * 
 * @param target_level Target node level for resonance
 * @return true if calibration succeeded, false otherwise
 */
bool portal_gun_calibrate(NodeLevel target_level);

/**
 * @brief Emergency shutdown of all portals
 * 
 * @return true if shutdown succeeded, false otherwise
 */
bool portal_gun_emergency_shutdown(void);

#endif /* CTRLXT_PORTAL_GUN_H */