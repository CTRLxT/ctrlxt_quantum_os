/**
 * @file quantum_message_bus.c
 * @brief Quantum Message Bus implementation
 */

#include "quantum_message_bus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../entanglement/entanglement_manager.h"

/* Maximum number of registered components */
#define MAX_COMPONENTS 64

/* Maximum number of subscriptions per component */
#define MAX_SUBSCRIPTIONS_PER_COMPONENT 32

/* Maximum number of remote bus entanglements */
#define MAX_BUS_ENTANGLEMENTS 16

/* Maximum queue size for pending messages */
#define MAX_PENDING_MESSAGES 1024

/* Message bus state */
static bool qbus_initialized = false;
static uint64_t next_message_id = 1;

/* Component tracking */
typedef struct {
    QComponentInfo info;
    bool registered;
    uint32_t subscription_count;
    QSubscription subscriptions[MAX_SUBSCRIPTIONS_PER_COMPONENT];
} ComponentEntry;

static ComponentEntry components[MAX_COMPONENTS];

/* Bus entanglement tracking */
typedef struct {
    uint64_t id;
    uint64_t remote_bus_id;
    NodeLevel resonance_level;
    bool is_synchronized;
} BusEntanglement;

static BusEntanglement bus_entanglements[MAX_BUS_ENTANGLEMENTS];

/* Pending message queue */
static QMessage* pending_messages[MAX_PENDING_MESSAGES];
static uint32_t pending_message_count = 0;

/**
 * @brief Get current timestamp in nanoseconds
 */
static uint64_t get_timestamp_ns(void) {
    /* In a real implementation, this would use a high-precision timer */
    /* For simulation, we'll just use time() and convert to nanoseconds */
    return (uint64_t)time(NULL) * 1000000000ULL;
}

/**
 * @brief Find a component entry by ID
 */
