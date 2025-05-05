/**
 * @file portal_gun.c
 * @brief Implementation of the Portal Gun for Interdimensional Transportation
 *
 * This file implements the Portal Gun interface, providing functionality for
 * creating and managing wormholes between different points in spacetime or
 * different realities in the multiverse.
 */

#include "portal_gun.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/**
 * @brief Structure for internal portal information and management
 */
typedef struct {
    Portal portal_data;              /**< Portal data */
    bool is_active;                  /**< Whether this portal is active */
    time_t creation_time;            /**< Creation timestamp */
    time_t last_traversal_time;      /**< Last traversal timestamp */
    uint32_t traversal_count;        /**< Number of traversals */
    double energy_consumption;       /**< Energy consumed so far */
    double stability_factor;         /**< Current stability factor (0.0-1.0) */
} PortalRecord;

/* Static variables */
static PortalGunSettings current_settings;
static PortalRecord* portal_registry = NULL;
static uint32_t max_portals = 0;
static uint32_t active_portals = 0;
static uint64_t next_portal_id = 1;
static uint64_t user_id = 0;
static bool is_initialized = false;

/**
 * @brief Calculate distance between two portal coordinates
 * 
 * @param coord1 First coordinates
 * @param coord2 Second coordinates
 * @return Distance value
 */
static double calculate_distance(PortalCoordinates coord1, PortalCoordinates coord2) {
    // Calculate spatial distance
    double spatial_dist = sqrt(
        pow(coord2.x - coord1.x, 2) +
        pow(coord2.y - coord1.y, 2) +
        pow(coord2.z - coord1.z, 2));
        
    // Include temporal component if in same dimension
    double temporal_dist = 0.0;
    if (coord1.dimension_id == coord2.dimension_id) {
        temporal_dist = fabs(coord2.t - coord1.t) * 0.1; // Scale factor for time
    }
    
    // Add dimension shift factor
    double dimension_factor = (coord1.dimension_id == coord2.dimension_id) ? 0.0 : 100.0;
    
    return spatial_dist + temporal_dist + dimension_factor;
}

/**
 * @brief Get available slot in portal registry
 * 
 * @return Index of available slot, or -1 if none available
 */
static int32_t get_available_portal_slot() {
    if (!is_initialized) {
        return -1;
    }
    
    for (uint32_t i = 0; i < max_portals; i++) {
        if (!portal_registry[i].is_active) {
            return i;
        }
    }
    
    return -1; // No slots available
}

/**
 * @brief Find a portal in the registry by ID
 * 
 * @param portal_id Portal ID to find
 * @return Index in registry, or -1 if not found
 */
static int32_t find_portal(uint64_t portal_id) {
    if (!is_initialized) {
        return -1;
    }
    
    for (uint32_t i = 0; i < max_portals; i++) {
        if (portal_registry[i].is_active && 
            portal_registry[i].portal_data.id == portal_id) {
            return i;
        }
    }
    
    return -1; // Not found
}

/**
 * @brief Update portal stability based on various factors
 * 
 * @param slot Registry slot to update
 */
static void update_portal_stability(int32_t slot) {
    if (slot < 0 || slot >= (int32_t)max_portals || !portal_registry[slot].is_active) {
        return;
    }
    
    PortalRecord* record = &portal_registry[slot];
    time_t current_time = time(NULL);
    
    // Calculate age factor (older portals become less stable)
    double age_hours = difftime(current_time, record->creation_time) / 3600.0;
    double age_factor = 1.0 - (age_hours / 1000.0); // Decay over 1000 hours
    if (age_factor < 0.0) age_factor = 0.0;
    
    // Calculate distance factor (longer distances are less stable)
    double distance = calculate_distance(
        record->portal_data.entry, 
        record->portal_data.exit);
    double distance_factor = 1.0 - (distance / current_settings.max_distance);
    if (distance_factor < 0.0) distance_factor = 0.0;
    
    // Calculate resonance factor (closer to target resonance is more stable)
    double resonance_factor = 1.0 - fabs(
        ((double)record->portal_data.resonance_level - 
         (double)current_settings.resonance_level) / 14.0); // Assumes 14 levels
    
    // Calculate usage factor (more usage reduces stability)
    double usage_factor = 1.0 - (record->traversal_count / 1000.0);
    if (usage_factor < 0.0) usage_factor = 0.0;
    
    // Combine factors with different weights
    double stability = (
        (age_factor * 0.2) +
        (distance_factor * 0.3) +
        (resonance_factor * 0.3) +
        (usage_factor * 0.2));
        
    // Apply auto-stabilization if enabled
    if (current_settings.auto_stabilize && stability < 0.5) {
        stability += 0.2; // Boost stability
        if (stability > 1.0) stability = 1.0;
    }
    
    // Update the stability factor
    record->stability_factor = stability;
    
    // Map to PortalStability enum
    if (stability < 0.2) {
        record->portal_data.stability = STABILITY_UNSTABLE;
    } else if (stability < 0.4) {
        record->portal_data.stability = STABILITY_FLUCTUATING;
    } else if (stability < 0.7) {
        record->portal_data.stability = STABILITY_STABLE;
    } else if (stability < 0.9) {
        record->portal_data.stability = STABILITY_RESONANT;
    } else {
        record->portal_data.stability = STABILITY_PERMANENT;
    }
}

