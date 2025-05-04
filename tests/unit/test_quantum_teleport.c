/**
 * @file test_quantum_teleport.c
 * @brief Unit tests for the Quantum Teleportation System
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../src/quantum/teleport/quantum_teleport.h"
#include "../../src/quantum/ocular/quantum_ocular.h"

/**
 * @brief Test initialization of the Quantum Teleportation System
 */
static void test_qteleport_init(void) {
    printf("Testing qteleport_init...\n");
    
    /* Create a mock Q-OPU instance */
    void *mock_qopu = malloc(sizeof(int)); /* Just a placeholder */
    
    /* Initialize the teleportation system */
    bool result = qteleport_init(mock_qopu);
    assert(result == true);
    
    printf("qteleport_init test passed!\n");
}

/**
 * @brief Test creating blink spot targets
 */
static void test_qteleport_create_blink_spots(void) {
    printf("\nTesting qteleport_create_blink_spot...\n");
    
    /* Create some blink spots */
    BlinkSpotTarget *home = qteleport_create_blink_spot(
        "Home Base", "Primary residence and quantum anchor",
        35.1495, -90.0489, 79.0, NODE_ZERO_POINT);
    
    assert(home != NULL);
    assert(strcmp(home->name, "Home Base") == 0);
    assert(home->latitude == 35.1495);
    assert(home->longitude == -90.0489);
    assert(home->altitude == 79.0);
    assert(home->resonance_level == NODE_ZERO_POINT);
    
    BlinkSpotTarget *mountain = qteleport_create_blink_spot(
        "Mountain Peak", "High altitude meditation spot",
        36.5785, -118.2923, 4421.0, NODE_DIMENSIONAL_ANCHOR);
    
    assert(mountain != NULL);
    assert(strcmp(mountain->name, "Mountain Peak") == 0);
    assert(mountain->latitude == 36.5785);
    assert(mountain->longitude == -118.2923);
    assert(mountain->altitude == 4421.0);
    assert(mountain->resonance_level == NODE_DIMENSIONAL_ANCHOR);
    
    BlinkSpotTarget *beach = qteleport_create_blink_spot(
        "Tropical Beach", "Relaxation and rejuvenation location",
        20.7634, -156.4450, 3.0, NODE_PORTAL_TECHNICIAN);
    
    assert(beach != NULL);
    assert(strcmp(beach->name, "Tropical Beach") == 0);
    assert(beach->latitude == 20.7634);
    assert(beach->longitude == -156.4450);
    assert(beach->altitude == 3.0);
    assert(beach->resonance_level == NODE_PORTAL_TECHNICIAN);
    
    printf("qteleport_create_blink_spot tests passed!\n");
}

/**
 * @brief Test listing blink spot targets
 */
static void test_qteleport_list_blink_spots(void) {
    printf("\nTesting qteleport_list_blink_spots...\n");
    
    uint32_t count = 0;
    BlinkSpotTarget **spots = qteleport_list_blink_spots(&count);
    
    assert(spots != NULL);
    assert(count == 3); /* We created 3 spots in the previous test */
    
    /* Check that we got all our spots */
    bool found_home = false;
    bool found_mountain = false;
    bool found_beach = false;
    
    for (uint32_t i = 0; i < count; i++) {
        if (strcmp(spots[i]->name, "Home Base") == 0) {
            found_home = true;
        } else if (strcmp(spots[i]->name, "Mountain Peak") == 0) {
            found_mountain = true;
        } else if (strcmp(spots[i]->name, "Tropical Beach") == 0) {
            found_beach = true;
        }
    }
    
    assert(found_home);
    assert(found_mountain);
    assert(found_beach);
    
    /* Free the result array (but not the spots themselves) */
    free(spots);
    
    printf("qteleport_list_blink_spots test passed!\n");
}

/**
 * @brief Test finding blink spot targets
 */
static void test_qteleport_find_blink_spots(void) {
    printf("\nTesting qteleport_find_blink_spots...\n");
    
    /* First mark one as favorite */
    uint32_t count = 0;
    BlinkSpotTarget **all_spots = qteleport_list_blink_spots(&count);
    assert(count == 3);
    
    uint64_t beach_id = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (strcmp(all_spots[i]->name, "Tropical Beach") == 0) {
            beach_id = all_spots[i]->id;
            break;
        }
    }
    
    assert(beach_id != 0);
    bool fav_result = qteleport_set_favorite(beach_id, true);
    assert(fav_result == true);
    
    free(all_spots);
    
    /* Test search by name */
    uint32_t found_count = 0;
    BlinkSpotTarget **found = qteleport_find_blink_spots("Mountain", 0.0, 0.0, 0.0, false, &found_count);
    
    assert(found != NULL);
    assert(found_count == 1);
    assert(strcmp(found[0]->name, "Mountain Peak") == 0);
    
    free(found);
    
    /* Test search by favorites */
    found = qteleport_find_blink_spots(NULL, 0.0, 0.0, 0.0, true, &found_count);
    
    assert(found != NULL);
    assert(found_count == 1);
    assert(strcmp(found[0]->name, "Tropical Beach") == 0);
    
    free(found);
    
    /* Test search by location radius */
    found = qteleport_find_blink_spots(NULL, 36.0, -118.0, 100.0, false, &found_count);
    
    assert(found != NULL);
    assert(found_count == 1);
    assert(strcmp(found[0]->name, "Mountain Peak") == 0);
    
    free(found);
    
    printf("qteleport_find_blink_spots tests passed!\n");
}

