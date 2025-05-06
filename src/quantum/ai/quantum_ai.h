#ifndef QUANTUM_AI_H
#define QUANTUM_AI_H

#include <stdbool.h>
#include <stdint.h>

// AI Model Types
typedef enum {
    AI_MODEL_QUANTUM = 0,
    AI_MODEL_HYBRID,
    AI_MODEL_CLASSICAL,
    AI_MODEL_QUANTUM_NEURAL,
    AI_MODEL_MAX
} AIModelType;

// AI Processing Modes
typedef enum {
    AI_MODE_REAL_TIME = 0,
    AI_MODE_BATCH,
    AI_MODE_QUANTUM_ACCELERATED,
    AI_MODE_HYBRID_PROCESSING,
    AI_MODE_MAX
} AIProcessingMode;

// AI Configuration Structure
typedef struct {
    AIModelType model_type;
    AIProcessingMode processing_mode;
    float quantum_entanglement_factor;
    float neural_network_confidence;
    uint32_t max_processing_threads;
    bool enable_quantum_acceleration;
    bool enable_hybrid_learning;
    char model_path[256];
    char api_endpoint[256];
    char api_key[64];
} AIConfig;

// AI State Structure
typedef struct {
    bool is_initialized;
    bool is_processing;
    float current_accuracy;
    float quantum_state_integrity;
    uint64_t processed_requests;
    uint64_t successful_predictions;
    char current_model[64];
    char last_error[256];
} AIState;

// Function Prototypes

// Initialize the AI system
bool qai_init(const AIConfig* config);

// Process input data through the AI model
bool qai_process_data(const void* input_data, size_t input_size, void* output_data, size_t output_size);

// Update AI model parameters
bool qai_update_model(const void* new_parameters, size_t param_size);

// Get current AI state
bool qai_get_state(AIState* state);

// Set AI processing mode
bool qai_set_processing_mode(AIProcessingMode mode);

// Enable/disable quantum acceleration
bool qai_set_quantum_acceleration(bool enable);

// Get AI model statistics
bool qai_get_statistics(float* accuracy, float* quantum_integrity, uint64_t* processed_count);

// Shutdown AI system
void qai_shutdown(void);

// Error handling
const char* qai_get_last_error(void);

// Model management
bool qai_load_model(const char* model_path);
bool qai_save_model(const char* model_path);
bool qai_switch_model(AIModelType model_type);

// Quantum-specific functions
bool qai_entangle_quantum_state(float entanglement_factor);
bool qai_measure_quantum_state(float* measurement_result);
bool qai_apply_quantum_correction(float correction_factor);

// API Integration
bool qai_connect_api(const char* endpoint, const char* api_key);
bool qai_disconnect_api(void);
bool qai_sync_with_cloud(void);

#endif // QUANTUM_AI_H 