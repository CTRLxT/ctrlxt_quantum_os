# CTRLxT Quantum OS

A next-generation operating system featuring quantum computing capabilities, advanced AI integration, and a modern web-based dashboard.

## Features

- **Quantum Processing**: Advanced quantum computing capabilities with support for:
  - Quantum-enhanced AI
  - Quantum state management
  - Quantum entanglement operations
  - Hybrid quantum-classical processing

- **AI Integration**:
  - Multiple AI model types (Quantum, Hybrid, Classical, Quantum Neural)
  - Real-time processing modes
  - Quantum acceleration
  - API integration capabilities
  - Model parameter tuning

- **Modern Dashboard**:
  - Real-time system monitoring
  - Quantum state visualization
  - AI model management
  - System health metrics
  - Alert system

## System Requirements

- Linux/Debian-based system
- GCC 9.0 or higher
- Node.js 14.0 or higher
- libcurl4-openssl-dev
- libjson-c-dev
- pthread support

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/ctrlxt_quantum_os.git
cd ctrlxt_quantum_os
```

2. Build the quantum AI module:
```bash
cd src/quantum/ai
make
sudo make install
```

3. Install dashboard dependencies:
```bash
cd src/ui/dashboard
npm install
```

4. Start the dashboard server:
```bash
npm start
```

5. Access the dashboard at `http://localhost:8080`

## Project Structure

```
ctrlxt_quantum_os/
├── src/
│   ├── quantum/
│   │   ├── ai/
│   │   │   ├── quantum_ai.h
│   │   │   ├── quantum_ai.c
│   │   │   ├── Makefile
│   │   │   └── debian/
│   │   └── ocular/
│   │       ├── quantum_ocular.h
│   │       ├── quantum_ocular.c
│   │       └── ocular_prime.sh
│   └── ui/
│       └── dashboard/
│           ├── index.html
│           ├── styles.css
│           ├── app.js
│           ├── ai_integration.js
│           ├── server.js
│           └── package.json
├── docs/
│   ├── api/
│   └── development/
├── tests/
├── .github/
├── LICENSE
└── README.md
```

## Development

### Building from Source

1. Install build dependencies:
```bash
sudo apt-get install build-essential libcurl4-openssl-dev libjson-c-dev
```

2. Build the quantum modules:
```bash
cd src/quantum/ai
make
```

3. Run tests:
```bash
make test
```

### Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## API Documentation

The API documentation is available in the `docs/api` directory. Key endpoints include:

- `/api/quantum/ai/metrics` - Get AI system metrics
- `/api/quantum/ai/model` - Switch AI models
- `/api/quantum/ai/mode` - Set processing mode
- `/api/quantum/ai/quantum` - Toggle quantum acceleration
- `/api/quantum/ai/parameters` - Update model parameters

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Quantum Computing Research Team
- AI Integration Team
- Dashboard Development Team

## Contact

- Project Maintainer: team@ctrlxt.org
- GitHub Issues: https://github.com/yourusername/ctrlxt_quantum_os/issues