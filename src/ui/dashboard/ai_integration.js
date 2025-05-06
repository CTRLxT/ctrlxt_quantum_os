// AI Integration Module for Quantum Dashboard

class AIIntegration {
    constructor() {
        this.initializeAIControls();
        this.initializeWebSocket();
        this.startAIMetricsUpdate();
    }

    // Initialize AI control elements
    initializeAIControls() {
        // Model selection
        const modelSelect = document.getElementById('ai-model');
        if (modelSelect) {
            modelSelect.addEventListener('change', (e) => {
                this.switchModel(e.target.value);
            });
        }

        // Processing mode selection
        const modeSelect = document.getElementById('ai-processing-mode');
        if (modeSelect) {
            modeSelect.addEventListener('change', (e) => {
                this.setProcessingMode(e.target.value);
            });
        }

        // Quantum acceleration toggle
        const quantumToggle = document.getElementById('quantum-acceleration');
        if (quantumToggle) {
            quantumToggle.addEventListener('change', (e) => {
                this.setQuantumAcceleration(e.target.checked);
            });
        }

        // Model parameters
        const paramForm = document.getElementById('ai-parameters');
        if (paramForm) {
            paramForm.addEventListener('submit', (e) => {
                e.preventDefault();
                this.updateModelParameters(new FormData(paramForm));
            });
        }
    }

    // Initialize WebSocket connection for AI updates
    initializeWebSocket() {
        this.ws = new WebSocket('ws://localhost:8081/ai');
        
        this.ws.onopen = () => {
            console.log('AI WebSocket connection established');
            this.addAIAlert('info', 'Connected to AI System');
        };

        this.ws.onmessage = (event) => {
            const data = JSON.parse(event.data);
            this.handleAIMessage(data);
        };

        this.ws.onerror = (error) => {
            console.error('AI WebSocket error:', error);
            this.addAIAlert('error', 'AI System connection error');
        };

        this.ws.onclose = () => {
            console.log('AI WebSocket connection closed');
            this.addAIAlert('warning', 'Disconnected from AI System');
            setTimeout(() => this.initializeWebSocket(), 5000);
        };
    }

    // Handle incoming AI WebSocket messages
    handleAIMessage(data) {
        switch (data.type) {
            case 'ai_state':
                this.updateAIState(data);
                break;
            case 'ai_metrics':
                this.updateAIMetrics(data);
                break;
            case 'ai_alert':
                this.addAIAlert(data.level, data.message);
                break;
            case 'model_status':
                this.updateModelStatus(data);
                break;
        }
    }

    // Update AI state display
    updateAIState(data) {
        const stateElements = {
            'model-type': data.modelType,
            'processing-mode': data.processingMode,
            'quantum-state': data.quantumState,
            'neural-confidence': data.neuralConfidence
        };

        Object.entries(stateElements).forEach(([id, value]) => {
            const element = document.getElementById(id);
            if (element) {
                element.textContent = value;
            }
        });
    }

    // Update AI metrics display
    updateAIMetrics(data) {
        const metricsElements = {
            'ai-accuracy': `${(data.accuracy * 100).toFixed(1)}%`,
            'quantum-integrity': `${(data.quantumIntegrity * 100).toFixed(1)}%`,
            'processed-requests': data.processedRequests,
            'success-rate': `${(data.successRate * 100).toFixed(1)}%`
        };

        Object.entries(metricsElements).forEach(([id, value]) => {
            const element = document.getElementById(id);
            if (element) {
                element.textContent = value;
            }
        });
    }

    // Add AI alert to the dashboard
    addAIAlert(level, message) {
        const alertsContainer = document.getElementById('ai-alerts');
        if (!alertsContainer) return;

        const alert = document.createElement('div');
        alert.className = `alert alert-${level}`;
        alert.innerHTML = `
            <div class="flex items-center">
                <span class="status-indicator status-${level}"></span>
                <span>${message}</span>
            </div>
        `;
        alertsContainer.insertBefore(alert, alertsContainer.firstChild);

        setTimeout(() => {
            alert.style.opacity = '0';
            setTimeout(() => alert.remove(), 300);
        }, 5000);
    }

    // Switch AI model
    async switchModel(modelType) {
        try {
            const response = await fetch('/api/quantum/ai/model', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ modelType }),
            });

            const data = await response.json();
            if (data.success) {
                this.addAIAlert('info', `Switched to ${modelType} model`);
            } else {
                this.addAIAlert('error', `Failed to switch model: ${data.error}`);
            }
        } catch (error) {
            this.addAIAlert('error', 'Failed to communicate with AI System');
        }
    }

    // Set processing mode
    async setProcessingMode(mode) {
        try {
            const response = await fetch('/api/quantum/ai/mode', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ mode }),
            });

            const data = await response.json();
            if (data.success) {
                this.addAIAlert('info', `Set processing mode to ${mode}`);
            } else {
                this.addAIAlert('error', `Failed to set processing mode: ${data.error}`);
            }
        } catch (error) {
            this.addAIAlert('error', 'Failed to communicate with AI System');
        }
    }

    // Set quantum acceleration
    async setQuantumAcceleration(enable) {
        try {
            const response = await fetch('/api/quantum/ai/quantum', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ enable }),
            });

            const data = await response.json();
            if (data.success) {
                this.addAIAlert('info', `Quantum acceleration ${enable ? 'enabled' : 'disabled'}`);
            } else {
                this.addAIAlert('error', `Failed to set quantum acceleration: ${data.error}`);
            }
        } catch (error) {
            this.addAIAlert('error', 'Failed to communicate with AI System');
        }
    }

    // Update model parameters
    async updateModelParameters(formData) {
        try {
            const response = await fetch('/api/quantum/ai/parameters', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(Object.fromEntries(formData)),
            });

            const data = await response.json();
            if (data.success) {
                this.addAIAlert('info', 'Model parameters updated');
            } else {
                this.addAIAlert('error', `Failed to update parameters: ${data.error}`);
            }
        } catch (error) {
            this.addAIAlert('error', 'Failed to communicate with AI System');
        }
    }

    // Start periodic AI metrics update
    startAIMetricsUpdate() {
        setInterval(async () => {
            try {
                const response = await fetch('/api/quantum/ai/metrics');
                const data = await response.json();
                this.updateAIMetrics(data);
            } catch (error) {
                console.error('Failed to fetch AI metrics:', error);
            }
        }, 5000);
    }

    // Update model status
    updateModelStatus(data) {
        const statusContainer = document.getElementById('model-status');
        if (!statusContainer) return;

        statusContainer.innerHTML = `
            <div class="grid grid-cols-2 gap-4">
                <div>
                    <h4 class="text-lg font-semibold">Model Status</h4>
                    <p>Type: ${data.modelType}</p>
                    <p>Mode: ${data.processingMode}</p>
                    <p>Quantum: ${data.quantumEnabled ? 'Enabled' : 'Disabled'}</p>
                </div>
                <div>
                    <h4 class="text-lg font-semibold">Performance</h4>
                    <p>Accuracy: ${(data.accuracy * 100).toFixed(1)}%</p>
                    <p>Latency: ${data.latency}ms</p>
                    <p>Throughput: ${data.throughput} req/s</p>
                </div>
            </div>
        `;
    }
}

// Initialize AI integration when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.aiIntegration = new AIIntegration();
}); 