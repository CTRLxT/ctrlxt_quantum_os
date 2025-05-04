/**
 * @file quantum_message_bus.h
 * @brief Quantum Message Bus for Inter-Component Communication
 *
 * This file defines the interface for the Quantum Message Bus,
 * which enables communication between quantum components in CTRLxT OS.
 */

#ifndef CTRLXT_QUANTUM_MESSAGE_BUS_H
#define CTRLXT_QUANTUM_MESSAGE_BUS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../resonance/resonant_frequencies.h"

/**
 * @brief Component IDs for message routing
 */
typedef enum {
    QCOMP_UNKNOWN = 0,
    QCOMP_KERNEL,                 /**< Kernel */
    QCOMP_MEMORY_MANAGER,         /**< Memory Manager */
    QCOMP_PROCESS_MANAGER,        /**< Process Manager */
    QCOMP_SCHEDULER,              /**< Scheduler */
    QCOMP_ENTANGLEMENT_MANAGER,   /**< Quantum Entanglement Manager */
    QCOMP_PORTAL_GUN,             /**< Portal Gun */
    QCOMP_OCULAR,                 /**< Quantum Ocular Processing Unit */
    QCOMP_TELEPORT,               /**< Quantum Teleportation System */
    QCOMP_REALITY_ENGINE,         /**< Unified Quantum Reality Engine */
    QCOMP_MEMEX,                  /**< Memex Integration */
    QCOMP_USER_PROCESS_BASE = 1000 /**< Base for user process components */
} QComponentId;

/**
 * @brief Message priority levels
 */
typedef enum {
    QMSG_PRIORITY_LOW = 0,        /**< Low priority message */
    QMSG_PRIORITY_NORMAL,          /**< Normal priority message */
    QMSG_PRIORITY_HIGH,            /**< High priority message */
    QMSG_PRIORITY_CRITICAL,        /**< Critical priority message */
    QMSG_PRIORITY_QUANTUM          /**< Quantum priority message (highest) */
} QMessagePriority;

/**
 * @brief Message types
 */
typedef enum {
    /* System messages */
    QMSG_SYSTEM_STARTUP = 0,       /**< System startup */
    QMSG_SYSTEM_SHUTDOWN,          /**< System shutdown */
    QMSG_COMPONENT_REGISTER,       /**< Component registration */
    QMSG_COMPONENT_UNREGISTER,     /**< Component unregistration */
    QMSG_PING,                     /**< Ping request */
    QMSG_PONG,                     /**< Ping response */
    
    /* Memory messages */
    QMSG_MEMORY_ALLOCATE = 100,    /**< Memory allocation request */
    QMSG_MEMORY_FREE,              /**< Memory free request */
    QMSG_MEMORY_ENTANGLE,          /**< Memory entanglement request */
    QMSG_MEMORY_DISENTANGLE,       /**< Memory disentanglement request */
    QMSG_MEMORY_SYNC,              /**< Memory synchronization request */
    
    /* Process messages */
    QMSG_PROCESS_CREATE = 200,     /**< Process creation request */
    QMSG_PROCESS_TERMINATE,        /**< Process termination request */
    QMSG_PROCESS_ENTANGLE,         /**< Process entanglement request */
    QMSG_PROCESS_DISENTANGLE,      /**< Process disentanglement request */
    QMSG_PROCESS_SYNC,             /**< Process synchronization request */
    
    /* Quantum entanglement messages */
    QMSG_ENTANGLE_CREATE = 300,    /**< Entanglement creation request */
    QMSG_ENTANGLE_DESTROY,         /**< Entanglement destruction request */
    QMSG_ENTANGLE_SYNC,            /**< Entanglement synchronization request */
    QMSG_ENTANGLE_STATE_CHANGE,    /**< Entanglement state change notification */
    
    /* Portal messages */
    QMSG_PORTAL_CREATE = 400,      /**< Portal creation request */
    QMSG_PORTAL_CLOSE,             /**< Portal close request */
    QMSG_PORTAL_MODIFY,            /**< Portal modification request */
    QMSG_PORTAL_TRAVERSE,          /**< Portal traversal request */
    
    /* Teleportation messages */
    QMSG_TELEPORT_REQUEST = 500,   /**< Teleportation request */
    QMSG_TELEPORT_COMPLETE,        /**< Teleportation complete notification */
    QMSG_BLINK_SPOT_CREATE,        /**< Blink spot creation request */
    QMSG_BLINK_SPOT_DELETE,        /**< Blink spot deletion request */
    
    /* Reality engine messages */
    QMSG_REALITY_MODE_CHANGE = 600, /**< Reality mode change request */
    QMSG_REALITY_OBJECT_CREATE,    /**< Reality object creation request */
    QMSG_REALITY_OBJECT_DELETE,    /**< Reality object deletion request */
    QMSG_REALITY_OBJECT_MODIFY,    /**< Reality object modification request */
    
    /* Ocular processing messages */
    QMSG_OCULAR_DATA = 700,        /**< Ocular data update */
    QMSG_OCULAR_MODE_CHANGE,       /**< Ocular mode change request */
    QMSG_OCULAR_AUDIO_UPGRADE,     /**< Ocular audio upgrade request */
    
    /* Resonance messages */
    QMSG_RESONANCE_SHIFT = 800,    /**< Resonance level shift request */
    QMSG_RESONANCE_SYNC,           /**< Resonance synchronization request */
    QMSG_RESONANCE_MEASURE,        /**< Resonance measurement request */
    
    /* User-defined messages */
    QMSG_USER_DEFINED_BASE = 10000 /**< Base for user-defined messages */
} QMessageType;