/**
 * @brief Initialize the Portal Gun
 * 
 * @param settings Initial settings for the Portal Gun
 * @param user_id User identifier
 * @return true if initialization succeeded, false otherwise
 */
bool portal_gun_init(PortalGunSettings settings, uint64_t user_identifier) {
    // If already initialized, shut down first
    if (is_initialized) {
        portal_gun_emergency_shutdown();
        // Free the registry
        if (portal_registry != NULL) {
            free(portal_registry);
            portal_registry = NULL;
        }
        is_initialized = false;
    }
    
    // Validate settings
    if (settings.max_portals == 0 || settings.max_distance <= 0.0) {
        return false;
    }
    
    // Allocate portal registry
    portal_registry = (PortalRecord*)calloc(settings.max_portals, sizeof(PortalRecord));
    if (portal_registry == NULL) {
        return false; // Memory allocation failed
    }
    
    // Initialize registry
    for (uint32_t i = 0; i < settings.max_portals; i++) {
        portal_registry[i].is_active = false;
    }
    
    // Store settings and state
    current_settings = settings;
    max_portals = settings.max_portals;
    active_portals = 0;
    next_portal_id = 1;
    user_id = user_identifier;
    is_initialized = true;
    
    // Initialize random seed for stability calculations
    srand((unsigned int)time(NULL));
    
    return true;
}

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
                               PortalAppearance *appearance) {
    // Check initialization
    if (!is_initialized) {
        return NULL;
    }
    
    // Check if we've reached maximum portals
    if (active_portals >= max_portals) {
        return NULL;
    }
    
    // Get available slot
    int32_t slot = get_available_portal_slot();
    if (slot < 0) {
        return NULL;
    }
    
    // Check distance limit
    double distance = calculate_distance(entry_coordinates, exit_coordinates);
    if (distance > current_settings.max_distance) {
        return NULL; // Distance exceeds limit
    }
    
    // Initialize portal record
    PortalRecord* record = &portal_registry[slot];
    
    // Set basic properties
    record->portal_data.id = next_portal_id++;
    record->portal_data.type = type;
    record->portal_data.entry = entry_coordinates;
    record->portal_data.exit = exit_coordinates;
    record->portal_data.creator_id = user_id;
    record->portal_data.power_level = 100.0; // Start at full power
    record->portal_data.resonance_level = current_settings.resonance_level;
    
    // Set appearance
    if (appearance != NULL) {
        record->portal_data.appearance = *appearance;
    } else {
        // Use default appearance
        record->portal_data.appearance.entry_color = current_settings.entry_color;
        record->portal_data.appearance.exit_color = current_settings.exit_color;
        record->portal_data.appearance.diameter = current_settings.default_diameter;
        record->portal_data.appearance.has_event_horizon = true;
        record->portal_data.appearance.has_energy_field = true;
        record->portal_data.appearance.custom_appearance = NULL;
    }
    
    // Initialize tracking data
    record->is_active = true;
    record->creation_time = time(NULL);
    record->last_traversal_time = 0;
    record->traversal_count = 0;
    record->energy_consumption = 0.0;
    record->stability_factor = 0.9; // Start with high stability
    
    // Create quantum entanglement for the portal
    record->portal_data.entanglement = qem_create_entanglement(
        ENTANGLE_DEVICE, 
        (uint64_t)&record->portal_data.entry, 
        (uint64_t)&record->portal_data.exit, 
        8); // Use 8 qubits for entanglement
    
    // Set initial stability
    update_portal_stability(slot);
    
    // Increment active portal count
    active_portals++;
    
    return &record->portal_data;
}

