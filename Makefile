# Makefile for Notebook Application
# Follows professional C project build structure

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude `pkg-config --cflags gtk+-3.0 gtksourceview-4`
LDFLAGS = `pkg-config --libs gtk+-3.0 gtksourceview-4`

# Debug and release flags
DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Target executable
TARGET = $(BIN_DIR)/notebook

# Source files
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/core/document.c \
          $(SRC_DIR)/core/application.c \
          $(SRC_DIR)/io/file_operations.c \
          $(SRC_DIR)/clipboard/clipboard_operations.c \
          $(SRC_DIR)/theme/theme_manager.c \
          $(SRC_DIR)/ui/main_window.c

# Object files
OBJECTS = $(OBJ_DIR)/main.o \
          $(OBJ_DIR)/core/document.o \
          $(OBJ_DIR)/core/application.o \
          $(OBJ_DIR)/io/file_operations.o \
          $(OBJ_DIR)/clipboard/clipboard_operations.o \
          $(OBJ_DIR)/theme/theme_manager.o \
          $(OBJ_DIR)/ui/main_window.o

# Default target
.PHONY: all
all: CFLAGS += $(DEBUG_FLAGS)
all: $(TARGET)

# Release target
.PHONY: release
release:
	@$(MAKE) clean
	@$(MAKE) CFLAGS="$(CFLAGS) $(RELEASE_FLAGS)" build-release

.PHONY: build-release
build-release: create-dirs $(TARGET)

.PHONY: create-dirs
create-dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/core
	@mkdir -p $(OBJ_DIR)/io
	@mkdir -p $(OBJ_DIR)/clipboard
	@mkdir -p $(OBJ_DIR)/theme
	@mkdir -p $(OBJ_DIR)/ui

# Link executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile individual source files
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/core/document.o: $(SRC_DIR)/core/document.c
	@mkdir -p $(OBJ_DIR)/core
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/core/application.o: $(SRC_DIR)/core/application.c
	@mkdir -p $(OBJ_DIR)/core
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/io/file_operations.o: $(SRC_DIR)/io/file_operations.c
	@mkdir -p $(OBJ_DIR)/io
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/clipboard/clipboard_operations.o: $(SRC_DIR)/clipboard/clipboard_operations.c
	@mkdir -p $(OBJ_DIR)/clipboard
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/theme/theme_manager.o: $(SRC_DIR)/theme/theme_manager.c
	@mkdir -p $(OBJ_DIR)/theme
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/ui/main_window.o: $(SRC_DIR)/ui/main_window.c
	@mkdir -p $(OBJ_DIR)/ui
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory"

# Run the application
.PHONY: run
run: all
	$(TARGET)

# Install target (optional)
.PHONY: install
install:
	@$(MAKE) clean
	@$(MAKE) CFLAGS="$(CFLAGS) $(RELEASE_FLAGS)" build-release
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(TARGET) $(DESTDIR)/usr/local/bin/notebook

# Uninstall target
.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)/usr/local/bin/notebook

# Show help
.PHONY: help
help:
	@echo "Notebook Build System"
	@echo "====================="
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build debug version (default)"
	@echo "  release  - Build optimized release version"
	@echo "  clean    - Remove build artifacts"
	@echo "  run      - Build and run the application"
	@echo "  install  - Install to system (requires sudo)"
	@echo "  uninstall- Remove from system (requires sudo)"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Requirements:"
	@echo "  - GTK+ 3.0 development libraries"
	@echo "  - GtkSourceView 4.0 development libraries"
	@echo "  - pkg-config"
	@echo ""
	@echo "Install dependencies (Ubuntu/Debian):"
	@echo "  sudo apt-get install libgtk-3-dev libgtksourceview-4-dev pkg-config"
	@echo ""
	@echo "Install dependencies (Fedora/RHEL):"
	@echo "  sudo dnf install gtk3-devel gtksourceview4-devel pkgconfig"
	@echo ""
	@echo "Install dependencies (Arch):"
	@echo "  sudo pacman -S gtk3 gtksourceview4 pkgconf"
	@echo ""
	@echo "Install dependencies (macOS with Homebrew):"
	@echo "  brew install gtk+3 gtksourceview4 pkg-config"
