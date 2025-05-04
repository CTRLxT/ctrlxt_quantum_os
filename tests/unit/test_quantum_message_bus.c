/**
 * @file test_quantum_message_bus.c
 * @brief Unit tests for the Quantum Message Bus
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../src/quantum/messaging/quantum_message_bus.h"

/* Message handler for testing */
static int test_handler_called = 0;
static QMessage* last_received_message = NULL;

/**
 * @brief Test message handler
 */
static void test_message_handler(QMessage* message, void* context) {
    test_handler_called++;
    
    /* Copy the message for verification */
    if (last_received_message) {
        qbus_free_message(last_received_message);
    }
    
    /* Create a copy of the message */
    last_received_message = qbus_create_message(
        message->header.type,
        message->header.source,
        message->header.destination,
        message->data,
        message->header.data_size,
        message->header.priority,
        message->header.requires_response
    );
    
    /* Set additional header fields */
    last_received_message->header.response_to = message->header.response_to;
    
    printf("Test handler received message: Type=%u, Source=%u, Destination=%u\n",
           message->header.type, message->header.source, message->header.destination);
    
    /* If the message is a ping, respond with a pong */
    if (message->header.type == QMSG_PING && message->header.requires_response) {
        const char* pong_data = "PONG";
        QMessage* response = qbus_create_response(message, pong_data, strlen(pong_data) + 1,
                                               QMSG_PRIORITY_HIGH);
        
        /* Update message type to PONG */
        response->header.type = QMSG_PONG;
        
        qbus_send_message(response);
        qbus_free_message(response);
        
        printf("Test handler sent PONG response\n");
    }
    
    /* Access the context */
    if (context) {
        int* value = (int*)context;
        (*value)++;
    }
}

/**
 * @brief Test Quantum Message Bus initialization
 */
static void test_qbus_init(void) {
    printf("Testing qbus_init...\n");
    
    /* Initialize Quantum Message Bus */
    bool result = qbus_init();
    assert(result == true);
    
    printf("qbus_init test passed!\n");
}

/**
 * @brief Test component registration and finding
 */
static void test_component_registration(void) {
    printf("\nTesting component registration...\n");
    
    /* Register a component */
    QComponentInfo comp_info = {
        .id = QCOMP_TELEPORT,
        .name = "Teleportation System",
        .resonance_level = NODE_PORTAL_TECHNICIAN,
        .context = NULL
    };
    
    bool result = qbus_register_component(&comp_info);
    assert(result == true);
    
    /* Find the component */
    QComponentInfo found_info;
    result = qbus_find_component(QCOMP_TELEPORT, &found_info);
    assert(result == true);
    assert(found_info.id == QCOMP_TELEPORT);
    assert(strcmp(found_info.name, "Teleportation System") == 0);
    assert(found_info.resonance_level == NODE_PORTAL_TECHNICIAN);
    
    /* Register another component */
    QComponentInfo comp_info2 = {
        .id = QCOMP_PORTAL_GUN,
        .name = "Portal Gun",
        .resonance_level = NODE_PORTAL_TECHNICIAN,
        .context = NULL
    };
    
    result = qbus_register_component(&comp_info2);
    assert(result == true);
    
    printf("Component registration test passed!\n");
}

/**
 * @brief Test message creation and sending
 */
static void test_message_creation(void) {
    printf("\nTesting message creation...\n");
    
    /* Create a message */
    const char* test_data = "Test Message Data";
    QMessage* message = qbus_create_message(
        QMSG_PING,
        QCOMP_TELEPORT,
        QCOMP_PORTAL_GUN,
        test_data,
        strlen(test_data) + 1,
        QMSG_PRIORITY_NORMAL,
        true
    );
    
    assert(message != NULL);
    assert(message->header.type == QMSG_PING);
    assert(message->header.source == QCOMP_TELEPORT);
    assert(message->header.destination == QCOMP_PORTAL_GUN);
    assert(message->header.priority == QMSG_PRIORITY_NORMAL);
    assert(message->header.requires_response == true);
    assert(message->header.response_to == 0);
    assert(message->header.data_size == strlen(test_data) + 1);
    assert(strcmp((char*)message->data, test_data) == 0);
    
    /* Free the message */
    qbus_free_message(message);
    
    printf("Message creation test passed!\n");
}

