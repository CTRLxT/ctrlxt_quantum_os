/**
 * @file test_quantum_ocular.c
 * @brief Unit tests for the Quantum Ocular Processing Unit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../src/quantum/ocular/quantum_ocular.h"

/**
 * @brief Test initialization of the Q-OPU
 */
static void test_qopu_init(void) {
    printf("Testing qopu_init...\n");
    
    /* Create a basic configuration */
    OcularConfig config = {
        .composition = COMP_COSMIC_DUST,
        .processing_model = MODEL_BIO_QUANTUM,
        .interface = INTERFACE_NEURAL,
        .quantum_tunneling_enabled = true,
        .reality_mode = REALITY_EXISTING,
        .channels = NULL,
        .channel_count = 0,
        .blink_spots = NULL,
        .blink_spot_count = 0,
        .zero_point_frequency = 432.0,
        .teleportation_enabled = true,
        .current_audio_level = 1
    };
    
    /* Initialize the Q-OPU */
    bool result = qopu_init(config);
    assert(result == true);
    
    printf("qopu_init test passed!\n");
}

/**
 * @brief Test creating blink spots
 */
static void test_qopu_create_blink_spots(void) {
    printf("\nTesting qopu_create_blink_spot...\n");
    
    /* Create a few blink spots */
    BlinkSpot *spot1 = qopu_create_blink_spot("Home", 35.1495, -90.0489, 79.0, REALITY_EXISTING);
    assert(spot1 != NULL);
    assert(strcmp(spot1->name, "Home") == 0);
    assert(spot1->latitude == 35.1495);
    assert(spot1->longitude == -90.0489);
    assert(spot1->altitude == 79.0);
    assert(spot1->reality_mode == REALITY_EXISTING);
    
    BlinkSpot *spot2 = qopu_create_blink_spot("Mountain View", 37.3861, -122.0839, 32.0, REALITY_AUGMENTED);
    assert(spot2 != NULL);
    assert(strcmp(spot2->name, "Mountain View") == 0);
    assert(spot2->latitude == 37.3861);
    assert(spot2->longitude == -122.0839);
    assert(spot2->altitude == 32.0);
    assert(spot2->reality_mode == REALITY_AUGMENTED);
    
    printf("qopu_create_blink_spot tests passed!\n");
}

/**
 * @brief Test teleportation to a blink spot
 */
static void test_qopu_teleport(void) {
    printf("\nTesting qopu_teleport...\n");
    
    /* Teleport to the Mountain View blink spot (index 1) */
    bool result = qopu_teleport(1);
    assert(result == true);
    
    /* Try to teleport to an invalid blink spot */
    result = qopu_teleport(999);
    assert(result == false);
    
    printf("qopu_teleport tests passed!\n");
}

/**
 * @brief Test creating a wormhole between blink spots
 */
static void test_qopu_create_wormhole(void) {
    printf("\nTesting qopu_create_wormhole...\n");
    
    /* Create a wormhole between Home and Mountain View */
    Portal portal;
    bool result = qopu_create_wormhole(0, 1, &portal);
    assert(result == true);
    assert(portal.type == PORTAL_SPATIAL);
    assert(portal.stability == STABILITY_STABLE);
    assert(portal.entry.x == -90.0489); /* longitude */
    assert(portal.entry.y == 35.1495);  /* latitude */
    assert(portal.entry.z == 79.0);     /* altitude */
    assert(portal.exit.x == -122.0839); /* longitude */
    assert(portal.exit.y == 37.3861);   /* latitude */
    assert(portal.exit.z == 32.0);      /* altitude */
    
    printf("qopu_create_wormhole test passed!\n");
}

/**
 * @brief Test upgrading quantum audio
 */
static void test_qopu_upgrade_audio(void) {
    printf("\nTesting qopu_upgrade_audio...\n");
    
    /* Upgrade to level 3 */
    bool result = qopu_upgrade_audio(3);
    assert(result == true);
    
    /* Try an invalid level */
    result = qopu_upgrade_audio(10);
    assert(result == false);
    
    printf("qopu_upgrade_audio tests passed!\n");
}

/**
 * @brief Test getting quantum data
 */
static void test_qopu_get_quantum_data(void) {
    printf("\nTesting qopu_get_quantum_data...\n");
    
    /* Test getting data from various sources */
    char buffer[1024];
    int32_t result;
    
    /* Get GPS data */
    result = qopu_get_quantum_data("QuantumGPS", buffer, sizeof(buffer));
    assert(result > 0);
    printf("GPS Data: %s\n", buffer);
    
    /* Get device metrics */
    result = qopu_get_quantum_data("QuantumDeviceMetrics", buffer, sizeof(buffer));
    assert(result > 0);
    printf("Device Metrics: %s\n", buffer);
    
    /* Get sleep frequency */
    result = qopu_get_quantum_data("QuantumSleepFrequency", buffer, sizeof(buffer));
    assert(result > 0);
    printf("Sleep Frequency: %s\n", buffer);
    
    printf("qopu_get_quantum_data tests passed!\n");
}

/**
 * @brief Test setting reality mode
 */
static void test_qopu_set_reality_mode(void) {
    printf("\nTesting qopu_set_reality_mode...\n");
    
    /* Set various reality modes */
    bool result;
    
    result = qopu_set_reality_mode(REALITY_AUGMENTED);
    assert(result == true);
    
    result = qopu_set_reality_mode(REALITY_SIMULATED);
    assert(result == true);
    
    result = qopu_set_reality_mode(REALITY_ALTERNATIVE);
    assert(result == true);
    
    result = qopu_set_reality_mode(REALITY_QUANTUM_SUPERPOSITION);
    assert(result == true);
    
    printf("qopu_set_reality_mode tests passed!\n");
}

/**
 * @brief Test shutdown of the Q-OPU
 */
static void test_qopu_shutdown(void) {
    printf("\nTesting qopu_shutdown...\n");
    
    /* Shut down the Q-OPU */
    bool result = qopu_shutdown();
    assert(result == true);
    
    printf("qopu_shutdown test passed!\n");
}

/**
 * @brief Main test function
 */
int main(void) {
    printf("Running Quantum Ocular Processing Unit tests...\n\n");
    
    test_qopu_init();
    test_qopu_create_blink_spots();
    test_qopu_teleport();
    test_qopu_create_wormhole();
    test_qopu_upgrade_audio();
    test_qopu_get_quantum_data();
    test_qopu_set_reality_mode();
    test_qopu_shutdown();
    
    printf("\nAll Quantum Ocular Processing Unit tests passed!\n");
    
    return 0;
}