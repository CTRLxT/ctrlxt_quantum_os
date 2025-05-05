/**
 * @file quantum_integration_test.c
 * @brief Integration test for quantum components
 *
 * This file demonstrates the integration between various quantum components
 * including the Quantum Entanglement Manager, Portal Gun, QRE, and Memex
 * Knowledge Network.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "../src/quantum/entanglement/entanglement_manager.h"
#include "../src/quantum/portals/portal_gun.h"
#include "../src/qre/qre.h"
#include "../src/memex/knowledge/knowledge_network.h"

/**
 * @brief Test the Quantum Entanglement Manager
 * 
 * @return true if tests passed, false otherwise
 */
bool test_quantum_entanglement_manager() {
    printf("\n=== Testing Quantum Entanglement Manager ===\n");
    
    // Initialize QEM
    printf("Initializing Quantum Entanglement Manager...\n");
    bool init_success = qem_init(10); // Allow up to 10 entanglements
    printf("Initialization %s\n", init_success ? "successful" : "failed");
    if (!init_success) return false;
    
    // Create an entanglement
    printf("Creating process-to-process entanglement...\n");
    EntanglementId entanglement = qem_create_entanglement(
        ENTANGLE_PROCESS, 1001, 1002, 4); // 4 qubits
    printf("Entanglement ID: %llu\n", (unsigned long long)entanglement.id);
    printf("Entanglement active: %s\n", entanglement.is_active ? "yes" : "no");
    
    if (!entanglement.is_active) {
        qem_shutdown();
        return false;
    }
    
    // Sync the entanglement
    printf("Synchronizing entanglement...\n");
    bool sync_success = qem_sync_entanglement(entanglement.id);
    printf("Synchronization %s\n", sync_success ? "successful" : "failed");
    
    // Get entanglement info
    printf("Getting entanglement information...\n");
    EntanglementId info = qem_get_entanglement_info(entanglement.id);
    printf("Source ID: %llu, Target ID: %llu, Type: %d\n",
           (unsigned long long)info.source_id,
           (unsigned long long)info.target_id,
           info.type);
    
    // Destroy the entanglement
    printf("Destroying entanglement...\n");
    bool destroy_success = qem_destroy_entanglement(entanglement.id);
    printf("Destruction %s\n", destroy_success ? "successful" : "failed");
    
    // Shutdown QEM
    printf("Shutting down Quantum Entanglement Manager...\n");
    qem_shutdown();
    
    return sync_success && destroy_success;
}

/**
 * @brief Test the Portal Gun system
 * 
 * @return true if tests passed, false otherwise
 */
bool test_portal_gun() {
    printf("\n=== Testing Portal Gun System ===\n");
    
    // Initialize QEM first (required by Portal Gun)
    printf("Initializing Quantum Entanglement Manager...\n");
    bool qem_init_success = qem_init(20); // Allow up to 20 entanglements
    if (!qem_init_success) {
        printf("QEM initialization failed!\n");
        return false;
    }
    
    // Initialize Portal Gun
    printf("Initializing Portal Gun...\n");
    PortalGunSettings settings = {
        .default_type = PORTAL_SPATIAL,
        .default_stability = STABILITY_STABLE,
        .entry_color = COLOR_BLUE, // Use defined color profile
        .exit_color = COLOR_ORANGE,  // Use defined color profile
        .default_diameter = 2.0,
        .max_distance = 1000.0,
        .max_portals = 5,
        .auto_stabilize = true,
        .power_efficiency = 0.85,
        .resonance_level = NODE_PORTAL_TECHNICIAN  // Use defined node level
    };
    
    bool portal_init_success = portal_gun_init(settings, 1000); // User ID 1000
    printf("Portal Gun initialization %s\n", portal_init_success ? "successful" : "failed");
    
    if (!portal_init_success) {
        qem_shutdown();
        return false;
    }
    
    // Create a portal
    printf("Creating spatial portal...\n");
    
    PortalCoordinates entry = {1.0, 2.0, 3.0, 0.0, 0, {0}};
    PortalCoordinates exit = {4.0, 5.0, 6.0, 0.0, 0, {0}};
    
    Portal *portal = portal_gun_create_portal(PORTAL_SPATIAL, entry, exit, NULL);
    
    if (portal == NULL) {
        printf("Portal creation failed!\n");
        portal_gun_emergency_shutdown();
        qem_shutdown();
        return false;
    }
    
    printf("Portal created with ID: %llu\n", (unsigned long long)portal->id);
    printf("Portal stability: %d\n", portal->stability);
    
    // Check if travel is safe
    printf("Checking if portal travel is safe...\n");
    bool is_safe = portal_gun_is_travel_safe(portal->id);
    printf("Portal travel is %s\n", is_safe ? "safe" : "unsafe");
    
    // Modify portal
    printf("Modifying portal...\n");
    PortalCoordinates new_exit = {7.0, 8.0, 9.0, 0.0, 0, {0}};
    bool modify_success = portal_gun_modify_portal(portal->id, &new_exit, NULL, -1);
    printf("Portal modification %s\n", modify_success ? "successful" : "failed");
    
    // Get active portals
    printf("Getting active portals...\n");
    uint32_t count = 0;
    Portal *portals = portal_gun_get_active_portals(5, &count);
    printf("Active portals: %u\n", count);
    
    if (portals != NULL) {
        free(portals); // Clean up allocated memory
    }
    
    // Close portal
    printf("Closing portal...\n");
    bool close_success = portal_gun_close_portal(portal->id);
    printf("Portal closing %s\n", close_success ? "successful" : "failed");
    
    // Emergency shutdown
    printf("Performing emergency shutdown...\n");
    bool shutdown_success = portal_gun_emergency_shutdown();
    printf("Emergency shutdown %s\n", shutdown_success ? "successful" : "failed");
    
    // Shutdown QEM
    printf("Shutting down Quantum Entanglement Manager...\n");
    qem_shutdown();
    
    return modify_success && close_success && shutdown_success;
}

