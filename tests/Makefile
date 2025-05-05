# Makefile for CTRLxT OS Quantum Component Tests

CC = gcc
CFLAGS = -Wall -Wextra -g -I../src

# Source files
QEM_SRC = ../src/quantum/entanglement/entanglement_manager.c
PORTAL_SRC = ../src/quantum/portals/portal_gun.c
QRE_SRC = ../src/qre/qre.c
KNOWLEDGE_SRC = ../src/memex/knowledge/knowledge_network.c

# Test files
INTEGRATION_TEST = quantum_integration_test.c

# Output binaries
INTEGRATION_TEST_BIN = quantum_integration_test

# Default target
all: $(INTEGRATION_TEST_BIN)

# Build the quantum integration test
$(INTEGRATION_TEST_BIN): $(INTEGRATION_TEST) $(QEM_SRC) $(PORTAL_SRC) $(QRE_SRC) $(KNOWLEDGE_SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Run the integration test
run_integration_test: $(INTEGRATION_TEST_BIN)
	./$(INTEGRATION_TEST_BIN)

# Clean target
clean:
	rm -f $(INTEGRATION_TEST_BIN)

.PHONY: all clean run_integration_test