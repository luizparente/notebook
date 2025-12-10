# Notebook - A Professional Text Editor

A simple yet powerful notebook clone built in C with GTK3, following SOLID principles and professional software architecture patterns.

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

## Screenshots

### Dark Theme (Default)
- Dark background (#1e1e1e) with light text (#d4d4d4)
- Comfortable for extended coding/writing sessions

### Light Theme
- Clean white background with black text
- Traditional notebook appearance

## Architecture

This project follows professional software engineering principles:

### SOLID Principles Implementation

1. **Single Responsibility Principle (SRP)**
   - Each module handles one specific concern
   - `document.c` - Document state management only
   - `file_operations.c` - File I/O only
   - `clipboard_operations.c` - Clipboard operations only
   - `theme_manager.c` - Theme management only
   - `main_window.c` - UI presentation only

2. **Open/Closed Principle (OCP)**
   - Theme system allows new themes to be added without modifying existing code
   - Callback-based event system allows extension without modification

3. **Liskov Substitution Principle (LSP)**
   - Interface-based design with function pointers
   - Modules can be swapped with compatible implementations

4. **Interface Segregation Principle (ISP)**
   - Separate interfaces for different concerns (file ops, clipboard, theme)
   - Clients only depend on interfaces they use

5. **Dependency Inversion Principle (DIP)**
   - High-level modules (Application) depend on abstractions
   - Low-level modules implement those abstractions

### Project Structure

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

### Architectural Layers

```
┌─────────────────────────────────────────┐
│          UI Layer (GTK)                 │
│  - Main Window                          │
│  - Event Handling                       │
│  - User Interaction                     │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│      Application Layer                  │
│  - Coordinates subsystems               │
│  - Business workflow                    │
│  - Event callbacks                      │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│      Domain Layer                       │
│  - Document model                       │
│  - Business rules                       │
└─────────────────────────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│   Infrastructure Layer                  │
│  - File I/O                            │
│  - Clipboard                           │
│  - Theme Management                    │
└─────────────────────────────────────────┘
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

### Keyboard Shortcuts

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

## Code Quality

### Standards Compliance
- C11 standard
- POSIX compliance for file operations
- GTK3 best practices

### Compiler Warnings
Built with strict warnings enabled:
- `-Wall` - All common warnings
- `-Wextra` - Extra warnings

### Memory Management
- Proper allocation/deallocation patterns
- No memory leaks (verified with valgrind)
- Safe string handling

### Error Handling
- Comprehensive error checking
- User-friendly error messages
- Graceful degradation

## Design Patterns Used

1. **MVC (Model-View-Controller)**
   - Model: Document
   - View: MainWindow
   - Controller: Application

2. **Observer Pattern**
   - Theme change callbacks
   - Document modification callbacks

3. **Facade Pattern**
   - Application acts as facade for subsystems

4. **Strategy Pattern**
   - Pluggable theme system

## Testing

Manual testing checklist:

- [ ] Create new document
- [ ] Open existing file
- [ ] Save document
- [ ] Save As with new name
- [ ] Detect unsaved changes
- [ ] Cut/Copy/Paste operations
- [ ] Select All functionality
- [ ] Toggle between themes
- [ ] Confirm dialog on quit with unsaved changes
- [ ] Error handling for file operations

## Future Enhancements

Potential features for future versions:

- [ ] Undo/Redo functionality
- [ ] Find and Replace
- [x] Line numbers (implemented)
- [ ] Syntax highlighting (GtkSourceView ready)
- [ ] Multiple document tabs
- [ ] Recent files list
- [ ] Configurable fonts and colors
- [ ] Word wrap toggle
- [ ] Status bar with line/column info
- [ ] Keyboard shortcuts
- [ ] Configuration file support

## Contributing

When contributing to this project, please maintain:

1. **SOLID principles** in all code
2. **Consistent coding style** (K&R style)
3. **Comprehensive documentation** in headers
4. **Error handling** for all operations
5. **Memory safety** - no leaks, proper cleanup

## License

This is a demonstration project showcasing professional C architecture.
Feel free to use, modify, and learn from this code.

## Author

Built with attention to software architecture and design principles.

## Acknowledgments

- GTK+ Team for the excellent GUI toolkit
- The C programming community for best practices and patterns
- Software engineering principles from Uncle Bob Martin and others