/**
 * @brief Close an existing portal
 * 
 * @param portal_id Portal identifier
 * @return true if closing succeeded, false otherwise
 */
bool portal_gun_close_portal(uint64_t portal_id) {
    // Check initialization
    if (!is_initialized) {
        return false;
    }
    
    // Find the portal
    int32_t slot = find_portal(portal_id);
    if (slot < 0) {
        return false; // Portal not found
    }
    
    // Get portal record
    PortalRecord* record = &portal_registry[slot];
    
    // Destroy quantum entanglement
    if (record->portal_data.entanglement.is_active) {
        qem_destroy_entanglement(record->portal_data.entanglement.id);
    }
    
    // Mark portal as inactive
    record->is_active = false;
    
    // Decrement active portal count
    active_portals--;
    
    return true;
}

/**
 * @brief Get information about an existing portal
 * 
 * @param portal_id Portal identifier
 * @return Portal structure with information
 */
Portal portal_gun_get_portal_info(uint64_t portal_id) {
    Portal empty_portal = {0};
    
    // Check initialization
    if (!is_initialized) {
        return empty_portal;
    }
    
    // Find the portal
    int32_t slot = find_portal(portal_id);
    if (slot < 0) {
        return empty_portal; // Portal not found
    }
    
    // Update stability before returning
    update_portal_stability(slot);
    
    // Return portal data
    return portal_registry[slot].portal_data;
}

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
                             int new_stability) {
    // Check initialization
    if (!is_initialized) {
        return false;
    }
    
    // Find the portal
    int32_t slot = find_portal(portal_id);
    if (slot < 0) {
        return false; // Portal not found
    }
    
    // Get portal record
    PortalRecord* record = &portal_registry[slot];
    
    // Update exit coordinates if provided
    if (new_exit_coordinates != NULL) {
        // Check if new distance is within limits
        double new_distance = calculate_distance(
            record->portal_data.entry, 
            *new_exit_coordinates);
            
        if (new_distance > current_settings.max_distance) {
            return false; // New distance exceeds limit
        }
        
        // Update coordinates
        record->portal_data.exit = *new_exit_coordinates;
    }
    
    // Update appearance if provided
    if (new_appearance != NULL) {
        record->portal_data.appearance = *new_appearance;
    }
    
    // Update stability if provided
    if (new_stability >= 0 && new_stability <= STABILITY_PERMANENT) {
        record->portal_data.stability = (PortalStability)new_stability;
        
        // Map stability enum to factor
        switch(record->portal_data.stability) {
            case STABILITY_UNSTABLE:
                record->stability_factor = 0.1;
                break;
            case STABILITY_FLUCTUATING:
                record->stability_factor = 0.3;
                break;
            case STABILITY_STABLE:
                record->stability_factor = 0.6;
                break;
            case STABILITY_RESONANT:
                record->stability_factor = 0.8;
                break;
            case STABILITY_PERMANENT:
                record->stability_factor = 1.0;
                break;
        }
    } else {
        // Recalculate stability
        update_portal_stability(slot);
    }
    
    return true;
}

/**
 * @brief Check if travel through a portal is currently safe
 * 
 * @param portal_id Portal identifier
 * @return true if travel is safe, false otherwise
 */
bool portal_gun_is_travel_safe(uint64_t portal_id) {
    // Check initialization
    if (!is_initialized) {
        return false;
    }
    
    // Find the portal
    int32_t slot = find_portal(portal_id);
    if (slot < 0) {
        return false; // Portal not found
    }
    
    // Update stability
    update_portal_stability(slot);
    
    // Get portal record
    PortalRecord* record = &portal_registry[slot];
    
    // Check if the entanglement is still active
    if (!record->portal_data.entanglement.is_active) {
        return false;
    }
    
    // Check if the power level is sufficient
    if (record->portal_data.power_level < 20.0) {
        return false;
    }
    
    // Check stability - only STABLE and above is considered safe
    if (record->portal_data.stability < STABILITY_STABLE) {
        return false;
    }
    
    return true;
}