/**
 * @brief Forward declaration of quantum message
 */
typedef struct QMessage QMessage;

/**
 * @brief Message handler function type
 */
typedef void (*QMessageHandler)(QMessage* message, void* context);

/**
 * @brief Subscription structure
 */
typedef struct {
    QComponentId component_id;     /**< Component ID */
    QMessageType message_type;     /**< Message type to subscribe to (-1 for all) */
    QMessageHandler handler;       /**< Message handler function */
    void* context;                 /**< Context to pass to handler */
    NodeLevel min_resonance;       /**< Minimum resonance level for messages */
} QSubscription;

/**
 * @brief Quantum message header
 */
typedef struct {
    uint64_t message_id;           /**< Unique message ID */
    QMessageType type;             /**< Message type */
    QComponentId source;           /**< Source component ID */
    QComponentId destination;      /**< Destination component ID (0 for broadcast) */
    QMessagePriority priority;     /**< Message priority */
    NodeLevel resonance_level;     /**< Message resonance level */
    uint64_t timestamp;            /**< Message timestamp */
    bool requires_response;        /**< Whether message requires a response */
    uint64_t response_to;          /**< Message ID this is a response to (0 if not a response) */
    uint32_t data_size;            /**< Size of message data */
} QMessageHeader;

/**
 * @brief Quantum message structure
 */
struct QMessage {
    QMessageHeader header;         /**< Message header */
    void* data;                    /**< Message data */
};

/**
 * @brief Component registration information
 */
typedef struct {
    QComponentId id;               /**< Component ID */
    char name[64];                 /**< Component name */
    NodeLevel resonance_level;     /**< Component resonance level */
    void* context;                 /**< Component context */
} QComponentInfo;

/**
 * @brief Initialize the Quantum Message Bus
 * 
 * @return true if initialization succeeded, false otherwise
 */
bool qbus_init(void);

/**
 * @brief Shutdown the Quantum Message Bus
 */
void qbus_shutdown(void);

/**
 * @brief Register a component with the message bus
 * 
 * @param info Component information
 * @return true if registration succeeded, false otherwise
 */
bool qbus_register_component(const QComponentInfo* info);

/**
 * @brief Unregister a component from the message bus
 * 
 * @param component_id Component ID to unregister
 * @return true if unregistration succeeded, false otherwise
 */
bool qbus_unregister_component(QComponentId component_id);

/**
 * @brief Subscribe to message types
 * 
 * @param subscription Subscription information
 * @return true if subscription succeeded, false otherwise
 */
bool qbus_subscribe(const QSubscription* subscription);

/**
 * @brief Unsubscribe from message types
 * 
 * @param component_id Component ID to unsubscribe
 * @param message_type Message type to unsubscribe from (-1 for all)
 * @param handler Specific handler to unsubscribe (NULL for all handlers)
 * @return true if unsubscription succeeded, false otherwise
 */
bool qbus_unsubscribe(QComponentId component_id, QMessageType message_type, QMessageHandler handler);

/**
 * @brief Send a message
 * 
 * @param message Message to send
 * @return true if send succeeded, false otherwise
 */
bool qbus_send_message(const QMessage* message);

/**
 * @brief Create a new message
 * 
 * @param type Message type
 * @param source Source component ID
 * @param destination Destination component ID (0 for broadcast)
 * @param data Message data
 * @param data_size Size of message data
 * @param priority Message priority
 * @param requires_response Whether message requires a response
 * @return New message or NULL on failure (must be freed with qbus_free_message)
 */
QMessage* qbus_create_message(QMessageType type, QComponentId source, QComponentId destination,
                           const void* data, uint32_t data_size, QMessagePriority priority,
                           bool requires_response);

/**
 * @brief Create a response message
 * 
 * @param original_message Original message to respond to
 * @param data Response data
 * @param data_size Size of response data
 * @param priority Response priority
 * @return New response message or NULL on failure (must be freed with qbus_free_message)
 */
QMessage* qbus_create_response(const QMessage* original_message, const void* data,
                             uint32_t data_size, QMessagePriority priority);

/**
 * @brief Free a message
 * 
 * @param message Message to free
 */
void qbus_free_message(QMessage* message);

/**
 * @brief Process pending messages
 * 
 * @param max_messages Maximum number of messages to process (0 for all pending)
 * @return Number of messages processed
 */
uint32_t qbus_process_messages(uint32_t max_messages);

/**
 * @brief Find a component by ID
 * 
 * @param component_id Component ID to find
 * @param info Component information structure to fill (if found)
 * @return true if component was found, false otherwise
 */
bool qbus_find_component(QComponentId component_id, QComponentInfo* info);

/**
 * @brief Set a component's resonance level
 * 
 * @param component_id Component ID
 * @param resonance_level New resonance level
 * @return true if change succeeded, false otherwise
 */
bool qbus_set_component_resonance(QComponentId component_id, NodeLevel resonance_level);

/**
 * @brief Create a quantum entanglement between message buses
 * 
 * @param remote_bus_id Remote bus identifier
 * @param resonance_level Resonance level for entanglement
 * @return Entanglement ID or 0 on failure
 */
uint64_t qbus_create_entanglement(uint64_t remote_bus_id, NodeLevel resonance_level);

/**
 * @brief Break a quantum entanglement between message buses
 * 
 * @param entanglement_id Entanglement ID to break
 * @return true if breaking succeeded, false otherwise
 */
bool qbus_break_entanglement(uint64_t entanglement_id);

#endif /* CTRLXT_QUANTUM_MESSAGE_BUS_H */