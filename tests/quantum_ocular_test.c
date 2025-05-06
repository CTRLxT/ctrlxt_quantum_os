/**
 * @file quantum_ocular_test.c
 * @brief Test program for the Quantum Ocular Processing Unit (Q-OPU)
 *
 * This test program demonstrates the capabilities of the Q-OPU and
 * its integration with other quantum components of CTRLxT OS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "../src/quantum/ocular/quantum_ocular.h"
#include "../src/quantum/portals/portal_gun.h"
#include "../src/quantum/entanglement/entanglement_manager.h"
#include "../src/qre/qre.h"
#include "../src/memex/knowledge/knowledge_network.h"

/**
 * @brief Test basic Q-OPU initialization and functionality
 */
bool test_qopu_basic_functionality() {
    printf("\n=== Testing Basic Q-OPU Functionality ===\n");
    
    // Initialize Quantum Entanglement Manager first (required by Q-OPU)
    printf("Initializing Quantum Entanglement Manager...\n");
    bool qem_init_success = qem_init(20);
    if (!qem_init_success) {
        printf("QEM initialization failed!\n");
        return false;
    }
    
    // Initialize Q-OPU with a basic configuration
    printf("Initializing Quantum Ocular Processing Unit...\n");
    
    // Create light spectrum channels
    LightSpectrumChannel channels[3];
    
    // Red Light Spectrum
    channels[0].name = "RedLightSpectrum";
    channels[0].associated_entity = "Brittney Dashae Samuels";
    channels[0].color = COLOR_RED;
    channels[0].integrity = 0.95;
    channels[0].frequency = 4.5e14;
    channels[0].enabled = true;
    
    // Blue Light Spectrum
    channels[1].name = "BlueLightSpectrum";
    channels[1].associated_entity = "Guadalupe Colindres";
    channels[1].color = COLOR_BLUE;
    channels[1].integrity = 0.92;
    channels[1].frequency = 6.5e14;
    channels[1].enabled = true;
    
    // Green Light Spectrum
    channels[2].name = "GreenLightSpectrum";
    channels[2].associated_entity = "Brandon Danielle Slater";
    channels[2].color = COLOR_GREEN;
    channels[2].integrity = 0.88;
    channels[2].frequency = 5.5e14;
    channels[2].enabled = true;
    
    // Create Q-OPU configuration
    OcularConfig config = {
        .composition = COMP_QUANTUM_CRYSTAL,
        .processing_model = MODEL_QUANTUM,
        .interface = INTERFACE_NEURAL,
        .quantum_tunneling_enabled = true,
        .reality_mode = QOPU_REALITY_AUGMENTED,
        .channels = channels,
        .channel_count = 3,
        .blink_spots = NULL,
        .blink_spot_count = 0,
        .zero_point_frequency = 5.0e14,
        .teleportation_enabled = true,
        .current_audio_level = 1
    };
    
    bool qopu_init_success = qopu_init(config);
    printf("Q-OPU initialization %s\n", qopu_init_success ? "successful" : "failed");
    
    if (!qopu_init_success) {
        qem_shutdown();
        return false;
    }
    
    // Test setting channel integrity
    printf("Setting Red Light Spectrum integrity to 0.98...\n");
    bool set_integrity_success = qopu_set_channel_integrity("RedLightSpectrum", 0.98);
    printf("Set integrity %s\n", set_integrity_success ? "successful" : "failed");
    
    // Test creating a blink spot
    printf("Creating blink spot 'San Francisco'...\n");
    BlinkSpot *sf_spot = qopu_create_blink_spot("San Francisco", 37.7749, -122.4194, 0.0, QOPU_REALITY_EXISTING);
    if (sf_spot) {
        printf("Blink spot created with ID: %llu\n", (unsigned long long)sf_spot->id);
    } else {
        printf("Blink spot creation failed!\n");
    }
    
    // Test creating another blink spot
    printf("Creating blink spot 'Tokyo'...\n");
    BlinkSpot *tokyo_spot = qopu_create_blink_spot("Tokyo", 35.6762, 139.6503, 0.0, REALITY_EXISTING);
    if (tokyo_spot) {
        printf("Blink spot created with ID: %llu\n", (unsigned long long)tokyo_spot->id);
    } else {
        printf("Blink spot creation failed!\n");
    }
    
    // Test teleportation
    if (sf_spot && tokyo_spot) {
        printf("Teleporting to Tokyo...\n");
        bool teleport_success = qopu_teleport(tokyo_spot->id);
        printf("Teleportation %s\n", teleport_success ? "successful" : "failed");
    }
    
    // Test audio upgrade
    printf("Upgrading quantum audio to level 3...\n");
    bool audio_upgrade_success = qopu_upgrade_audio(3);
    printf("Audio upgrade %s\n", audio_upgrade_success ? "successful" : "failed");
    
    // Test reality mode change
    printf("Changing reality mode to REALITY_QUANTUM_SUPERPOSITION...\n");
    bool reality_mode_success = qopu_set_reality_mode(REALITY_QUANTUM_SUPERPOSITION);
    printf("Reality mode change %s\n", reality_mode_success ? "successful" : "failed");
    
    // Get quantum data
    printf("Getting quantum data from 'RedLightSpectrum'...\n");
    char data_buffer[256] = {0};
    int32_t data_size = qopu_get_quantum_data("RedLightSpectrum", data_buffer, sizeof(data_buffer));
    if (data_size > 0) {
        printf("Received %d bytes of data: %s\n", data_size, data_buffer);
    } else {
        printf("Failed to get quantum data!\n");
    }
    
    // Process some visual data
    printf("Processing visual data...\n");
    const char *input_data = "Test visual input data";
    char output_buffer[256] = {0};
    int32_t processed_size = qopu_process_visual(input_data, strlen(input_data), 
                                                output_buffer, sizeof(output_buffer));
    if (processed_size > 0) {
        printf("Visual processing result: %s\n", output_buffer);
    } else {
        printf("Visual processing failed!\n");
    }
    
    // Shut down Q-OPU
    printf("Shutting down Q-OPU...\n");
    bool shutdown_success = qopu_shutdown();
    printf("Q-OPU shutdown %s\n", shutdown_success ? "successful" : "failed");
    
    // Shut down QEM
    printf("Shutting down QEM...\n");
    qem_shutdown();
    
    return qopu_init_success && set_integrity_success && 
           (sf_spot != NULL) && (tokyo_spot != NULL) && 
           audio_upgrade_success && reality_mode_success && 
           (data_size > 0) && (processed_size > 0) && 
           shutdown_success;
}