/**
 * @brief Get currently active portals
 * 
 * @param max_count Maximum number of portals to return
 * @param actual_count Pointer to store actual count
 * @return Array of Portal structures (must be freed by caller)
 */
Portal *portal_gun_get_active_portals(uint32_t max_count, uint32_t *actual_count) {
    // Check initialization
    if (!is_initialized || actual_count == NULL) {
        if (actual_count) *actual_count = 0;
        return NULL;
    }
    
    // If there are no active portals, return NULL
    if (active_portals == 0) {
        *actual_count = 0;
        return NULL;
    }
    
    // Limit the count to actual active portals
    uint32_t count_to_return = (max_count < active_portals) ? max_count : active_portals;
    *actual_count = count_to_return;
    
    // Allocate array for portal data
    Portal* portal_array = (Portal*)malloc(count_to_return * sizeof(Portal));
    if (portal_array == NULL) {
        *actual_count = 0;
        return NULL;
    }
    
    // Fill array with active portals
    uint32_t array_index = 0;
    for (uint32_t i = 0; i < max_portals && array_index < count_to_return; i++) {
        if (portal_registry[i].is_active) {
            // Update stability before returning
            update_portal_stability(i);
            
            // Copy portal data
            portal_array[array_index] = portal_registry[i].portal_data;
            array_index++;
        }
    }
    
    return portal_array;
}

/**
 * @brief Update Portal Gun settings
 * 
 * @param new_settings New settings
 * @return true if update succeeded, false otherwise
 */
bool portal_gun_update_settings(PortalGunSettings new_settings) {
    // Check initialization
    if (!is_initialized) {
        return false;
    }
    
    // Validate new settings
    if (new_settings.max_portals == 0 || new_settings.max_distance <= 0.0) {
        return false;
    }
    
    // If max_portals is being reduced, ensure we're not below active count
    if (new_settings.max_portals < active_portals) {
        return false;
    }
    
    // If max_portals is being increased, resize the registry
    if (new_settings.max_portals > max_portals) {
        PortalRecord* new_registry = (PortalRecord*)realloc(
            portal_registry, 
            new_settings.max_portals * sizeof(PortalRecord));
            
        if (new_registry == NULL) {
            return false; // Memory allocation failed
        }
        
        // Initialize new slots
        for (uint32_t i = max_portals; i < new_settings.max_portals; i++) {
            new_registry[i].is_active = false;
        }
        
        portal_registry = new_registry;
    }
    
    // Update settings
    current_settings = new_settings;
    max_portals = new_settings.max_portals;
    
    // Update all portals' stability
    for (uint32_t i = 0; i < max_portals; i++) {
        if (portal_registry[i].is_active) {
            update_portal_stability(i);
        }
    }
    
    return true;
}

/**
 * @brief Calibrate the Portal Gun to a specific resonance level
 * 
 * @param target_level Target node level for resonance
 * @return true if calibration succeeded, false otherwise
 */
bool portal_gun_calibrate(NodeLevel target_level) {
    // Check initialization
    if (!is_initialized) {
        return false;
    }
    
    // Validate target level
    if (target_level < 0 || target_level > 13) { // Assuming 14 levels (0-13)
        return false;
    }
    
    // Update resonance level in settings
    current_settings.resonance_level = target_level;
    
    // Update all portals' stability
    for (uint32_t i = 0; i < max_portals; i++) {
        if (portal_registry[i].is_active) {
            update_portal_stability(i);
        }
    }
    
    return true;
}

/**
 * @brief Emergency shutdown of all portals
 * 
 * @return true if shutdown succeeded, false otherwise
 */
bool portal_gun_emergency_shutdown(void) {
    // Check initialization
    if (!is_initialized) {
        return false;
    }
    
    // Attempt to close all active portals
    for (uint32_t i = 0; i < max_portals; i++) {
        if (portal_registry[i].is_active) {
            // Destroy quantum entanglement
            if (portal_registry[i].portal_data.entanglement.is_active) {
                qem_destroy_entanglement(portal_registry[i].portal_data.entanglement.id);
            }
            
            // Mark portal as inactive
            portal_registry[i].is_active = false;
        }
    }
    
    // Reset active portal count
    active_portals = 0;
    
    return true;
}