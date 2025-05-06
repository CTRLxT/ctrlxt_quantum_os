# Makefile for CTRLxT OS Quantum Component Tests

CC = gcc
CFLAGS = -Wall -Wextra -g -I../src

# Source files
QEM_SRC = ../src/quantum/entanglement/entanglement_manager.c
PORTAL_SRC = ../src/quantum/portals/portal_gun.c
QRE_SRC = ../src/qre/qre.c
KNOWLEDGE_SRC = ../src/memex/knowledge/knowledge_network.c
QOPU_SRC = ../src/quantum/ocular/quantum_ocular.c

# Test files
INTEGRATION_TEST = quantum_integration_test.c
OCULAR_TEST = quantum_ocular_test.c

# Output binaries
INTEGRATION_TEST_BIN = quantum_integration_test
OCULAR_TEST_BIN = quantum_ocular_test

# Default target
all: $(INTEGRATION_TEST_BIN) $(OCULAR_TEST_BIN)

# Build the quantum integration test
$(INTEGRATION_TEST_BIN): $(INTEGRATION_TEST) $(QEM_SRC) $(PORTAL_SRC) $(QRE_SRC) $(KNOWLEDGE_SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Build the quantum ocular test
$(OCULAR_TEST_BIN): $(OCULAR_TEST) $(QEM_SRC) $(PORTAL_SRC) $(QRE_SRC) $(KNOWLEDGE_SRC) $(QOPU_SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Run the integration test
run_integration_test: $(INTEGRATION_TEST_BIN)
	./$(INTEGRATION_TEST_BIN)

# Run the ocular test
run_ocular_test: $(OCULAR_TEST_BIN)
	./$(OCULAR_TEST_BIN)

# Run all tests
run_all: run_integration_test run_ocular_test

# Clean target
clean:
	rm -f $(INTEGRATION_TEST_BIN) $(OCULAR_TEST_BIN)

.PHONY: all clean run_integration_test run_ocular_test run_all