/**
 * @brief Test the Quantum Reality Engine
 * 
 * @return true if tests passed, false otherwise
 */
bool test_quantum_reality_engine() {
    printf("\n=== Testing Quantum Reality Engine ===\n");
    
    // Initialize QEM first (required by QRE)
    printf("Initializing Quantum Entanglement Manager...\n");
    bool qem_init_success = qem_init(30); // Allow up to 30 entanglements
    if (!qem_init_success) {
        printf("QEM initialization failed!\n");
        return false;
    }
    
    // Initialize QRE
    printf("Initializing Quantum Reality Engine...\n");
    bool qre_init_success = qre_init(REALITY_MIXED, DIM_3D, true);
    printf("QRE initialization %s\n", qre_init_success ? "successful" : "failed");
    
    if (!qre_init_success) {
        qem_shutdown();
        return false;
    }
    
    // Create a reality space
    printf("Creating quantum reality space...\n");
    RealitySpace space = qre_create_space(REALITY_QUANTUM, DIM_MULTI, true);
    printf("Reality space created with ID: %llu\n", (unsigned long long)space.id);
    
    if (space.id == 0) {
        printf("Reality space creation failed!\n");
        qre_shutdown();
        qem_shutdown();
        return false;
    }
    
    // Create a reality object
    printf("Creating quantum reality object...\n");
    
    // Sample geometry data
    float geometry[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0}; // Simple cube vertices
    
    // Sample material data
    float material[] = {1.0, 0.0, 0.0, 1.0}; // Red color (RGBA)
    
    RealityObject object = qre_create_object(
        space.id,
        "Quantum Cube",
        geometry, sizeof(geometry),
        material, sizeof(material),
        true, // Interactive
        true, // Use quantum
        0);   // No knowledge node yet
    
    printf("Reality object created with ID: %llu\n", (unsigned long long)object.id);
    
    // Sync space
    printf("Synchronizing quantum space...\n");
    bool sync_success = qre_sync_space(space.id);
    printf("Space synchronization %s\n", sync_success ? "successful" : "failed");
    
    // Render space
    printf("Rendering space...\n");
    char render_buffer[256];
    bool render_success = qre_render_space(space.id, render_buffer, sizeof(render_buffer));
    printf("Space rendering %s\n", render_success ? "successful" : "failed");
    
    if (render_success) {
        printf("Render output: %s\n", render_buffer);
    }
    
    // Shutdown QRE and QEM
    printf("Shutting down Quantum Reality Engine...\n");
    qre_shutdown();
    
    printf("Shutting down Quantum Entanglement Manager...\n");
    qem_shutdown();
    
    return sync_success && render_success;
}

/**
 * @brief Test the Memex Knowledge Network
 * 
 * @return true if tests passed, false otherwise
 */
