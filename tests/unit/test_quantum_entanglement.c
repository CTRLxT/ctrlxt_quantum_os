/**
 * @file test_quantum_entanglement.c
 * @brief Unit tests for the Quantum Entanglement Manager
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../../src/quantum/entanglement/entanglement_manager.h"

/**
 * @brief Test initialization of the Quantum Entanglement Manager
 */
static void test_qem_init(void) {
    printf("Testing qem_init...\n");
    
    // Test with valid parameters
    bool result = qem_init(100);
    assert(result == true);
    
    // Clean up
    qem_shutdown();
    
    // Test with invalid parameters
    result = qem_init(0);
    assert(result == false);
    
    printf("qem_init tests passed!\n");
}

/**
 * @brief Test entanglement creation
 */
static void test_qem_create_entanglement(void) {
    printf("Testing qem_create_entanglement...\n");
    
    // Initialize the QEM
    bool init_result = qem_init(100);
    assert(init_result == true);
    
    // Test process entanglement
    EntanglementId process_entanglement = qem_create_entanglement(
        ENTANGLE_PROCESS, 1001, 1002, 10);
    
    assert(process_entanglement.id != 0);
    assert(process_entanglement.type == ENTANGLE_PROCESS);
    assert(process_entanglement.source_id == 1001);
    assert(process_entanglement.target_id == 1002);
    assert(process_entanglement.qubit_count == 10);
    assert(process_entanglement.is_active == true);
    
    // Test device entanglement
    EntanglementId device_entanglement = qem_create_entanglement(
        ENTANGLE_DEVICE, 2001, 2002, 20);
    
    assert(device_entanglement.id != 0);
    assert(device_entanglement.type == ENTANGLE_DEVICE);
    assert(device_entanglement.source_id == 2001);
    assert(device_entanglement.target_id == 2002);
    assert(device_entanglement.qubit_count == 20);
    assert(device_entanglement.is_active == true);
    
    // Clean up
    qem_shutdown();
    
    printf("qem_create_entanglement tests passed!\n");
}

/**
 * @brief Test entanglement destruction
 */
static void test_qem_destroy_entanglement(void) {
    printf("Testing qem_destroy_entanglement...\n");
    
    // Initialize the QEM
    bool init_result = qem_init(100);
    assert(init_result == true);
    
    // Create an entanglement
    EntanglementId entanglement = qem_create_entanglement(
        ENTANGLE_PROCESS, 1001, 1002, 10);
    
    // Destroy the entanglement
    bool destroy_result = qem_destroy_entanglement(entanglement.id);
    assert(destroy_result == true);
    
    // Verify it's destroyed
    EntanglementId info = qem_get_entanglement_info(entanglement.id);
    assert(info.id == 0 || info.is_active == false);
    
    // Try to destroy a non-existent entanglement
    destroy_result = qem_destroy_entanglement(9999);
    assert(destroy_result == false);
    
    // Clean up
    qem_shutdown();
    
    printf("qem_destroy_entanglement tests passed!\n");
}

/**
 * @brief Test entanglement synchronization
 */
static void test_qem_sync_entanglement(void) {
    printf("Testing qem_sync_entanglement...\n");
    
    // Initialize the QEM
    bool init_result = qem_init(100);
    assert(init_result == true);
    
    // Create an entanglement
    EntanglementId entanglement = qem_create_entanglement(
        ENTANGLE_MEMORY, 3001, 3002, 10);
    
    // Synchronize the entanglement
    bool sync_result = qem_sync_entanglement(entanglement.id);
    assert(sync_result == true);
    
    // Try to synchronize a non-existent entanglement
    sync_result = qem_sync_entanglement(9999);
    assert(sync_result == false);
    
    // Clean up
    qem_shutdown();
    
    printf("qem_sync_entanglement tests passed!\n");
}

/**
 * @brief Main test function
 */
int main(void) {
    printf("Running Quantum Entanglement Manager tests...\n\n");
    
    test_qem_init();
    test_qem_create_entanglement();
    test_qem_destroy_entanglement();
    test_qem_sync_entanglement();
    
    printf("\nAll Quantum Entanglement Manager tests passed!\n");
    
    return 0;
}