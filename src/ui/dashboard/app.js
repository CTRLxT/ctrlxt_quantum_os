// Quantum Dashboard Application

class QuantumDashboard {
    constructor() {
        this.initializeEventListeners();
        this.initializeWebSocket();
        this.startMetricsUpdate();
    }

    // Initialize event listeners for UI controls
    initializeEventListeners() {
        // Enhancement strength slider
        const strengthSlider = document.getElementById('enhancement-strength');
        const strengthValue = document.getElementById('strength-value');
        strengthSlider.addEventListener('input', (e) => {
            strengthValue.textContent = `${e.target.value}%`;
        });

        // Reality stability slider
        const stabilitySlider = document.getElementById('reality-stability');
        const stabilityValue = document.getElementById('stability-value');
        stabilitySlider.addEventListener('input', (e) => {
            stabilityValue.textContent = `${e.target.value}%`;
        });

        // Apply enhancement button
        document.getElementById('apply-enhancement').addEventListener('click', () => {
            this.applyVisualEnhancement();
        });

        // Apply reality settings button
        document.getElementById('apply-reality').addEventListener('click', () => {
            this.applyRealitySettings();
        });
    }

    // Initialize WebSocket connection for real-time updates
    initializeWebSocket() {
        this.ws = new WebSocket('ws://localhost:8080/quantum');
        
        this.ws.onopen = () => {
            console.log('WebSocket connection established');
            this.addAlert('info', 'Connected to Quantum System');
        };

        this.ws.onmessage = (event) => {
            const data = JSON.parse(event.data);
            this.handleWebSocketMessage(data);
        };

        this.ws.onerror = (error) => {
            console.error('WebSocket error:', error);
            this.addAlert('error', 'Connection error with Quantum System');
        };

        this.ws.onclose = () => {
            console.log('WebSocket connection closed');
            this.addAlert('warning', 'Disconnected from Quantum System');
            // Attempt to reconnect after 5 seconds
            setTimeout(() => this.initializeWebSocket(), 5000);
        };
    }

    // Handle incoming WebSocket messages
    handleWebSocketMessage(data) {
        switch (data.type) {
            case 'quantum_state':
                this.updateQuantumState(data);
                break;
            case 'system_metrics':
                this.updateSystemMetrics(data);
                break;
            case 'alert':
                this.addAlert(data.level, data.message);
                break;
            case 'component_status':
                this.updateComponentStatus(data);
                break;
        }
    }

    // Update quantum state display
    updateQuantumState(data) {
        document.getElementById('entanglement-level').textContent = data.entanglement.toFixed(2);
        document.getElementById('superposition-state').textContent = data.superposition ? 'Active' : 'Inactive';
        document.getElementById('tunneling-state').textContent = data.tunneling ? 'Enabled' : 'Disabled';
    }

    // Update system metrics display
    updateSystemMetrics(data) {
        document.getElementById('processing-load').textContent = `${data.processingLoad}%`;
        document.getElementById('memory-util').textContent = `${data.memoryUtil}%`;
        document.getElementById('quantum-coherence').textContent = `${data.coherence}%`;
    }

    // Add alert to the dashboard
    addAlert(level, message) {
        const alertsContainer = document.getElementById('active-alerts');
        const alert = document.createElement('div');
        alert.className = `alert alert-${level}`;
        alert.innerHTML = `
            <div class="flex items-center">
                <span class="status-indicator status-${level}"></span>
                <span>${message}</span>
            </div>
        `;
        alertsContainer.insertBefore(alert, alertsContainer.firstChild);

        // Remove alert after 5 seconds
        setTimeout(() => {
            alert.style.opacity = '0';
            setTimeout(() => alert.remove(), 300);
        }, 5000);
    }

    // Apply visual enhancement
    async applyVisualEnhancement() {
        const type = document.getElementById('enhancement-type').value;
        const strength = document.getElementById('enhancement-strength').value / 100;

        try {
            const response = await fetch('/api/quantum/ocular/enhance', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    type,
                    strength,
                }),
            });

            const data = await response.json();
            if (data.success) {
                this.addAlert('info', `Applied ${type} enhancement successfully`);
            } else {
                this.addAlert('error', `Failed to apply enhancement: ${data.error}`);
            }
        } catch (error) {
            this.addAlert('error', 'Failed to communicate with Quantum System');
        }
    }

    // Apply reality settings
    async applyRealitySettings() {
        const mode = document.getElementById('reality-mode').value;
        const stability = document.getElementById('reality-stability').value / 100;

        try {
            const response = await fetch('/api/quantum/reality/settings', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    mode,
                    stability,
                }),
            });

            const data = await response.json();
            if (data.success) {
                this.addAlert('info', `Applied ${mode} reality mode successfully`);
            } else {
                this.addAlert('error', `Failed to apply reality settings: ${data.error}`);
            }
        } catch (error) {
            this.addAlert('error', 'Failed to communicate with Quantum System');
        }
    }

    // Start periodic metrics update
    startMetricsUpdate() {
        setInterval(async () => {
            try {
                const response = await fetch('/api/quantum/metrics');
                const data = await response.json();
                this.updateSystemMetrics(data);
            } catch (error) {
                console.error('Failed to fetch metrics:', error);
            }
        }, 5000);
    }

    // Update component status
    updateComponentStatus(data) {
        const componentsContainer = document.getElementById('quantum-components');
        componentsContainer.innerHTML = '';

        Object.entries(data.components).forEach(([name, status]) => {
            const component = document.createElement('div');
            component.className = 'flex items-center justify-between py-2';
            component.innerHTML = `
                <div class="flex items-center">
                    <span class="status-indicator status-${status.state}"></span>
                    <span>${name}</span>
                </div>
                <span class="text-sm text-gray-400">${status.message}</span>
            `;
            componentsContainer.appendChild(component);
        });
    }
}

// Initialize dashboard when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.quantumDashboard = new QuantumDashboard();
}); 