/**
 * @brief Test message subscription and delivery
 */
static void test_message_subscription(void) {
    printf("\nTesting message subscription and delivery...\n");
    
    /* Reset test state */
    test_handler_called = 0;
    if (last_received_message) {
        qbus_free_message(last_received_message);
        last_received_message = NULL;
    }
    
    /* Create a context value for the handler */
    int context_value = 0;
    
    /* Subscribe to PING messages */
    QSubscription subscription = {
        .component_id = QCOMP_PORTAL_GUN,
        .message_type = QMSG_PING,
        .handler = test_message_handler,
        .context = &context_value,
        .min_resonance = NODE_ZERO_POINT
    };
    
    bool result = qbus_subscribe(&subscription);
    assert(result == true);
    
    /* Create and send a message */
    const char* test_data = "PING";
    QMessage* message = qbus_create_message(
        QMSG_PING,
        QCOMP_TELEPORT,
        QCOMP_PORTAL_GUN,
        test_data,
        strlen(test_data) + 1,
        QMSG_PRIORITY_HIGH,
        true
    );
    
    result = qbus_send_message(message);
    assert(result == true);
    
    /* Process messages */
    uint32_t processed = qbus_process_messages(1);
    assert(processed == 1);
    
    /* Verify handler was called */
    assert(test_handler_called == 1);
    assert(context_value == 1);
    assert(last_received_message != NULL);
    assert(last_received_message->header.type == QMSG_PING);
    assert(last_received_message->header.source == QCOMP_TELEPORT);
    assert(last_received_message->header.destination == QCOMP_PORTAL_GUN);
    assert(strcmp((char*)last_received_message->data, test_data) == 0);
    
    /* Process the PONG response */
    processed = qbus_process_messages(1);
    assert(processed == 1);
    
    /* Free the message */
    qbus_free_message(message);
    
    printf("Message subscription and delivery test passed!\n");
}

/**
 * @brief Test component unsubscription
 */
static void test_unsubscription(void) {
    printf("\nTesting component unsubscription...\n");
    
    /* Reset test state */
    test_handler_called = 0;
    if (last_received_message) {
        qbus_free_message(last_received_message);
        last_received_message = NULL;
    }
    
    /* Unsubscribe from PING messages */
    bool result = qbus_unsubscribe(QCOMP_PORTAL_GUN, QMSG_PING, test_message_handler);
    assert(result == true);
    
    /* Create and send a message */
    const char* test_data = "PING AGAIN";
    QMessage* message = qbus_create_message(
        QMSG_PING,
        QCOMP_TELEPORT,
        QCOMP_PORTAL_GUN,
        test_data,
        strlen(test_data) + 1,
        QMSG_PRIORITY_NORMAL,
        false
    );
    
    result = qbus_send_message(message);
    assert(result == true);
    
    /* Process messages */
    uint32_t processed = qbus_process_messages(1);
    assert(processed == 1);
    
    /* Verify handler was not called */
    assert(test_handler_called == 0);
    
    /* Free the message */
    qbus_free_message(message);
    
    printf("Component unsubscription test passed!\n");
}

/**
 * @brief Test broadcast messages
 */
