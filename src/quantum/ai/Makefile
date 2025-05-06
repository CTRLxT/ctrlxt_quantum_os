CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC
LDFLAGS = -shared -lpthread -lcurl -ljson-c

# Installation paths
PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include

# Source files
SRCS = quantum_ai.c
OBJS = $(SRCS:.c=.o)

# Library name
LIBNAME = libquantum_ai.so
VERSION = 1.0.0

# Targets
all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(LIBNAME)
	install -d $(DESTDIR)$(LIBDIR)
	install -d $(DESTDIR)$(INCLUDEDIR)
	install -m 644 $(LIBNAME) $(DESTDIR)$(LIBDIR)
	install -m 644 quantum_ai.h $(DESTDIR)$(INCLUDEDIR)
	ldconfig

uninstall:
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME)
	rm -f $(DESTDIR)$(INCLUDEDIR)/quantum_ai.h
	ldconfig

clean:
	rm -f $(OBJS) $(LIBNAME)

.PHONY: all install uninstall clean 