/**
 * @brief Test teleportation to a blink spot
 */
static void test_qteleport_to_blink_spot(void) {
    printf("\nTesting qteleport_to_blink_spot...\n");
    
    /* Find a blink spot to teleport to */
    uint32_t count = 0;
    BlinkSpotTarget **spots = qteleport_list_blink_spots(&count);
    assert(count > 0);
    
    uint64_t target_id = spots[0]->id;
    free(spots);
    
    /* Get default settings */
    TeleportSettings settings = qteleport_get_default_settings();
    
    /* Customize settings for the test */
    settings.method = TELEPORT_STANDARD;
    settings.visual_effect = EFFECT_FADE;
    settings.energy_limit = 1000.0; /* High limit to ensure it succeeds */
    settings.speed_factor = 2.0; /* Faster teleportation */
    
    /* Perform the teleportation */
    TeleportResult result = qteleport_to_blink_spot(target_id, settings);
    
    /* Check the result */
    assert(result.success == true);
    assert(result.energy_used > 0.0);
    assert(result.duration > 0.0);
    assert(result.error_message == NULL);
    assert(result.destination != NULL);
    assert(result.destination->id == target_id);
    
    printf("qteleport_to_blink_spot test passed!\n");
}

/**
 * @brief Test teleportation to coordinates
 */
static void test_qteleport_to_coordinates(void) {
    printf("\nTesting qteleport_to_coordinates...\n");
    
    /* Get default settings */
    TeleportSettings settings = qteleport_get_default_settings();
    
    /* Customize settings for the test */
    settings.method = TELEPORT_INSTANT;
    settings.visual_effect = EFFECT_BEAM;
    settings.energy_limit = 2000.0; /* Higher limit for direct coordinates */
    settings.speed_factor = 1.5;
    settings.resonance_level = NODE_QUANTUM_GUARDIAN;
    
    /* Teleport to specific coordinates */
    TeleportResult result = qteleport_to_coordinates(40.7128, -74.0060, 10.0, settings);
    
    /* Check the result */
    assert(result.success == true);
    assert(result.energy_used > 0.0);
    assert(result.duration > 0.0);
    assert(result.error_message == NULL);
    assert(result.destination != NULL);
    assert(result.destination->latitude == 40.7128);
    assert(result.destination->longitude == -74.0060);
    assert(result.destination->altitude == 10.0);
    
    printf("qteleport_to_coordinates test passed!\n");
}

/**
 * @brief Test updating a blink spot
 */
static void test_qteleport_update_blink_spot(void) {
    printf("\nTesting qteleport_update_blink_spot...\n");
    
    /* Find a blink spot to update */
    uint32_t count = 0;
    BlinkSpotTarget **spots = qteleport_list_blink_spots(&count);
    assert(count > 0);
    
    uint64_t target_id = spots[0]->id;
    free(spots);
    
    /* Update the blink spot */
    bool result = qteleport_update_blink_spot(target_id, "Updated Location", 
                                         "Updated description for testing", NODE_DREAMER);
    
    assert(result == true);
    
    /* Verify the update */
    spots = qteleport_list_blink_spots(&count);
    bool found_updated = false;
    
    for (uint32_t i = 0; i < count; i++) {
        if (spots[i]->id == target_id) {
            assert(strcmp(spots[i]->name, "Updated Location") == 0);
            assert(strcmp(spots[i]->description, "Updated description for testing") == 0);
            assert(spots[i]->resonance_level == NODE_DREAMER);
            found_updated = true;
            break;
        }
    }
    
    assert(found_updated);
    free(spots);
    
    printf("qteleport_update_blink_spot test passed!\n");
}

/**
 * @brief Test deleting a blink spot
 */
static void test_qteleport_delete_blink_spot(void) {
    printf("\nTesting qteleport_delete_blink_spot...\n");
    
    /* Find a blink spot to delete */
    uint32_t count = 0;
    BlinkSpotTarget **spots = qteleport_list_blink_spots(&count);
    assert(count > 0);
    
    uint64_t target_id = spots[count - 1]->id; /* Delete the last one */
    free(spots);
    
    /* Delete the blink spot */
    bool result = qteleport_delete_blink_spot(target_id);
    assert(result == true);
    
    /* Verify it was deleted */
    spots = qteleport_list_blink_spots(&count);
    
    for (uint32_t i = 0; i < count; i++) {
        assert(spots[i]->id != target_id);
    }
    
    free(spots);
    
    printf("qteleport_delete_blink_spot test passed!\n");
}

/**
 * @brief Test shutdown of the Quantum Teleportation System
 */
static void test_qteleport_shutdown(void) {
    printf("\nTesting qteleport_shutdown...\n");
    
    /* Shut down the teleportation system */
    bool result = qteleport_shutdown();
    assert(result == true);
    
    printf("qteleport_shutdown test passed!\n");
}

/**
 * @brief Main test function
 */
int main(void) {
    printf("Running Quantum Teleportation System tests...\n\n");
    
    test_qteleport_init();
    test_qteleport_create_blink_spots();
    test_qteleport_list_blink_spots();
    test_qteleport_find_blink_spots();
    test_qteleport_to_blink_spot();
    test_qteleport_to_coordinates();
    test_qteleport_update_blink_spot();
    test_qteleport_delete_blink_spot();
    test_qteleport_shutdown();
    
    printf("\nAll Quantum Teleportation System tests passed!\n");
    
    return 0;
}