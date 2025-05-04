/**
 * @file quantum_teleport.c
 * @brief Implementation of Quantum Teleportation System
 */

#include "quantum_teleport.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

/* Path to the teleport_blink.sh script */
#define TELEPORT_SCRIPT_PATH "./src/quantum/teleport/teleport_blink.sh"

/* Maximum number of blink spots */
#define MAX_BLINK_SPOTS 100

/* Static variables for Quantum Teleportation state */
static void *qopu_instance = NULL;
static BlinkSpotTarget *blink_spots[MAX_BLINK_SPOTS];
static uint32_t blink_spot_count = 0;
static TeleportResult last_result;
static bool initialized = false;

/* Earth radius in kilometers for distance calculations */
#define EARTH_RADIUS_KM 6371.0

/**
 * @brief Calculate distance between two geographic coordinates in kilometers
 */
static double calculate_distance(double lat1, double lon1, double lat2, double lon2) {
    /* Convert latitude and longitude from degrees to radians */
    double lat1_rad = lat1 * M_PI / 180.0;
    double lon1_rad = lon1 * M_PI / 180.0;
    double lat2_rad = lat2 * M_PI / 180.0;
    double lon2_rad = lon2 * M_PI / 180.0;
    
    /* Haversine formula */
    double dlon = lon2_rad - lon1_rad;
    double dlat = lat2_rad - lat1_rad;
    double a = sin(dlat/2) * sin(dlat/2) + cos(lat1_rad) * cos(lat2_rad) * sin(dlon/2) * sin(dlon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double distance = EARTH_RADIUS_KM * c;
    
    return distance;
}

/**
 * @brief Execute the teleport_blink.sh script with arguments
 */
static char *execute_teleport_script(const char *command, const char *args[]) {
    char cmd[1024] = {0};
    char *result = NULL;
    FILE *pipe = NULL;
    char buffer[1024] = {0};
    
    /* Build the command */
    strcat(cmd, "bash ");
    strcat(cmd, TELEPORT_SCRIPT_PATH);
    strcat(cmd, " ");
    strcat(cmd, command);
    
    if (args) {
        int i = 0;
        while (args[i]) {
            strcat(cmd, " \"");
            strcat(cmd, args[i]);
            strcat(cmd, "\"");
            i++;
        }
    }
    
    /* Execute the command */
    pipe = popen(cmd, "r");
    if (!pipe) {
        return NULL;
    }
    
    /* Read the output */
    result = malloc(4096);
    if (!result) {
        pclose(pipe);
        return NULL;
    }
    
    result[0] = '\0';
    while (fgets(buffer, sizeof(buffer), pipe)) {
        strcat(result, buffer);
    }
    
    pclose(pipe);
    return result;
}

/**
 * @brief Find a blink spot by ID
 */
static BlinkSpotTarget *find_blink_spot_by_id(uint64_t id) {
    for (uint32_t i = 0; i < blink_spot_count; i++) {
        if (blink_spots[i]->id == id) {
            return blink_spots[i];
        }
    }
    return NULL;
}

/**
 * @brief Calculate teleportation energy cost
 */
static double calculate_energy_cost(BlinkSpotTarget *source, BlinkSpotTarget *destination, 
                                TeleportMethod method, double speed_factor) {
    /* Base energy cost for teleportation */
    double base_cost = 100.0;
    
    /* Calculate distance factor */
    double distance_km = 0.0;
    if (source) {
        distance_km = calculate_distance(source->latitude, source->longitude,
                                       destination->latitude, destination->longitude);
    } else {
        /* Assume a default distance if source is not provided */
        distance_km = 1000.0;
    }
    
    /* Distance factor: Energy increases with square root of distance */
    double distance_factor = sqrt(distance_km) / 10.0;
    
    /* Method factor: Different methods have different energy costs */
    double method_factor = 1.0;
    switch (method) {
        case TELEPORT_STANDARD:
            method_factor = 1.0;
            break;
        case TELEPORT_INSTANT:
            method_factor = 2.5;
            break;
        case TELEPORT_SEQUENTIAL:
            method_factor = 1.2;
            break;
        case TELEPORT_PARALLEL:
            method_factor = 2.0;
            break;
        case TELEPORT_TEMPORAL:
            method_factor = 3.0;
            break;
    }
    
    /* Speed factor: Higher speed costs more energy */
    double speed_cost = speed_factor * speed_factor;
    
    /* Resonance factor: Different resonance levels have different energy efficiencies */
    double resonance_factor = 1.0;
    if (destination->resonance_level == NODE_PORTAL_TECHNICIAN) {
        resonance_factor = 0.7; /* More efficient for portal-specific resonance */
    } else if (destination->resonance_level == NODE_DIMENSIONAL_ANCHOR) {
        resonance_factor = 0.8; /* More efficient for dimensional anchors */
    }
    
    /* Calculate final energy cost */
    double energy_cost = base_cost * distance_factor * method_factor * speed_cost * resonance_factor;
    
    return energy_cost;
}

/**
 * @brief Calculate teleportation duration
 */
static double calculate_duration(BlinkSpotTarget *source, BlinkSpotTarget *destination, 
                             TeleportMethod method, double speed_factor) {
    /* Base duration for teleportation (in seconds) */
    double base_duration = 3.0;
    
    /* Calculate distance factor */
    double distance_km = 0.0;
    if (source) {
        distance_km = calculate_distance(source->latitude, source->longitude,
                                       destination->latitude, destination->longitude);
    } else {
        /* Assume a default distance if source is not provided */
        distance_km = 1000.0;
    }
    
    /* Distance factor: Duration increases with logarithm of distance */
    double distance_factor = log10(distance_km + 1.0) / 3.0;
    if (distance_factor < 0.1) distance_factor = 0.1;
    
    /* Method factor: Different methods have different durations */
    double method_factor = 1.0;
    switch (method) {
        case TELEPORT_STANDARD:
            method_factor = 1.0;
            break;
        case TELEPORT_INSTANT:
            method_factor = 0.1;
            break;
        case TELEPORT_SEQUENTIAL:
            method_factor = 2.0;
            break;
        case TELEPORT_PARALLEL:
            method_factor = 0.5;
            break;
        case TELEPORT_TEMPORAL:
            method_factor = 1.5;
            break;
    }
    
    /* Speed factor: Higher speed reduces duration */
    double speed_effect = 1.0 / speed_factor;
    
    /* Calculate final duration */
    double duration = base_duration * distance_factor * method_factor * speed_effect;
    
    /* Ensure minimum duration */
    if (duration < 0.1) duration = 0.1;
    
    return duration;
}

/**
 * @brief Initialize the quantum teleportation system
 */
bool qteleport_init(void *qopu) {
    if (initialized) {
        return true; /* Already initialized */
    }
    
    /* Store the Q-OPU instance */
    qopu_instance = qopu;
    
    /* Initialize the teleportation script */
    const char *args[] = {NULL};
    char *result = execute_teleport_script("initialize_qopu", args);
    if (!result) {
        return false;
    }
    
    /* Process the result */
    printf("%s\n", result);
    free(result);
    
    /* Establish neural entanglement */
    const char *entangle_args[] = {"NeuralCortex", NULL};
    result = execute_teleport_script("establish_neural_entanglement", entangle_args);
    if (!result) {
        return false;
    }
    
    /* Process the result */
    printf("%s\n", result);
    free(result);
    
    /* Initialize the blink spot array */
    blink_spot_count = 0;
    memset(blink_spots, 0, sizeof(blink_spots));
    
    /* Initialize the last result */
    memset(&last_result, 0, sizeof(last_result));
    
    initialized = true;
    return true;
}

/**
 * @brief Create a new blink spot target
 */
BlinkSpotTarget *qteleport_create_blink_spot(const char *name, const char *description,
                                          double latitude, double longitude, double altitude,
                                          NodeLevel resonance_level) {
    if (!initialized || blink_spot_count >= MAX_BLINK_SPOTS) {
        return NULL;
    }
    
    /* Create a new blink spot target */
    BlinkSpotTarget *target = (BlinkSpotTarget *)malloc(sizeof(BlinkSpotTarget));
    if (!target) {
        return NULL;
    }
    
    /* Initialize the target */
    target->id = (uint64_t)time(NULL) + blink_spot_count; /* Unique ID based on time */
    target->name = strdup(name);
    target->description = strdup(description);
    target->latitude = latitude;
    target->longitude = longitude;
    target->altitude = altitude;
    target->creation_time = time(NULL);
    target->resonance_level = resonance_level;
    target->satellite_imagery = NULL;
    target->imagery_size = 0;
    target->is_favorite = false;
    target->stability = 0.95; /* Initial stability */
    
    /* Add to the blink spots array */
    blink_spots[blink_spot_count++] = target;
    
    /* Create via script */
    char lat_str[32], lon_str[32], alt_str[32];
    sprintf(lat_str, "%f", latitude);
    sprintf(lon_str, "%f", longitude);
    sprintf(alt_str, "%f", altitude);
    
    const char *args[] = {name, lat_str, lon_str, alt_str, description, NULL};
    char *result = execute_teleport_script("create_blink_spot", args);
    
    if (!result) {
        /* Script execution failed, but we'll keep the blink spot in memory */
        printf("Warning: Failed to execute create_blink_spot script\n");
    } else {
        /* Process the result */
        printf("%s\n", result);
        free(result);
    }
    
    return target;
}

/**
 * @brief List all available blink spot targets
 */
BlinkSpotTarget **qteleport_list_blink_spots(uint32_t *count) {
    if (!initialized || !count) {
        return NULL;
    }
    
    /* Execute the script to list blink spots */
    char *result = execute_teleport_script("list_blink_spots", NULL);
    if (result) {
        printf("%s\n", result);
        free(result);
    }
    
    /* Create a copy of the blink spots array for the caller */
    BlinkSpotTarget **targets = (BlinkSpotTarget **)malloc(sizeof(BlinkSpotTarget *) * blink_spot_count);
    if (!targets) {
        *count = 0;
        return NULL;
    }
    
    /* Copy the pointers */
    for (uint32_t i = 0; i < blink_spot_count; i++) {
        targets[i] = blink_spots[i];
    }
    
    *count = blink_spot_count;
    return targets;
}

/**
 * @brief Find blink spot targets by criteria
 */
BlinkSpotTarget **qteleport_find_blink_spots(const char *search_term,
                                          double near_latitude, double near_longitude,
                                          double radius_km, bool favorites_only,
                                          uint32_t *count) {
    if (!initialized || !count) {
        return NULL;
    }
    
    /* Count matching blink spots */
    uint32_t matches = 0;
    for (uint32_t i = 0; i < blink_spot_count; i++) {
        bool matches_search = true;
        bool matches_location = true;
        bool matches_favorite = true;
        
        /* Check search term */
        if (search_term && strlen(search_term) > 0) {
            matches_search = (strstr(blink_spots[i]->name, search_term) != NULL ||
                             strstr(blink_spots[i]->description, search_term) != NULL);
        }
        
        /* Check location */
        if (near_latitude != 0.0 && near_longitude != 0.0 && radius_km > 0.0) {
            double distance = calculate_distance(near_latitude, near_longitude,
                                               blink_spots[i]->latitude, blink_spots[i]->longitude);
            matches_location = (distance <= radius_km);
        }
        
        /* Check favorite */
        if (favorites_only) {
            matches_favorite = blink_spots[i]->is_favorite;
        }
        
        /* Count if all criteria match */
        if (matches_search && matches_location && matches_favorite) {
            matches++;
        }
    }
    
    /* Allocate result array */
    BlinkSpotTarget **results = (BlinkSpotTarget **)malloc(sizeof(BlinkSpotTarget *) * matches);
    if (!results) {
        *count = 0;
        return NULL;
    }
    
    /* Fill the results array */
    uint32_t result_index = 0;
    for (uint32_t i = 0; i < blink_spot_count; i++) {
        bool matches_search = true;
        bool matches_location = true;
        bool matches_favorite = true;
        
        /* Check search term */
        if (search_term && strlen(search_term) > 0) {
            matches_search = (strstr(blink_spots[i]->name, search_term) != NULL ||
                             strstr(blink_spots[i]->description, search_term) != NULL);
        }
        
        /* Check location */
        if (near_latitude != 0.0 && near_longitude != 0.0 && radius_km > 0.0) {
            double distance = calculate_distance(near_latitude, near_longitude,
                                               blink_spots[i]->latitude, blink_spots[i]->longitude);
            matches_location = (distance <= radius_km);
        }
        
        /* Check favorite */
        if (favorites_only) {
            matches_favorite = blink_spots[i]->is_favorite;
        }
        
        /* Add to results if all criteria match */
        if (matches_search && matches_location && matches_favorite) {
            results[result_index++] = blink_spots[i];
        }
    }
    
    *count = matches;
    return results;
}

/**
 * @brief Teleport to a blink spot target
 */
TeleportResult qteleport_to_blink_spot(uint64_t target_id, TeleportSettings settings) {
    TeleportResult result = {0};
    
    if (!initialized) {
        result.success = false;
        result.error_message = strdup("Teleportation system not initialized");
        memcpy(&last_result, &result, sizeof(result));
        return result;
    }
    
    /* Find the target blink spot */
    BlinkSpotTarget *target = find_blink_spot_by_id(target_id);
    if (!target) {
        result.success = false;
        result.error_message = strdup("Invalid blink spot target ID");
        memcpy(&last_result, &result, sizeof(result));
        return result;
    }
    
    /* Set up the source and destination */
    result.source = NULL; /* Current location */
    result.destination = target;
    
    /* Calculate energy and duration */
    result.energy_used = calculate_energy_cost(NULL, target, settings.method, settings.speed_factor);
    result.duration = calculate_duration(NULL, target, settings.method, settings.speed_factor);
    
    /* Check energy limit */
    if (settings.energy_limit > 0.0 && result.energy_used > settings.energy_limit) {
        result.success = false;
        result.error_message = strdup("Energy limit exceeded");
        memcpy(&last_result, &result, sizeof(result));
        return result;
    }
    
    /* Execute the teleportation via script */
    char id_str[32];
    sprintf(id_str, "%lu", target_id % blink_spot_count); /* Convert to script's index */
    
    const char *args[] = {id_str, NULL};
    char *script_result = execute_teleport_script("teleport_to_blink_spot", args);
    
    if (!script_result) {
        result.success = false;
        result.error_message = strdup("Teleportation script execution failed");
        memcpy(&last_result, &result, sizeof(result));
        return result;
    }
    
    /* Process the result */
    printf("%s\n", script_result);
    free(script_result);
    
    /* Set teleportation results */
    result.success = true;
    result.stability = target->stability;
    result.error_message = NULL;
    
    /* Simulate the teleportation visually */
    switch (settings.visual_effect) {
        case EFFECT_FADE:
            printf("Visual Effect: Fading from current location to %s...\n", target->name);
            break;
        case EFFECT_DISSOLVE:
            printf("Visual Effect: Dissolving into particles...\n");
            break;
        case EFFECT_BEAM:
            printf("Visual Effect: Energy beam teleportation...\n");
            break;
        case EFFECT_PORTAL:
            printf("Visual Effect: Portal opening to %s...\n", target->name);
            break;
        case EFFECT_QUANTUM:
            printf("Visual Effect: Quantum superposition shift...\n");
            break;
        default:
            /* No visual effect */
            break;
    }
    
    /* Simulate teleportation time based on duration */
    if (result.duration > 0.1) {
        printf("Teleporting to %s in %.1f seconds...\n", target->name, result.duration);
        sleep((unsigned int)result.duration);
    }
    
    printf("Teleportation to %s complete! Energy used: %.1f units\n", 
           target->name, result.energy_used);
    
    /* Store the result */
    memcpy(&last_result, &result, sizeof(result));
    
    return result;
}

/**
 * @brief Teleport to specific coordinates
 */
TeleportResult qteleport_to_coordinates(double latitude, double longitude, 
                                      double altitude, TeleportSettings settings) {
    /* Create a temporary blink spot for the coordinates */
    char name[64], description[128];
    sprintf(name, "Temporary Location (%.4f, %.4f)", latitude, longitude);
    sprintf(description, "Ad-hoc teleportation target created at %.4f, %.4f, %.1fm", 
            latitude, longitude, altitude);
    
    BlinkSpotTarget *target = qteleport_create_blink_spot(name, description, 
                                                      latitude, longitude, altitude,
                                                      settings.resonance_level);
    
    if (!target) {
        TeleportResult result = {0};
        result.success = false;
        result.error_message = strdup("Failed to create temporary blink spot");
        memcpy(&last_result, &result, sizeof(result));
        return result;
    }
    
    /* Teleport to the created blink spot */
    TeleportResult result = qteleport_to_blink_spot(target->id, settings);
    
    return result;
}

/**
 * @brief Mark a blink spot as favorite
 */
bool qteleport_set_favorite(uint64_t target_id, bool is_favorite) {
    if (!initialized) {
        return false;
    }
    
    /* Find the target blink spot */
    BlinkSpotTarget *target = find_blink_spot_by_id(target_id);
    if (!target) {
        return false;
    }
    
    /* Update the favorite status */
    target->is_favorite = is_favorite;
    
    return true;
}

/**
 * @brief Update a blink spot's information
 */
bool qteleport_update_blink_spot(uint64_t target_id, const char *name,
                                const char *description, int resonance_level) {
    if (!initialized) {
        return false;
    }
    
    /* Find the target blink spot */
    BlinkSpotTarget *target = find_blink_spot_by_id(target_id);
    if (!target) {
        return false;
    }
    
    /* Update the name if provided */
    if (name) {
        free(target->name);
        target->name = strdup(name);
    }
    
    /* Update the description if provided */
    if (description) {
        free(target->description);
        target->description = strdup(description);
    }
    
    /* Update the resonance level if provided */
    if (resonance_level >= 0) {
        target->resonance_level = (NodeLevel)resonance_level;
    }
    
    return true;
}

/**
 * @brief Delete a blink spot target
 */
bool qteleport_delete_blink_spot(uint64_t target_id) {
    if (!initialized) {
        return false;
    }
    
    /* Find the target blink spot */
    BlinkSpotTarget *target = NULL;
    uint32_t target_index = 0;
    bool found = false;
    
    for (uint32_t i = 0; i < blink_spot_count; i++) {
        if (blink_spots[i]->id == target_id) {
            target = blink_spots[i];
            target_index = i;
            found = true;
            break;
        }
    }
    
    if (!found) {
        return false;
    }
    
    /* Free the target resources */
    free(target->name);
    free(target->description);
    if (target->error_message) {
        free(target->error_message);
    }
    if (target->satellite_imagery) {
        free(target->satellite_imagery);
    }
    free(target);
    
    /* Remove from the array by shifting elements */
    for (uint32_t i = target_index; i < blink_spot_count - 1; i++) {
        blink_spots[i] = blink_spots[i + 1];
    }
    
    blink_spot_count--;
    
    return true;
}

/**
 * @brief Get the default teleportation settings
 */
TeleportSettings qteleport_get_default_settings(void) {
    TeleportSettings settings = {
        .method = TELEPORT_STANDARD,
        .visual_effect = EFFECT_FADE,
        .energy_limit = 0.0, /* No limit */
        .speed_factor = 1.0,
        .maintain_consciousness = true,
        .create_return_anchor = true,
        .resonance_level = NODE_PORTAL_TECHNICIAN,
        .allow_time_shift = false,
        .auto_stabilize = true
    };
    
    return settings;
}

/**
 * @brief Get the last teleportation result
 */
TeleportResult qteleport_get_last_result(void) {
    return last_result;
}

/**
 * @brief Shutdown the quantum teleportation system
 */
bool qteleport_shutdown(void) {
    if (!initialized) {
        return false;
    }
    
    /* Free all blink spots */
    for (uint32_t i = 0; i < blink_spot_count; i++) {
        free(blink_spots[i]->name);
        free(blink_spots[i]->description);
        if (blink_spots[i]->error_message) {
            free(blink_spots[i]->error_message);
        }
        if (blink_spots[i]->satellite_imagery) {
            free(blink_spots[i]->satellite_imagery);
        }
        free(blink_spots[i]);
    }
    
    /* Free last result error message */
    if (last_result.error_message) {
        free(last_result.error_message);
    }
    
    /* Reset state */
    blink_spot_count = 0;
    memset(blink_spots, 0, sizeof(blink_spots));
    memset(&last_result, 0, sizeof(last_result));
    qopu_instance = NULL;
    initialized = false;
    
    printf("Quantum Teleportation System shutdown complete.\n");
    
    return true;
}