# Notebook - A Minimalistic (and Vibe-Coded) Text Editor

A simple yet powerful notebook clone built in C with GTK3.

## Features

- ✨ **Full text editing capabilities**
- 📝 **Line numbers** displayed in the editor
- 🎯 **Current line highlighting** for better visibility
- 📂 **File operations**: New, Open, Save, Save As
- ✂️ **Clipboard operations**: Cut, Copy, Paste, Select All
- 🎨 **Theme support**: Dark theme (default) and Light theme with toggle
- 💾 **Unsaved changes detection** with user confirmation dialogs
- 🏗️ **Professional architecture** following SOLID principles
- 🎯 **Clean separation of concerns** with layered architecture

## Project Structure

```
notebook/
├── include/              # Public header files
│   ├── clipboard/        # Clipboard operations interface
│   ├── core/            # Core business logic interfaces
│   ├── io/              # File I/O interfaces
│   ├── theme/           # Theme management interface
│   └── ui/              # UI interfaces
├── src/                 # Implementation files
│   ├── clipboard/       # Clipboard operations implementation
│   ├── core/           # Document and application logic
│   ├── io/             # File operations implementation
│   ├── theme/          # Theme management implementation
│   ├── ui/             # GTK UI implementation
│   └── main.c          # Application entry point
├── build/              # Build artifacts (generated)
│   ├── obj/           # Object files
│   └── bin/           # Executable
├── Makefile           # Build system
└── README.md          # This file
```

## Requirements

### Build Dependencies

- GCC or Clang compiler
- GTK+ 3.0 development libraries
- GtkSourceView 4.0 development libraries
- pkg-config
- GNU Make

### Installation of Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install build-essential libgtk-3-dev libgtksourceview-4-dev pkg-config
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc gtk3-devel gtksourceview4-devel pkgconfig make
```

**Arch Linux:**
```bash
sudo pacman -S base-devel gtk3 gtksourceview4 pkgconf
```

**macOS (with Homebrew):**
```bash
brew install gtk+3 gtksourceview4 pkg-config
```

## Building

### Debug Build (Default)
```bash
make
```

### Release Build (Optimized)
```bash
make release
```

### Clean Build Artifacts
```bash
make clean
```

### Build and Run
```bash
make run
```

### Show All Available Targets
```bash
make help
```

## Installation

### System-wide Installation
```bash
make release
sudo make install
```

This installs the executable to `/usr/local/bin/notebook`.

### Uninstall
```bash
sudo make uninstall
```

## Usage

### Starting the Application
```bash
./build/bin/notebook
```

Or if installed:
```bash
notebook
```

### Features

All operations are accessible via the menu bar:

**File Menu:**
- New - Create a new document
- Open - Open an existing file
- Save - Save current document
- Save As - Save with a new filename
- Quit - Exit application

**Edit Menu:**
- Cut - Cut selected text
- Copy - Copy selected text
- Paste - Paste from clipboard
- Select All - Select all text

**View Menu:**
- Toggle Theme - Switch between dark and light themes

**Help Menu:**
- About - Show application information

## Future Enhancements

Potential features for future versions:

- [ ] Undo/Redo functionality
- [ ] Find and Replace
- [ ] Syntax highlighting (GtkSourceView ready)
- [ ] Multiple document tabs
- [ ] Recent files list
- [ ] Configurable fonts and colors
- [ ] Word wrap toggle
- [ ] Status bar with line/column info
- [ ] Keyboard shortcuts
- [ ] Configuration file support

## License

This is a demonstration project showcasing the implementation of an AI-first C application.

Feel free to use, modify, and learn from this code.