bool test_memex_knowledge_network() {
    printf("\n=== Testing Memex Knowledge Network ===\n");
    
    // Initialize QEM first (required by Knowledge Network)
    printf("Initializing Quantum Entanglement Manager...\n");
    bool qem_init_success = qem_init(40); // Allow up to 40 entanglements
    if (!qem_init_success) {
        printf("QEM initialization failed!\n");
        return false;
    }
    
    // Initialize Knowledge Network
    printf("Initializing Memex Knowledge Network...\n");
    bool knowledge_init_success = memex_knowledge_init(true); // Use quantum
    printf("Knowledge Network initialization %s\n", knowledge_init_success ? "successful" : "failed");
    
    if (!knowledge_init_success) {
        qem_shutdown();
        return false;
    }
    
    // Create knowledge nodes
    printf("Creating knowledge nodes...\n");
    
    KnowledgeNode quantum_node = memex_knowledge_create_node(
        NODE_CONCEPT,
        "Quantum Entanglement",
        "A quantum phenomenon where particles become correlated in such a way that the quantum state of each particle cannot be described independently.",
        true); // Use quantum
    
    printf("Created 'Quantum Entanglement' node with ID: %llu\n", (unsigned long long)quantum_node.id);
    
    KnowledgeNode teleport_node = memex_knowledge_create_node(
        NODE_CONCEPT,
        "Quantum Teleportation",
        "A process by which the exact state of an atom or photon can be transmitted from one location to another, with the help of quantum entanglement.",
        true); // Use quantum
    
    printf("Created 'Quantum Teleportation' node with ID: %llu\n", (unsigned long long)teleport_node.id);
    
    KnowledgeNode portal_node = memex_knowledge_create_node(
        NODE_CONCEPT,
        "Portal Technology",
        "Technology that creates wormholes between different points in spacetime or between different realities in the multiverse.",
        true); // Use quantum
    
    printf("Created 'Portal Technology' node with ID: %llu\n", (unsigned long long)portal_node.id);
    
    // Create relations between nodes
    printf("Creating knowledge relations...\n");
    
    KnowledgeRelation relation1 = memex_knowledge_create_relation(
        RELATION_RELATED_TO,
        quantum_node.id,
        teleport_node.id,
        0.9, // Strong relation
        true); // Use quantum
    
    printf("Created relation between Quantum Entanglement and Quantum Teleportation with ID: %llu\n", 
           (unsigned long long)relation1.id);
    
    KnowledgeRelation relation2 = memex_knowledge_create_relation(
        RELATION_RELATED_TO,
        teleport_node.id,
        portal_node.id,
        0.7, // Medium-strong relation
        true); // Use quantum
    
    printf("Created relation between Quantum Teleportation and Portal Technology with ID: %llu\n", 
           (unsigned long long)relation2.id);
    
    // Find nodes by query
    printf("Searching for knowledge nodes...\n");
    
    uint32_t result_count = 0;
    KnowledgeNode *search_results = memex_knowledge_find_nodes(
        "quantum", // Search query
        10,         // Max results
        &result_count);
    
    printf("Found %u nodes containing 'quantum':\n", result_count);
    
    if (search_results != NULL) {
        for (uint32_t i = 0; i < result_count; i++) {
            printf(" - %s (ID: %llu)\n", 
                   search_results[i].name,
                   (unsigned long long)search_results[i].id);
        }
        
        free(search_results); // Clean up allocated memory
    }
    
    // Get related nodes
    printf("Getting nodes related to Quantum Entanglement...\n");
    
    result_count = 0;
    KnowledgeNode *related_results = memex_knowledge_get_related(
        quantum_node.id, // Source node
        -1,              // Any relation type
        10,              // Max results
        &result_count);
    
    printf("Found %u related nodes:\n", result_count);
    
    if (related_results != NULL) {
        for (uint32_t i = 0; i < result_count; i++) {
            printf(" - %s (ID: %llu)\n", 
                   related_results[i].name,
                   (unsigned long long)related_results[i].id);
        }
        
        free(related_results); // Clean up allocated memory
    }
    
    // Shutdown Knowledge Network and QEM
    printf("Shutting down Memex Knowledge Network...\n");
    memex_knowledge_shutdown();
    
    printf("Shutting down Quantum Entanglement Manager...\n");
    qem_shutdown();
    
    return (quantum_node.id != 0 && teleport_node.id != 0 && portal_node.id != 0 &&
            relation1.id != 0 && relation2.id != 0);
}

/**
 * @brief Integrated test combining all quantum components
 * 
 * @return true if tests passed, false otherwise
 */