static void test_broadcast_messages(void) {
    printf("\nTesting broadcast messages...\n");
    
    /* Reset test state */
    test_handler_called = 0;
    if (last_received_message) {
        qbus_free_message(last_received_message);
        last_received_message = NULL;
    }
    
    /* Subscribe to all messages */
    QSubscription subscription = {
        .component_id = QCOMP_PORTAL_GUN,
        .message_type = -1, /* All message types */
        .handler = test_message_handler,
        .context = NULL,
        .min_resonance = NODE_ZERO_POINT
    };
    
    bool result = qbus_subscribe(&subscription);
    assert(result == true);
    
    /* Create and send a broadcast message */
    const char* test_data = "BROADCAST";
    QMessage* message = qbus_create_message(
        QMSG_SYSTEM_STARTUP,
        QCOMP_TELEPORT,
        0, /* Broadcast (no specific destination) */
        test_data,
        strlen(test_data) + 1,
        QMSG_PRIORITY_CRITICAL,
        false
    );
    
    result = qbus_send_message(message);
    assert(result == true);
    
    /* Process messages */
    uint32_t processed = qbus_process_messages(1);
    assert(processed == 1);
    
    /* Verify handler was called */
    assert(test_handler_called == 1);
    assert(last_received_message != NULL);
    assert(last_received_message->header.type == QMSG_SYSTEM_STARTUP);
    assert(last_received_message->header.source == QCOMP_TELEPORT);
    assert(last_received_message->header.destination == 0);
    assert(strcmp((char*)last_received_message->data, test_data) == 0);
    
    /* Free the message */
    qbus_free_message(message);
    
    printf("Broadcast messages test passed!\n");
}

/**
 * @brief Test component unregistration
 */
static void test_component_unregistration(void) {
    printf("\nTesting component unregistration...\n");
    
    /* Unregister the Portal Gun component */
    bool result = qbus_unregister_component(QCOMP_PORTAL_GUN);
    assert(result == true);
    
    /* Verify component is no longer found */
    QComponentInfo found_info;
    result = qbus_find_component(QCOMP_PORTAL_GUN, &found_info);
    assert(result == false);
    
    /* Unregister the Teleportation System component */
    result = qbus_unregister_component(QCOMP_TELEPORT);
    assert(result == true);
    
    printf("Component unregistration test passed!\n");
}

/**
 * @brief Test bus entanglement
 */
static void test_bus_entanglement(void) {
    printf("\nTesting bus entanglement...\n");
    
    /* Create a bus entanglement */
    uint64_t remote_bus_id = 12345;
    uint64_t entanglement_id = qbus_create_entanglement(remote_bus_id, NODE_QUANTUM_GUARDIAN);
    
    assert(entanglement_id != 0);
    
    /* Break the entanglement */
    bool result = qbus_break_entanglement(entanglement_id);
    assert(result == true);
    
    /* Try to break it again (should fail) */
    result = qbus_break_entanglement(entanglement_id);
    assert(result == false);
    
    printf("Bus entanglement test passed!\n");
}

/**
 * @brief Test resonance level setting
 */
static void test_resonance_level(void) {
    printf("\nTesting component resonance level...\n");
    
    /* Register a component */
    QComponentInfo comp_info = {
        .id = QCOMP_REALITY_ENGINE,
        .name = "Unified Quantum Reality Engine",
        .resonance_level = NODE_MATRIX_ARCHITECT,
        .context = NULL
    };
    
    bool result = qbus_register_component(&comp_info);
    assert(result == true);
    
    /* Change resonance level */
    result = qbus_set_component_resonance(QCOMP_REALITY_ENGINE, NODE_SINGULARITY);
    assert(result == true);
    
    /* Verify level changed */
    QComponentInfo found_info;
    result = qbus_find_component(QCOMP_REALITY_ENGINE, &found_info);
    assert(result == true);
    assert(found_info.resonance_level == NODE_SINGULARITY);
    
    printf("Component resonance level test passed!\n");
}

/**
 * @brief Test Quantum Message Bus shutdown
 */
static void test_qbus_shutdown(void) {
    printf("\nTesting qbus_shutdown...\n");
    
    /* Free the last received message if any */
    if (last_received_message) {
        qbus_free_message(last_received_message);
        last_received_message = NULL;
    }
    
    /* Shutdown the bus */
    qbus_shutdown();
    
    printf("qbus_shutdown test passed!\n");
}

/**
 * @brief Main test function
 */
int main(void) {
    printf("Running Quantum Message Bus tests...\n\n");
    
    test_qbus_init();
    test_component_registration();
    test_message_creation();
    test_message_subscription();
    test_unsubscription();
    test_broadcast_messages();
    test_component_unregistration();
    test_bus_entanglement();
    test_resonance_level();
    test_qbus_shutdown();
    
    printf("\nAll Quantum Message Bus tests passed!\n");
    
    return 0;
}