static ComponentEntry* find_component_entry(QComponentId component_id) {
    for (int i = 0; i < MAX_COMPONENTS; i++) {
        if (components[i].registered && components[i].info.id == component_id) {
            return &components[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Find a free component entry slot
 */
static ComponentEntry* find_free_component_slot(void) {
    for (int i = 0; i < MAX_COMPONENTS; i++) {
        if (!components[i].registered) {
            return &components[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Find a bus entanglement by ID
 */
static BusEntanglement* find_bus_entanglement(uint64_t entanglement_id) {
    for (int i = 0; i < MAX_BUS_ENTANGLEMENTS; i++) {
        if (bus_entanglements[i].id == entanglement_id) {
            return &bus_entanglements[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Find a free bus entanglement slot
 */
static BusEntanglement* find_free_entanglement_slot(void) {
    for (int i = 0; i < MAX_BUS_ENTANGLEMENTS; i++) {
        if (bus_entanglements[i].id == 0) {
            return &bus_entanglements[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Add a message to the pending queue
 */
static bool add_to_pending_queue(QMessage* message) {
    if (pending_message_count >= MAX_PENDING_MESSAGES) {
        return false;
    }
    
    /* Find insertion point based on priority */
    int insert_index = pending_message_count;
    
    /* Priority queue - higher priority messages go to the front */
    for (int i = 0; i < pending_message_count; i++) {
        if (message->header.priority > pending_messages[i]->header.priority) {
            insert_index = i;
            break;
        }
    }
    
    /* Shift messages to make room */
    for (int i = pending_message_count; i > insert_index; i--) {
        pending_messages[i] = pending_messages[i - 1];
    }
    
    /* Insert the message */
    pending_messages[insert_index] = message;
    pending_message_count++;
    
    return true;
}

/**
 * @brief Remove and return the next message from the pending queue
 */
static QMessage* remove_from_pending_queue(void) {
    if (pending_message_count == 0) {
        return NULL;
    }
    
    /* Get the next message (highest priority) */
    QMessage* message = pending_messages[0];
    
    /* Shift remaining messages */
    for (int i = 0; i < pending_message_count - 1; i++) {
        pending_messages[i] = pending_messages[i + 1];
    }
    
    pending_message_count--;
    return message;
}

/**
 * @brief Deliver a message to its destination(s)
 */
static bool deliver_message(const QMessage* message) {
    bool delivered = false;
    
    /* Get destination component */
    if (message->header.destination != 0) {
        /* Targeted message */
        ComponentEntry* dest = find_component_entry(message->header.destination);
        if (!dest) {
            printf("Cannot deliver message: destination component %u not found\n", message->header.destination);
            return false;
        }
        
        /* Check subscriptions */
        for (uint32_t i = 0; i < dest->subscription_count; i++) {
            QSubscription* sub = &dest->subscriptions[i];
            
            if ((sub->message_type == -1 || sub->message_type == message->header.type) &&
                sub->min_resonance <= message->header.resonance_level) {
                /* Call handler */
                if (sub->handler) {
                    /* Need to cast away const to match handler signature */
                    sub->handler((QMessage*)message, sub->context);
                    delivered = true;
                }
            }
        }
    } else {
        /* Broadcast message */
        for (int i = 0; i < MAX_COMPONENTS; i++) {
            if (components[i].registered) {
                ComponentEntry* comp = &components[i];
                
                /* Skip source component */
                if (comp->info.id == message->header.source) {
                    continue;
                }
                
                /* Check subscriptions */
                for (uint32_t j = 0; j < comp->subscription_count; j++) {
                    QSubscription* sub = &comp->subscriptions[j];
                    
                    if ((sub->message_type == -1 || sub->message_type == message->header.type) &&
                        sub->min_resonance <= message->header.resonance_level) {
                        /* Call handler */
                        if (sub->handler) {
                            /* Need to cast away const to match handler signature */
                            sub->handler((QMessage*)message, sub->context);
                            delivered = true;
                        }
                    }
                }
            }
        }
    }
    
    /* Propagate to entangled buses */
    /* In a real implementation, this would send the message to remote buses */
    
    return delivered;
}

/**
 * @brief Initialize the Quantum Message Bus
 */
bool qbus_init(void) {
    if (qbus_initialized) {
        return true;
    }
    
    /* Initialize component table */
    memset(components, 0, sizeof(components));
    
    /* Initialize bus entanglement table */
    memset(bus_entanglements, 0, sizeof(bus_entanglements));
    
    /* Initialize pending message queue */
    memset(pending_messages, 0, sizeof(pending_messages));
    pending_message_count = 0;
    
    qbus_initialized = true;
    printf("Quantum Message Bus initialized\n");
    
    return true;
}

/**
 * @brief Shutdown the Quantum Message Bus
 */
void qbus_shutdown(void) {
    if (!qbus_initialized) {
        return;
    }
    
    /* Free all pending messages */
    for (uint32_t i = 0; i < pending_message_count; i++) {
        qbus_free_message(pending_messages[i]);
    }
    
    /* Reset state */
    memset(components, 0, sizeof(components));
    memset(bus_entanglements, 0, sizeof(bus_entanglements));
    memset(pending_messages, 0, sizeof(pending_messages));
    pending_message_count = 0;
    
    qbus_initialized = false;
    printf("Quantum Message Bus shutdown complete\n");
}

/**
 * @brief Register a component with the message bus
 */
bool qbus_register_component(const QComponentInfo* info) {
    if (!qbus_initialized || !info) {
        return false;
    }
    
    /* Check if component ID is already registered */
    ComponentEntry* existing = find_component_entry(info->id);
    if (existing) {
        printf("Cannot register component: ID %u already in use\n", info->id);
        return false;
    }
    
    /* Find a free slot */
    ComponentEntry* slot = find_free_component_slot();
    if (!slot) {
        printf("Cannot register component: maximum components reached\n");
        return false;
    }
    
    /* Initialize the component entry */
    slot->info = *info;
    slot->registered = true;
    slot->subscription_count = 0;
    
    printf("Registered component: %s (ID: %u)\n", info->name, info->id);
    
    /* Create and send a registration notification message */
    QMessage* message = qbus_create_message(QMSG_COMPONENT_REGISTER, info->id, 0,
                                          &slot->info, sizeof(QComponentInfo),
                                          QMSG_PRIORITY_NORMAL, false);
    if (message) {
        qbus_send_message(message);
        qbus_free_message(message);
    }
    
    return true;
}

/**
 * @brief Unregister a component from the message bus
 */
bool qbus_unregister_component(QComponentId component_id) {
    if (!qbus_initialized) {
        return false;
    }
    
    /* Find the component */
    ComponentEntry* component = find_component_entry(component_id);
    if (!component) {
        return false;
    }
    
    /* Create and send an unregistration notification message */
    QMessage* message = qbus_create_message(QMSG_COMPONENT_UNREGISTER, component_id, 0,
                                          &component->info, sizeof(QComponentInfo),
                                          QMSG_PRIORITY_NORMAL, false);
    if (message) {
        qbus_send_message(message);
        qbus_free_message(message);
    }
    
    /* Mark as unregistered */
    component->registered = false;
    component->subscription_count = 0;
    
    printf("Unregistered component: %s (ID: %u)\n", component->info.name, component_id);
    
    return true;
}

/**
 * @brief Subscribe to message types
 */
bool qbus_subscribe(const QSubscription* subscription) {
    if (!qbus_initialized || !subscription) {
        return false;
    }
    
    /* Find the component */
    ComponentEntry* component = find_component_entry(subscription->component_id);
    if (!component) {
        printf("Cannot subscribe: component %u not found\n", subscription->component_id);
        return false;
    }
    
    /* Check if maximum subscriptions reached */
    if (component->subscription_count >= MAX_SUBSCRIPTIONS_PER_COMPONENT) {
        printf("Cannot subscribe: maximum subscriptions reached for component %u\n", subscription->component_id);
        return false;
    }
    
    /* Add the subscription */
    component->subscriptions[component->subscription_count] = *subscription;
    component->subscription_count++;
    
    return true;
}

/**
 * @brief Unsubscribe from message types
 */
bool qbus_unsubscribe(QComponentId component_id, QMessageType message_type, QMessageHandler handler) {
    if (!qbus_initialized) {
        return false;
    }
    
    /* Find the component */
    ComponentEntry* component = find_component_entry(component_id);
    if (!component) {
        return false;
    }
    
    bool found = false;
    
    /* Remove matching subscriptions */
    for (uint32_t i = 0; i < component->subscription_count; i++) {
        QSubscription* sub = &component->subscriptions[i];
        
        if ((message_type == -1 || sub->message_type == message_type) &&
            (handler == NULL || sub->handler == handler)) {
            /* Remove this subscription by shifting others down */
            for (uint32_t j = i; j < component->subscription_count - 1; j++) {
                component->subscriptions[j] = component->subscriptions[j + 1];
            }
            
            component->subscription_count--;
            i--; /* Recheck this index since we shifted */
            found = true;
        }
    }
    
    return found;
}

/**
 * @brief Send a message
 */
bool qbus_send_message(const QMessage* message) {
    if (!qbus_initialized || !message) {
        return false;
    }
    
    /* Create a copy of the message */
    QMessage* message_copy = qbus_create_message(
        message->header.type,
        message->header.source,
        message->header.destination,
        message->data,
        message->header.data_size,
        message->header.priority,
        message->header.requires_response
    );
    
    if (!message_copy) {
        return false;
    }
    
    /* Set response_to if this is a response */
    message_copy->header.response_to = message->header.response_to;
    
    /* Add to pending queue */
    if (!add_to_pending_queue(message_copy)) {
        qbus_free_message(message_copy);
        return false;
    }
    
    return true;
}

/**
 * @brief Create a new message
 */
QMessage* qbus_create_message(QMessageType type, QComponentId source, QComponentId destination,
                           const void* data, uint32_t data_size, QMessagePriority priority,
                           bool requires_response) {
    if (!qbus_initialized) {
        return NULL;
    }
    
    /* Validate source component */
    if (source != 0 && !find_component_entry(source)) {
        printf("Cannot create message: source component %u not registered\n", source);
        return NULL;
    }
    
    /* Allocate message structure */
    QMessage* message = (QMessage*)malloc(sizeof(QMessage));
    if (!message) {
        return NULL;
    }
    
    /* Initialize message header */
    message->header.message_id = next_message_id++;
    message->header.type = type;
    message->header.source = source;
    message->header.destination = destination;
    message->header.priority = priority;
    message->header.timestamp = get_timestamp_ns();
    message->header.requires_response = requires_response;
    message->header.response_to = 0;
    message->header.data_size = data_size;
    
    /* Set resonance level based on source component */
    if (source != 0) {
        ComponentEntry* source_comp = find_component_entry(source);
        message->header.resonance_level = source_comp->info.resonance_level;
    } else {
        message->header.resonance_level = NODE_ZERO_POINT;
    }
    
    /* Allocate and copy data if provided */
    if (data && data_size > 0) {
        message->data = malloc(data_size);
        if (!message->data) {
            free(message);
            return NULL;
        }
        
        memcpy(message->data, data, data_size);
    } else {
        message->data = NULL;
    }
    
    return message;
}

/**
 * @brief Create a response message
 */
QMessage* qbus_create_response(const QMessage* original_message, const void* data,
                             uint32_t data_size, QMessagePriority priority) {
    if (!qbus_initialized || !original_message) {
        return NULL;
    }
    
    /* Create a new message */
    QMessage* response = qbus_create_message(
        original_message->header.type,
        original_message->header.destination, /* Swap source and destination */
        original_message->header.source,       /* Swap source and destination */
        data,
        data_size,
        priority,
        false /* Response doesn't require another response */
    );
    
    if (!response) {
        return NULL;
    }
    
    /* Set response_to to the original message ID */
    response->header.response_to = original_message->header.message_id;
    
    return response;
}

/**
 * @brief Free a message
 */
void qbus_free_message(QMessage* message) {
    if (!message) {
        return;
    }
    
    /* Free data if present */
    if (message->data) {
        free(message->data);
    }
    
    /* Free message structure */
    free(message);
}

/**
 * @brief Process pending messages
 */
uint32_t qbus_process_messages(uint32_t max_messages) {
    if (!qbus_initialized) {
        return 0;
    }
    
    uint32_t processed = 0;
    uint32_t limit = (max_messages > 0) ? max_messages : pending_message_count;
    
    while (processed < limit && pending_message_count > 0) {
        /* Get the next message */
        QMessage* message = remove_from_pending_queue();
        if (!message) {
            break;
        }
        
        /* Deliver the message */
        deliver_message(message);
        
        /* Free the message */
        qbus_free_message(message);
        
        processed++;
    }
    
    return processed;
}

/**
 * @brief Find a component by ID
 */
bool qbus_find_component(QComponentId component_id, QComponentInfo* info) {
    if (!qbus_initialized || !info) {
        return false;
    }
    
    ComponentEntry* component = find_component_entry(component_id);
    if (!component) {
        return false;
    }
    
    /* Copy the component info */
    *info = component->info;
    
    return true;
}

/**
 * @brief Set a component's resonance level
 */
bool qbus_set_component_resonance(QComponentId component_id, NodeLevel resonance_level) {
    if (!qbus_initialized) {
        return false;
    }
    
    ComponentEntry* component = find_component_entry(component_id);
    if (!component) {
        return false;
    }
    
    /* Update the resonance level */
    component->info.resonance_level = resonance_level;
    
    return true;
}

/**
 * @brief Create a quantum entanglement between message buses
 */
uint64_t qbus_create_entanglement(uint64_t remote_bus_id, NodeLevel resonance_level) {
    if (!qbus_initialized) {
        return 0;
    }
    
    /* Find a free entanglement slot */
    BusEntanglement* slot = find_free_entanglement_slot();
    if (!slot) {
        printf("Cannot create bus entanglement: maximum entanglements reached\n");
        return 0;
    }
    
    /* Set up the entanglement */
    static uint64_t next_entanglement_id = 1;
    slot->id = next_entanglement_id++;
    slot->remote_bus_id = remote_bus_id;
    slot->resonance_level = resonance_level;
    slot->is_synchronized = true;
    
    printf("Created entanglement between local bus and remote bus %llu with resonance level %d\n",
           (unsigned long long)remote_bus_id, resonance_level);
    
    return slot->id;
}

/**
 * @brief Break a quantum entanglement between message buses
 */
bool qbus_break_entanglement(uint64_t entanglement_id) {
    if (!qbus_initialized) {
        return false;
    }
    
    /* Find the entanglement */
    BusEntanglement* entanglement = find_bus_entanglement(entanglement_id);
    if (!entanglement) {
        return false;
    }
    
    /* Clear the entanglement slot */
    uint64_t remote_id = entanglement->remote_bus_id;
    memset(entanglement, 0, sizeof(BusEntanglement));
    
    printf("Broke entanglement (ID: %llu) with remote bus %llu\n",
           (unsigned long long)entanglement_id, (unsigned long long)remote_id);
    
    return true;
}