bool test_integrated_quantum_system() {
    printf("\n=== Testing Integrated Quantum System ===\n");
    
    // Initialize all components
    printf("Initializing all quantum components...\n");
    
    // Clean up any previous state to ensure fresh initialization
    portal_gun_emergency_shutdown();
    qre_shutdown();
    memex_knowledge_shutdown();
    qem_shutdown();
    
    // 1. Initialize QEM (foundation for all other components)
    bool qem_init_success = qem_init(100); // Allow up to 100 entanglements
    if (!qem_init_success) {
        printf("QEM initialization failed!\n");
        return false;
    }
    
    printf("QEM initialization successful!\n");
    
    // 2. Initialize Portal Gun
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
    
    printf("Attempting to initialize Portal Gun...\n");
    bool portal_init_success = portal_gun_init(portal_settings, 1000); // User ID 1000
    if (!portal_init_success) {
        printf("Portal Gun initialization failed!\n");
        qem_shutdown();
        return false;
    }
    
    printf("Portal Gun initialization successful!\n");
    
    // 3. Initialize QRE
    bool qre_init_success = qre_init(REALITY_MIXED, DIM_3D, true);
    if (!qre_init_success) {
        printf("QRE initialization failed!\n");
        portal_gun_emergency_shutdown();
        qem_shutdown();
        return false;
    }
    
    // 4. Initialize Knowledge Network
    bool knowledge_init_success = memex_knowledge_init(true); // Use quantum
    if (!knowledge_init_success) {
        printf("Knowledge Network initialization failed!\n");
        qre_shutdown();
        portal_gun_emergency_shutdown();
        qem_shutdown();
        return false;
    }
    
    printf("All components initialized successfully.\n");
    
    // Create an integrated quantum system
    
    // 1. Create knowledge nodes for concept and real-world locations
    KnowledgeNode location1_node = memex_knowledge_create_node(
        NODE_ENTITY,
        "San Francisco",
        "City in California, USA",
        true);
    
    KnowledgeNode location2_node = memex_knowledge_create_node(
        NODE_ENTITY,
        "Tokyo",
        "Capital city of Japan",
        true);
    
    printf("Created location nodes: %s (ID: %llu) and %s (ID: %llu)\n",
           location1_node.name, (unsigned long long)location1_node.id,
           location2_node.name, (unsigned long long)location2_node.id);
    
    // 2. Create a reality space
    RealitySpace space = qre_create_space(REALITY_QUANTUM, DIM_3D, true);
    printf("Created quantum reality space with ID: %llu\n", (unsigned long long)space.id);
    
    // 3. Create a portal between the two locations
    PortalCoordinates sf_coords = {37.7749, -122.4194, 0.0, 0.0, 0, {0}}; // San Francisco
    PortalCoordinates tokyo_coords = {35.6762, 139.6503, 0.0, 0.0, 0, {0}}; // Tokyo
    
    Portal *portal = portal_gun_create_portal(PORTAL_SPATIAL, sf_coords, tokyo_coords, NULL);
    printf("Created portal from San Francisco to Tokyo with ID: %llu\n", (unsigned long long)portal->id);
    
    // 4. Create reality objects for the locations
    // Create simple geometry data for San Francisco
    float sf_geometry[] = {37.7, -122.4, 0.0, 37.8, -122.3, 0.1}; // Bounding box
    float sf_material[] = {0.0, 0.5, 1.0, 0.8}; // Blue-ish color (RGBA)
    
    RealityObject sf_object = qre_create_object(
        space.id,
        "San Francisco Object",
        sf_geometry, sizeof(sf_geometry),
        sf_material, sizeof(sf_material),
        true, // Interactive
        true, // Use quantum
        location1_node.id); // Link to knowledge node
    
    printf("Created reality object for San Francisco with ID: %llu\n", (unsigned long long)sf_object.id);
    
    // Create simple geometry data for Tokyo
    float tokyo_geometry[] = {35.6, 139.6, 0.0, 35.7, 139.7, 0.1}; // Bounding box
    float tokyo_material[] = {1.0, 0.3, 0.3, 0.8}; // Red-ish color (RGBA)
    
    RealityObject tokyo_object = qre_create_object(
        space.id,
        "Tokyo Object",
        tokyo_geometry, sizeof(tokyo_geometry),
        tokyo_material, sizeof(tokyo_material),
        true, // Interactive
        true, // Use quantum
        location2_node.id); // Link to knowledge node
    
    printf("Created reality object for Tokyo with ID: %llu\n", (unsigned long long)tokyo_object.id);
    
    // 5. Create knowledge relation between locations
    KnowledgeRelation location_relation = memex_knowledge_create_relation(
        RELATION_ENTANGLED,
        location1_node.id,
        location2_node.id,
        0.95, // Very strong relation
        true); // Use quantum
    
    printf("Created entangled relation between locations with ID: %llu\n", 
           (unsigned long long)location_relation.id);
    
    // 6. Synchronize the space across the quantum entanglement
    printf("Synchronizing quantum reality space...\n");
    bool sync_success = qre_sync_space(space.id);
    printf("Space synchronization %s\n", sync_success ? "successful" : "failed");
    
    // 7. Render the space
    char render_buffer[512];
    bool render_success = qre_render_space(space.id, render_buffer, sizeof(render_buffer));
    
    if (render_success) {
        printf("Rendered quantum reality: %s\n", render_buffer);
    }
    
    // Simulate quantum teleportation through the portal
    printf("\nSimulating quantum teleportation through the portal...\n");
    sleep(1); // Pause for effect
    
    printf("Initiating teleportation sequence...\n");
    sleep(1);
    
    printf("Portal travel safety check: %s\n", 
           portal_gun_is_travel_safe(portal->id) ? "SAFE" : "UNSAFE");
    sleep(1);
    
    printf("* Quantum entanglement stabilized\n");
    sleep(1);
    
    printf("* Coordinates locked\n");
    sleep(1);
    
    printf("* Engaging quantum tunneling\n");
    sleep(1);
    
    printf("* Quantum state transfer in progress... %d%%\n", 33);
    sleep(1);
    
    printf("* Quantum state transfer in progress... %d%%\n", 66);
    sleep(1);
    
    printf("* Quantum state transfer in progress... %d%%\n", 100);
    sleep(1);
    
    printf("* Reconstituting quantum signature\n");
    sleep(1);
    
    printf("* Resolving superposition\n");
    sleep(1);
    
    printf("\nTeleportation complete! Now in: Tokyo\n");
    
    // Clean up
    printf("\nCleaning up quantum systems...\n");
    
    // Close portal
    portal_gun_close_portal(portal->id);
    
    // Shutdown all components
    memex_knowledge_shutdown();
    qre_shutdown();
    portal_gun_emergency_shutdown();
    qem_shutdown();
    
    printf("All quantum systems shut down successfully.\n");
    
    return sync_success && render_success;
}

