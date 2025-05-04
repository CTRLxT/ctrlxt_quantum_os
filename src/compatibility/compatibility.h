/**
 * @file compatibility.h
 * @brief CTRLxT OS Compatibility Layer interface
 *
 * This file defines the interface for the CTRLxT OS Compatibility Layer,
 * which provides support for running applications from various operating
 * systems on CTRLxT OS.
 */

#ifndef CTRLXT_COMPATIBILITY_H
#define CTRLXT_COMPATIBILITY_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Compatibility platform types
 */
typedef enum {
    COMPAT_WINDOWS,      /**< Windows compatibility */
    COMPAT_MACOS,        /**< macOS compatibility */
    COMPAT_IOS,          /**< iOS compatibility */
    COMPAT_ANDROID,      /**< Android compatibility */
    COMPAT_LINUX,        /**< Linux compatibility */
    COMPAT_POSIX         /**< POSIX compatibility */
} CompatibilityPlatform;

/**
 * @brief Compatibility method types
 */
typedef enum {
    METHOD_API_TRANSLATION,   /**< API translation */
    METHOD_CONTAINERIZATION,  /**< Containerization */
    METHOD_BINARY_TRANSLATION, /**< Binary translation */
    METHOD_KERNEL_COMPAT      /**< Kernel-level compatibility */
} CompatibilityMethod;

/**
 * @brief Compatibility environment structure
 */
typedef struct {
    uint64_t id;                         /**< Unique identifier */
    CompatibilityPlatform platform;       /**< Target platform */
    CompatibilityMethod method;           /**< Compatibility method */
    char *environment_path;               /**< Path to environment */
    bool use_quantum_entanglement;        /**< Whether to use quantum entanglement */
    uint32_t memory_limit;                /**< Memory limit in MB */
} CompatibilityEnvironment;

/**
 * @brief Initialize the Compatibility Layer
 * 
 * @return true if initialization succeeded, false otherwise
 */
bool compat_init(void);

/**
 * @brief Create a new compatibility environment
 * 
 * @param platform Target platform
 * @param method Compatibility method
 * @param use_quantum_entanglement Whether to use quantum entanglement
 * @param memory_limit Memory limit in MB (0 for unlimited)
 * @return New CompatibilityEnvironment structure
 */
CompatibilityEnvironment compat_create_environment(CompatibilityPlatform platform,
                                                 CompatibilityMethod method,
                                                 bool use_quantum_entanglement,
                                                 uint32_t memory_limit);

/**
 * @brief Launch an application in a compatibility environment
 * 
 * @param environment_id Environment ID
 * @param application_path Path to the application
 * @param arguments Command-line arguments
 * @param working_directory Working directory
 * @return Process ID if successful, 0 otherwise
 */
uint64_t compat_launch_application(uint64_t environment_id,
                                  const char *application_path,
                                  const char **arguments,
                                  const char *working_directory);

/**
 * @brief Install a compatibility layer for a specific platform
 * 
 * @param platform Target platform
 * @param method Compatibility method
 * @return true if installation succeeded, false otherwise
 */
bool compat_install_layer(CompatibilityPlatform platform, CompatibilityMethod method);

/**
 * @brief Check if a compatibility layer is installed
 * 
 * @param platform Target platform
 * @param method Compatibility method
 * @return true if installed, false otherwise
 */
bool compat_is_installed(CompatibilityPlatform platform, CompatibilityMethod method);

/**
 * @brief Shutdown the Compatibility Layer
 */
void compat_shutdown(void);

#endif /* CTRLXT_COMPATIBILITY_H */