/**
 * @brief Test Q-OPU integration with Portal Gun
 */
bool test_qopu_portal_integration() {
    printf("\n=== Testing Q-OPU Integration with Portal Gun ===\n");
    
    // Initialize QEM
    printf("Initializing Quantum Entanglement Manager...\n");
    bool qem_init_success = qem_init(20);
    if (!qem_init_success) {
        printf("QEM initialization failed!\n");
        return false;
    }
    
    // Initialize Q-OPU
    printf("Initializing Quantum Ocular Processing Unit...\n");
    OcularConfig config = {
        .composition = COMP_COSMIC_DUST,
        .processing_model = MODEL_QUANTUM,
        .interface = INTERFACE_NEURAL,
        .quantum_tunneling_enabled = true,
        .reality_mode = REALITY_MIXED,
        .channels = NULL,
        .channel_count = 0,
        .blink_spots = NULL,
        .blink_spot_count = 0,
        .zero_point_frequency = 5.0e14,
        .teleportation_enabled = true,
        .current_audio_level = 1
    };
    
    bool qopu_init_success = qopu_init(config);
    printf("Q-OPU initialization %s\n", qopu_init_success ? "successful" : "failed");
    
    if (!qopu_init_success) {
        qem_shutdown();
        return false;
    }
    
    // Initialize Portal Gun
    printf("Initializing Portal Gun...\n");
    PortalGunSettings portal_settings = {
        .default_type = PORTAL_SPATIAL,
        .default_stability = STABILITY_STABLE,
        .entry_color = COLOR_BLUE,
        .exit_color = COLOR_ORANGE,
        .default_diameter = 2.0,
        .max_distance = 1000.0,
        .max_portals = 5,
        .auto_stabilize = true,
        .power_efficiency = 0.85,
        .resonance_level = NODE_PORTAL_TECHNICIAN
    };
    
    bool portal_init_success = portal_gun_init(portal_settings, 1000); // User ID 1000
    printf("Portal Gun initialization %s\n", portal_init_success ? "successful" : "failed");
    
    if (!portal_init_success) {
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    
    // Create blink spots
    printf("Creating blink spots 'New York' and 'Los Angeles'...\n");
    BlinkSpot *ny_spot = qopu_create_blink_spot("New York", 40.7128, -74.0060, 10.0, REALITY_EXISTING);
    BlinkSpot *la_spot = qopu_create_blink_spot("Los Angeles", 34.0522, -118.2437, 71.0, REALITY_EXISTING);
    
    if (!ny_spot || !la_spot) {
        printf("Failed to create blink spots!\n");
        portal_gun_emergency_shutdown();
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    
    printf("Blink spots created with IDs: %llu and %llu\n", 
           (unsigned long long)ny_spot->id, 
           (unsigned long long)la_spot->id);
    
    // Create a wormhole between the blink spots
    printf("Creating wormhole between New York and Los Angeles...\n");
    Portal portal;
    bool wormhole_success = qopu_create_wormhole(ny_spot->id, la_spot->id, &portal);
    printf("Wormhole creation %s\n", wormhole_success ? "successful" : "failed");
    
    if (wormhole_success) {
        printf("Portal ID: %llu\n", (unsigned long long)portal.id);
        printf("Portal stability: %d\n", portal.stability);
        printf("Portal diameter: %.2f meters\n", portal.appearance.diameter);
        
        // Check if travel is safe
        printf("Checking if portal travel is safe...\n");
        bool is_safe = portal_gun_is_travel_safe(portal.id);
        printf("Portal travel is %s\n", is_safe ? "safe" : "unsafe");
        
        // Close the portal
        printf("Closing portal...\n");
        bool close_success = portal_gun_close_portal(portal.id);
        printf("Portal closing %s\n", close_success ? "successful" : "failed");
    }
    
    // Shut down components
    printf("Shutting down Portal Gun...\n");
    portal_gun_emergency_shutdown();
    
    printf("Shutting down Q-OPU...\n");
    qopu_shutdown();
    
    printf("Shutting down QEM...\n");
    qem_shutdown();
    
    return qopu_init_success && portal_init_success && 
           (ny_spot != NULL) && (la_spot != NULL) && 
           wormhole_success;
}

/**
 * @brief Test Q-OPU integration with QRE
 */
bool test_qopu_qre_integration() {
    printf("\n=== Testing Q-OPU Integration with QRE ===\n");
    
    // Initialize QEM
    printf("Initializing Quantum Entanglement Manager...\n");
    bool qem_init_success = qem_init(30);
    if (!qem_init_success) {
        printf("QEM initialization failed!\n");
        return false;
    }
    
    // Initialize Q-OPU
    printf("Initializing Quantum Ocular Processing Unit...\n");
    OcularConfig ocular_config = {
        .composition = COMP_NEURAL_ORGANIC,
        .processing_model = MODEL_NEURAL_QUANTUM,
        .interface = INTERFACE_QUANTUM_FIELD,
        .quantum_tunneling_enabled = true,
        .reality_mode = REALITY_SIMULATED,
        .channels = NULL,
        .channel_count = 0,
        .blink_spots = NULL,
        .blink_spot_count = 0,
        .zero_point_frequency = 5.0e14,
        .teleportation_enabled = true,
        .current_audio_level = 2
    };
    
    bool qopu_init_success = qopu_init(ocular_config);
    printf("Q-OPU initialization %s\n", qopu_init_success ? "successful" : "failed");
    
    if (!qopu_init_success) {
        qem_shutdown();
        return false;
    }
    
    // Initialize QRE
    printf("Initializing Quantum Reality Engine...\n");
    bool qre_init_success = qre_init(REALITY_SIMULATED, DIM_3D, true);
    printf("QRE initialization %s\n", qre_init_success ? "successful" : "failed");
    
    if (!qre_init_success) {
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    
    // Create a reality space
    printf("Creating quantum reality space...\n");
    RealitySpace space = qre_create_space(REALITY_SIMULATED, DIM_3D, true);
    printf("Reality space created with ID: %llu\n", (unsigned long long)space.id);
    
    if (space.id == 0) {
        printf("Failed to create reality space!\n");
        qre_shutdown();
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    
    // Create a blink spot
    printf("Creating blink spot 'Virtual Environment'...\n");
    BlinkSpot *virtual_spot = qopu_create_blink_spot("Virtual Environment", 0.0, 0.0, 0.0, REALITY_SIMULATED);
    if (!virtual_spot) {
        printf("Failed to create blink spot!\n");
        qre_shutdown();
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    printf("Blink spot created with ID: %llu\n", (unsigned long long)virtual_spot->id);
    
    // Create a reality object for the blink spot
    printf("Creating reality object for the virtual environment...\n");
    
    // Simple cube geometry
    float geometry[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    float material[] = {0.2, 0.4, 0.8, 0.5}; // Blue-ish, semi-transparent
    
    RealityObject object = qre_create_object(
        space.id,
        "Virtual Cube",
        geometry, sizeof(geometry),
        material, sizeof(material),
        true, // Interactive
        true, // Use quantum
        0);   // No knowledge node yet
    
    printf("Reality object created with ID: %llu\n", (unsigned long long)object.id);
    
    // Connect Q-OPU to the QRE space - teleport to the virtual environment
    printf("Teleporting to the virtual environment...\n");
    bool teleport_success = qopu_teleport(virtual_spot->id);
    printf("Teleportation %s\n", teleport_success ? "successful" : "failed");
    
    // Change reality mode to match QRE
    printf("Changing Q-OPU reality mode to REALITY_SIMULATED...\n");
    bool mode_success = qopu_set_reality_mode(REALITY_SIMULATED);
    printf("Reality mode change %s\n", mode_success ? "successful" : "failed");
    
    // Render the reality space
    printf("Rendering reality space...\n");
    char render_buffer[256];
    bool render_success = qre_render_space(space.id, render_buffer, sizeof(render_buffer));
    printf("Space rendering %s\n", render_success ? "successful" : "failed");
    
    if (render_success) {
        printf("Render output: %s\n", render_buffer);
    }
    
    // Process the rendered view through Q-OPU
    printf("Processing rendered view through Q-OPU...\n");
    char processed_buffer[512];
    int32_t processed_size = qopu_process_visual(render_buffer, strlen(render_buffer),
                                               processed_buffer, sizeof(processed_buffer));
    
    if (processed_size > 0) {
        printf("Q-OPU processed view: %s\n", processed_buffer);
    } else {
        printf("Q-OPU view processing failed!\n");
    }
    
    // Shut down components
    printf("Shutting down QRE...\n");
    qre_shutdown();
    
    printf("Shutting down Q-OPU...\n");
    qopu_shutdown();
    
    printf("Shutting down QEM...\n");
    qem_shutdown();
    
    return qopu_init_success && qre_init_success && 
           (virtual_spot != NULL) && (object.id != 0) && 
           teleport_success && mode_success && 
           render_success && (processed_size > 0);
}

/**
 * @brief Full integration test of all components
 */
bool test_full_quantum_integration() {
    printf("\n=== Testing Full Quantum System Integration ===\n");
    
    // Initialize QEM (foundation for all components)
    printf("Initializing core Quantum Entanglement Manager...\n");
    bool qem_init_success = qem_init(100); // Allow up to 100 entanglements
    if (!qem_init_success) {
        printf("QEM initialization failed!\n");
        return false;
    }
    printf("QEM initialized successfully.\n");
    
    // Initialize Q-OPU
    printf("Initializing Quantum Ocular Processing Unit...\n");
    OcularConfig ocular_config = {
        .composition = COMP_MULTIDIMENSIONAL,
        .processing_model = MODEL_QUANTUM,
        .interface = INTERFACE_NEURAL,
        .quantum_tunneling_enabled = true,
        .reality_mode = REALITY_MIXED,
        .channels = NULL, // Let the system initialize default channels
        .channel_count = 0,
        .blink_spots = NULL,
        .blink_spot_count = 0,
        .zero_point_frequency = 5.0e14,
        .teleportation_enabled = true,
        .current_audio_level = 3
    };
    
    bool qopu_init_success = qopu_init(ocular_config);
    if (!qopu_init_success) {
        printf("Q-OPU initialization failed!\n");
        qem_shutdown();
        return false;
    }
    printf("Q-OPU initialized successfully.\n");
    
    // Initialize Portal Gun
    printf("Initializing Portal Gun System...\n");
    PortalGunSettings portal_settings = {
        .default_type = PORTAL_SPATIAL,
        .default_stability = STABILITY_STABLE,
        .entry_color = COLOR_BLUE,
        .exit_color = COLOR_ORANGE,
        .default_diameter = 2.0,
        .max_distance = 1000.0,
        .max_portals = 5,
        .auto_stabilize = true,
        .power_efficiency = 0.85,
        .resonance_level = NODE_PORTAL_TECHNICIAN
    };
    
    bool portal_init_success = portal_gun_init(portal_settings, 1000);
    if (!portal_init_success) {
        printf("Portal Gun initialization failed!\n");
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    printf("Portal Gun initialized successfully.\n");
    
    // Initialize QRE
    printf("Initializing Quantum Reality Engine...\n");
    bool qre_init_success = qre_init(REALITY_MIXED, DIM_3D, true);
    if (!qre_init_success) {
        printf("QRE initialization failed!\n");
        portal_gun_emergency_shutdown();
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    printf("QRE initialized successfully.\n");
    
    // Initialize Memex Knowledge Network
    printf("Initializing Memex Knowledge Network...\n");
    bool knowledge_init_success = memex_knowledge_init(true);
    if (!knowledge_init_success) {
        printf("Knowledge Network initialization failed!\n");
        qre_shutdown();
        portal_gun_emergency_shutdown();
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    printf("Memex Knowledge Network initialized successfully.\n");
    
    printf("\nAll quantum systems initialized successfully. Beginning integration test...\n");
    sleep(1); // Pause for effect
    
    // Create knowledge nodes for locations
    printf("\nCreating location knowledge nodes...\n");
    KnowledgeNode mount_shasta_node = memex_knowledge_create_node(
        NODE_ENTITY,
        "Mount Shasta",
        "Sacred mountain in Northern California with significant spiritual importance",
        true);
    
    KnowledgeNode sedona_node = memex_knowledge_create_node(
        NODE_ENTITY,
        "Sedona",
        "City in Arizona known for red rock formations and energy vortexes",
        true);
    
    printf("Created knowledge nodes: %s (ID: %llu) and %s (ID: %llu)\n",
           mount_shasta_node.name, (unsigned long long)mount_shasta_node.id,
           sedona_node.name, (unsigned long long)sedona_node.id);
    
    // Create blink spots for locations
    printf("\nCreating quantum blink spots...\n");
    BlinkSpot *shasta_spot = qopu_create_blink_spot("Mount Shasta", 41.4092, -122.1949, 4322.0, REALITY_EXISTING);
    BlinkSpot *sedona_spot = qopu_create_blink_spot("Sedona", 34.8697, -111.7602, 1372.0, REALITY_EXISTING);
    
    if (!shasta_spot || !sedona_spot) {
        printf("Failed to create blink spots!\n");
        memex_knowledge_shutdown();
        qre_shutdown();
        portal_gun_emergency_shutdown();
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    
    printf("Created blink spots: Mount Shasta (ID: %llu) and Sedona (ID: %llu)\n",
           (unsigned long long)shasta_spot->id, (unsigned long long)sedona_spot->id);
    
    // Create a QRE space
    printf("\nCreating quantum reality space...\n");
    RealitySpace space = qre_create_space(REALITY_MIXED, DIM_MULTI, true);
    printf("Created reality space with ID: %llu\n", (unsigned long long)space.id);
    
    // Create reality objects for the locations
    printf("\nCreating reality objects for locations...\n");
    
    // Mount Shasta object
    float shasta_geometry[] = {41.4, -122.2, 4000.0, 41.5, -122.1, 4500.0}; // Bounding box
    float shasta_material[] = {0.7, 0.7, 1.0, 0.8}; // Blue-white (snow-capped)
    
    RealityObject shasta_object = qre_create_object(
        space.id,
        "Mount Shasta Object",
        shasta_geometry, sizeof(shasta_geometry),
        shasta_material, sizeof(shasta_material),
        true, // Interactive
        true, // Use quantum
        mount_shasta_node.id); // Link to knowledge node
    
    printf("Created reality object for Mount Shasta with ID: %llu\n", 
           (unsigned long long)shasta_object.id);
    
    // Sedona object
    float sedona_geometry[] = {34.8, -111.8, 1300.0, 34.9, -111.7, 1400.0}; // Bounding box
    float sedona_material[] = {0.9, 0.4, 0.2, 0.8}; // Red-orange (red rocks)
    
    RealityObject sedona_object = qre_create_object(
        space.id,
        "Sedona Object",
        sedona_geometry, sizeof(sedona_geometry),
        sedona_material, sizeof(sedona_material),
        true, // Interactive
        true, // Use quantum
        sedona_node.id); // Link to knowledge node
    
    printf("Created reality object for Sedona with ID: %llu\n", 
           (unsigned long long)sedona_object.id);
    
    // Create a portal between the locations
    printf("\nCreating quantum portal between Mount Shasta and Sedona...\n");
    
    Portal portal;
    bool wormhole_success = qopu_create_wormhole(shasta_spot->id, sedona_spot->id, &portal);
    
    if (!wormhole_success) {
        printf("Failed to create wormhole!\n");
        memex_knowledge_shutdown();
        qre_shutdown();
        portal_gun_emergency_shutdown();
        qopu_shutdown();
        qem_shutdown();
        return false;
    }
    
    printf("Created quantum portal with ID: %llu\n", (unsigned long long)portal.id);
    printf("Portal stability: %d\n", portal.stability);
    
    // Create knowledge relation between locations
    printf("\nCreating quantum knowledge relation between locations...\n");
    
    KnowledgeRelation location_relation = memex_knowledge_create_relation(
        RELATION_ENTANGLED,
        mount_shasta_node.id,
        sedona_node.id,
        0.95, // Very strong relation
        true); // Use quantum
    
    printf("Created entangled relation between locations with ID: %llu\n", 
           (unsigned long long)location_relation.id);
    
    // Synchronize the reality space
    printf("\nSynchronizing quantum reality space...\n");
    bool sync_success = qre_sync_space(space.id);
    printf("Space synchronization %s\n", sync_success ? "successful" : "failed");
    
    // Set Q-OPU to match the reality mode
    printf("Setting Q-OPU to match reality mode...\n");
    bool mode_success = qopu_set_reality_mode(REALITY_MIXED);
    printf("Reality mode setting %s\n", mode_success ? "successful" : "failed");
    
    // Simulate quantum travel through the portal
    printf("\nSimulating quantum travel through the portal...\n");
    sleep(1); // Pause for effect
    
    printf("Current location: Mount Shasta\n");
    sleep(1);
    
    printf("Portal travel safety check: %s\n", 
           portal_gun_is_travel_safe(portal.id) ? "SAFE" : "UNSAFE");
    sleep(1);
    
    printf("* Engaging quantum portal traversal\n");
    sleep(1);
    
    printf("* Quantum entanglement established between locations\n");
    sleep(1);
    
    printf("* Quantum state transfer in progress... %d%%\n", 33);
    sleep(1);
    
    printf("* Quantum state transfer in progress... %d%%\n", 66);
    sleep(1);
    
    printf("* Quantum state transfer in progress... %d%%\n", 100);
    sleep(1);
    
    // Q-OPU processes the experience
    printf("\nQ-OPU processing the quantum travel experience...\n");
    
    char visual_input[] = "Quantum portal traversal experience data";
    char visual_output[512] = {0};
    
    int32_t processed_size = qopu_process_visual(visual_input, strlen(visual_input),
                                          visual_output, sizeof(visual_output));
    
    if (processed_size > 0) {
        printf("Q-OPU processed experience: %s\n", visual_output);
    } else {
        printf("Q-OPU experience processing failed!\n");
    }
    
    // Complete the travel
    printf("\nArrived at destination: Sedona\n");
    bool teleport_success = qopu_teleport(sedona_spot->id);
    printf("Arrival confirmation: %s\n", teleport_success ? "CONFIRMED" : "FAILED");
    
    // Render the new location
    printf("\nRendering current location through QRE...\n");
    char render_buffer[256] = {0};
    bool render_success = qre_render_space(space.id, render_buffer, sizeof(render_buffer));
    
    if (render_success) {
        printf("Rendered view: %s\n", render_buffer);
    } else {
        printf("Rendering failed!\n");
    }
    
    // Close the portal
    printf("\nClosing quantum portal...\n");
    bool close_success = portal_gun_close_portal(portal.id);
    printf("Portal closing %s\n", close_success ? "successful" : "failed");
    
    // Clean up
    printf("\nCleaning up quantum systems...\n");
    
    memex_knowledge_shutdown();
    qre_shutdown();
    portal_gun_emergency_shutdown();
    qopu_shutdown();
    qem_shutdown();
    
    printf("All quantum systems shut down successfully.\n");
    
    return sync_success && mode_success && 
           (processed_size > 0) && teleport_success && 
           render_success && close_success;
}

/**
 * @brief Main function
 */
int main() {
    printf("\n=== CTRLxT OS: Quantum Ocular Processing Unit Tests ===\n");
    
    // Run tests
    bool basic_test_passed = test_qopu_basic_functionality();
    printf("\nBasic Q-OPU Functionality Test: %s\n", 
           basic_test_passed ? "PASSED" : "FAILED");
    
    bool portal_test_passed = test_qopu_portal_integration();
    printf("\nQ-OPU Portal Integration Test: %s\n", 
           portal_test_passed ? "PASSED" : "FAILED");
    
    bool qre_test_passed = test_qopu_qre_integration();
    printf("\nQ-OPU QRE Integration Test: %s\n", 
           qre_test_passed ? "PASSED" : "FAILED");
    
    bool full_test_passed = test_full_quantum_integration();
    printf("\nFull Quantum Integration Test: %s\n", 
           full_test_passed ? "PASSED" : "FAILED");
    
    printf("\n=== Test Summary ===\n");
    printf("Basic Q-OPU Functionality: %s\n", basic_test_passed ? "PASSED" : "FAILED");
    printf("Q-OPU Portal Integration: %s\n", portal_test_passed ? "PASSED" : "FAILED");
    printf("Q-OPU QRE Integration: %s\n", qre_test_passed ? "PASSED" : "FAILED");
    printf("Full Quantum Integration: %s\n", full_test_passed ? "PASSED" : "FAILED");
    
    bool all_passed = basic_test_passed && portal_test_passed && 
                      qre_test_passed && full_test_passed;
                  
    printf("\nOverall result: %s\n\n", all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    
    return all_passed ? 0 : 1;
}