/**
 * @brief Main function
 * 
 * @return Exit code
 */
int main() {
    printf("\n=== CTRLxT OS: Quantum Component Integration Tests ===\n");
    
    // Run tests
    bool qem_test_passed = test_quantum_entanglement_manager();
    printf("Quantum Entanglement Manager Test: %s\n", qem_test_passed ? "PASSED" : "FAILED");
    
    bool portal_test_passed = test_portal_gun();
    printf("Portal Gun Test: %s\n", portal_test_passed ? "PASSED" : "FAILED");
    
    bool qre_test_passed = test_quantum_reality_engine();
    printf("Quantum Reality Engine Test: %s\n", qre_test_passed ? "PASSED" : "FAILED");
    
    bool knowledge_test_passed = test_memex_knowledge_network();
    printf("Memex Knowledge Network Test: %s\n", knowledge_test_passed ? "PASSED" : "FAILED");
    
    bool integrated_test_passed = test_integrated_quantum_system();
    printf("Integrated Quantum System Test: %s\n", integrated_test_passed ? "PASSED" : "FAILED");
    
    printf("\n=== Test Summary ===\n");
    printf("Quantum Entanglement Manager: %s\n", qem_test_passed ? "PASSED" : "FAILED");
    printf("Portal Gun: %s\n", portal_test_passed ? "PASSED" : "FAILED");
    printf("Quantum Reality Engine: %s\n", qre_test_passed ? "PASSED" : "FAILED");
    printf("Memex Knowledge Network: %s\n", knowledge_test_passed ? "PASSED" : "FAILED");
    printf("Integrated Quantum System: %s\n", integrated_test_passed ? "PASSED" : "FAILED");
    
    bool all_passed = qem_test_passed && portal_test_passed && 
                   qre_test_passed && knowledge_test_passed && 
                   integrated_test_passed;
                   
    printf("\nOverall result: %s\n\n", all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    
    return all_passed ? 0 : 1;
}