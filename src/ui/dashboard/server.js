const express = require('express');
const WebSocket = require('ws');
const path = require('path');
const { exec } = require('child_process');

const app = express();
const port = 8080;

// Create WebSocket server
const wss = new WebSocket.Server({ port: 8081 });

// Serve static files
app.use(express.static(path.join(__dirname)));

// Parse JSON bodies
app.use(express.json());

// Store connected clients
const clients = new Set();

// WebSocket connection handler
wss.on('connection', (ws) => {
    clients.add(ws);
    console.log('Client connected');

    ws.on('close', () => {
        clients.delete(ws);
        console.log('Client disconnected');
    });
});

// Broadcast to all connected clients
function broadcast(data) {
    const message = JSON.stringify(data);
    clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(message);
        }
    });
}

// Start periodic system metrics update
setInterval(() => {
    // Simulate quantum system metrics
    const metrics = {
        type: 'system_metrics',
        processingLoad: Math.floor(Math.random() * 100),
        memoryUtil: Math.floor(Math.random() * 100),
        coherence: Math.floor(Math.random() * 100)
    };
    broadcast(metrics);
}, 5000);

// API Routes

// Get system metrics
app.get('/api/quantum/metrics', (req, res) => {
    const metrics = {
        processingLoad: Math.floor(Math.random() * 100),
        memoryUtil: Math.floor(Math.random() * 100),
        coherence: Math.floor(Math.random() * 100)
    };
    res.json(metrics);
});

// Apply visual enhancement
app.post('/api/quantum/ocular/enhance', (req, res) => {
    const { type, strength } = req.body;
    
    // Execute the ocular enhancement script
    const command = `./src/quantum/ocular/ocular_prime.sh enhance_visual ${type} ${strength}`;
    
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error executing enhancement: ${error}`);
            res.json({ success: false, error: error.message });
            return;
        }
        
        if (stderr) {
            console.error(`Enhancement stderr: ${stderr}`);
            res.json({ success: false, error: stderr });
            return;
        }
        
        // Broadcast enhancement result
        broadcast({
            type: 'alert',
            level: 'info',
            message: `Applied ${type} enhancement with strength ${strength}`
        });
        
        res.json({ success: true, result: stdout.trim() });
    });
});

// Apply reality settings
app.post('/api/quantum/reality/settings', (req, res) => {
    const { mode, stability } = req.body;
    
    // Execute the reality settings script
    const command = `./src/quantum/ocular/ocular_prime.sh set_reality_mode ${mode} ${stability}`;
    
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error setting reality mode: ${error}`);
            res.json({ success: false, error: error.message });
            return;
        }
        
        if (stderr) {
            console.error(`Reality settings stderr: ${stderr}`);
            res.json({ success: false, error: stderr });
            return;
        }
        
        // Broadcast reality mode change
        broadcast({
            type: 'alert',
            level: 'info',
            message: `Changed reality mode to ${mode} with stability ${stability}`
        });
        
        res.json({ success: true, result: stdout.trim() });
    });
});

// AI-related endpoints

// Get AI metrics
app.get('/api/quantum/ai/metrics', (req, res) => {
    const metrics = {
        accuracy: Math.random(),
        quantumIntegrity: Math.random(),
        processedRequests: Math.floor(Math.random() * 1000),
        successRate: Math.random()
    };
    res.json(metrics);
});

// Switch AI model
app.post('/api/quantum/ai/model', (req, res) => {
    const { modelType } = req.body;
    
    // Execute the model switch script
    const command = `./src/quantum/ai/quantum_ai.sh switch_model ${modelType}`;
    
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error switching model: ${error}`);
            res.json({ success: false, error: error.message });
            return;
        }
        
        if (stderr) {
            console.error(`Model switch stderr: ${stderr}`);
            res.json({ success: false, error: stderr });
            return;
        }
        
        // Broadcast model switch
        broadcast({
            type: 'ai_alert',
            level: 'info',
            message: `Switched to ${modelType} model`
        });
        
        res.json({ success: true, result: stdout.trim() });
    });
});

// Set processing mode
app.post('/api/quantum/ai/mode', (req, res) => {
    const { mode } = req.body;
    
    // Execute the mode switch script
    const command = `./src/quantum/ai/quantum_ai.sh set_mode ${mode}`;
    
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error setting mode: ${error}`);
            res.json({ success: false, error: error.message });
            return;
        }
        
        if (stderr) {
            console.error(`Mode switch stderr: ${stderr}`);
            res.json({ success: false, error: stderr });
            return;
        }
        
        // Broadcast mode switch
        broadcast({
            type: 'ai_alert',
            level: 'info',
            message: `Set processing mode to ${mode}`
        });
        
        res.json({ success: true, result: stdout.trim() });
    });
});

// Set quantum acceleration
app.post('/api/quantum/ai/quantum', (req, res) => {
    const { enable } = req.body;
    
    // Execute the quantum acceleration script
    const command = `./src/quantum/ai/quantum_ai.sh set_quantum ${enable}`;
    
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error setting quantum acceleration: ${error}`);
            res.json({ success: false, error: error.message });
            return;
        }
        
        if (stderr) {
            console.error(`Quantum acceleration stderr: ${stderr}`);
            res.json({ success: false, error: stderr });
            return;
        }
        
        // Broadcast quantum acceleration change
        broadcast({
            type: 'ai_alert',
            level: 'info',
            message: `Quantum acceleration ${enable ? 'enabled' : 'disabled'}`
        });
        
        res.json({ success: true, result: stdout.trim() });
    });
});

// Update model parameters
app.post('/api/quantum/ai/parameters', (req, res) => {
    const params = req.body;
    
    // Execute the parameter update script
    const command = `./src/quantum/ai/quantum_ai.sh update_params ${JSON.stringify(params)}`;
    
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error updating parameters: ${error}`);
            res.json({ success: false, error: error.message });
            return;
        }
        
        if (stderr) {
            console.error(`Parameter update stderr: ${stderr}`);
            res.json({ success: false, error: stderr });
            return;
        }
        
        // Broadcast parameter update
        broadcast({
            type: 'ai_alert',
            level: 'info',
            message: 'Model parameters updated'
        });
        
        res.json({ success: true, result: stdout.trim() });
    });
});

// Start periodic AI metrics update
setInterval(() => {
    // Simulate AI metrics
    const metrics = {
        type: 'ai_metrics',
        accuracy: Math.random(),
        quantumIntegrity: Math.random(),
        processedRequests: Math.floor(Math.random() * 1000),
        successRate: Math.random()
    };
    broadcast(metrics);
}, 5000);

// Start the server
app.listen(port, () => {
    console.log(`Quantum Dashboard server running at http://localhost:${port}`);
}); 