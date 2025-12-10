# Quick Start Guide

## Building the Application

### 1. Check Dependencies

First, ensure you have the required dependencies installed:

```bash
# Check for pkg-config
pkg-config --version

# Check for GTK3
pkg-config --exists gtk+-3.0 && echo "GTK3 installed" || echo "GTK3 not found"
```

### 2. Install Dependencies (if needed)

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential libgtk-3-dev libgtksourceview-4-dev pkg-config
```

**Fedora/RHEL/CentOS:**
```bash
sudo dnf install gcc gtk3-devel gtksourceview4-devel pkgconfig make
```

**Arch Linux:**
```bash
sudo pacman -S base-devel gtk3 gtksourceview4 pkgconf
```

**macOS (Homebrew):**
```bash
brew install gtk+3 gtksourceview4 pkg-config
```

### 3. Build the Project

```bash
# From the notebook directory
make
```

This will:
- Create the `build/` directory structure
- Compile all source files
- Link the final executable to `build/bin/notebook`

### 4. Run the Application

```bash
./build/bin/notebook
```

Or use the convenience target:
```bash
make run
```

## Using the Notebook

### File Operations

**Creating a New Document:**
1. File → New (or start the application fresh)
2. Start typing

**Opening a File:**
1. File → Open...
2. Select a text file from the file chooser
3. The file content will be loaded into the editor

**Saving:**
- File → Save (saves to current file if already saved/opened)
- File → Save As... (choose a new filename/location)

**Note:** The application will warn you about unsaved changes when:
- Creating a new document
- Opening another file
- Closing the application

### Editing Operations

**Cut:**
1. Select text
2. Edit → Cut
3. Selected text is removed and copied to clipboard

**Copy:**
1. Select text
2. Edit → Copy
3. Selected text is copied to clipboard

**Paste:**
1. Position cursor where you want to paste
2. Edit → Paste
3. Clipboard content is inserted

**Select All:**
- Edit → Select All
- Selects all text in the document

### Theme Switching

**Toggle Between Dark and Light Themes:**
- View → Toggle Theme

**Dark Theme (Default):**
- Dark background with light text
- Easy on the eyes for extended use
- Modern coding-style appearance

**Light Theme:**
- White background with black text
- Traditional notebook look
- High contrast for readability

### Window Management

**Title Bar Information:**
- Shows current filename (or "Untitled" for new documents)
- Shows asterisk (*) when document has unsaved changes
- Format: `Notebook - filename *` or `Notebook - filename`

**Closing:**
- File → Quit
- Click window close button (X)
- Both methods will warn about unsaved changes

## Advanced Build Options

### Release Build

For optimized production build:
```bash
make release
```

This builds with:
- Full optimizations (-O2)
- No debug symbols
- NDEBUG defined

### Cleaning Build Artifacts

```bash
make clean
```

Removes the entire `build/` directory.

### Installing System-wide

```bash
make release
sudo make install
```

Installs to `/usr/local/bin/notebook`

To run after installation:
```bash
notebook
```

To uninstall:
```bash
sudo make uninstall
```

## Troubleshooting

### "Package gtk+-3.0 was not found" or "Package gtksourceview-4 was not found"

Install GTK3 and GtkSourceView 4.0 development libraries (see step 2 above).

### Build Errors

1. Ensure you're in the `notebook` directory
2. Try cleaning first: `make clean`
3. Check that all dependencies are installed
4. Verify gcc/clang compiler is available

### Application Won't Start

1. Check the executable exists: `ls build/bin/notebook`
2. Verify GTK3 and GtkSourceView runtime libraries are installed
3. Ensure you have a display server (X11/Wayland)
4. For SSH sessions, enable X forwarding or use VNC

### File Open/Save Dialog Issues

If file dialogs don't appear:
- Ensure GTK3 is properly installed
- Check that desktop environment is running
- Verify no AppArmor/SELinux restrictions

## Development

### Project Structure
```
notebook/
├── include/     - Header files (public interfaces)
├── src/         - Implementation files
├── build/       - Build artifacts (generated)
├── Makefile     - Build configuration
└── README.md    - Full documentation
```

### Key Modules

- **core/** - Document model and application controller
- **ui/** - GTK user interface
- **io/** - File operations
- **clipboard/** - Clipboard management
- **theme/** - Theme system

### Modifying the Code

After making changes:
```bash
make clean
make
```

For faster incremental builds:
```bash
make
```

(Only changed files will be recompiled)

## Testing Your Changes

### Manual Testing Checklist

- [ ] Create new document
- [ ] Type some text
- [ ] Save to a file
- [ ] Close and reopen the file
- [ ] Modify text
- [ ] Test Save (should save to same file)
- [ ] Test Save As (should prompt for new filename)
- [ ] Test Cut/Copy/Paste
- [ ] Test Select All
- [ ] Toggle theme (View → Toggle Theme)
- [ ] Try to close with unsaved changes (should warn)
- [ ] Try to open file with unsaved changes (should warn)

## Features at a Glance

| Feature | Status |
|---------|--------|
| New Document | ✅ |
| Open File | ✅ |
| Save File | ✅ |
| Save As | ✅ |
| Cut | ✅ |
| Copy | ✅ |
| Paste | ✅ |
| Select All | ✅ |
| Dark Theme | ✅ |
| Light Theme | ✅ |
| Line Numbers | ✅ |
| Current Line Highlighting | ✅ |
| Auto Indent | ✅ |
| Unsaved Changes Detection | ✅ |
| Confirmation Dialogs | ✅ |
| Error Messages | ✅ |
| Monospace Font | ✅ |
| Word Wrap | ✅ |

## Next Steps

- Read `README.md` for full documentation
- Read `ARCHITECTURE.md` for design details
- Explore the source code in `src/` and `include/`
- Try modifying themes in `src/theme/theme_manager.c`
- Add new features following the existing patterns

## Getting Help

For issues or questions:
1. Check this guide
2. Read the README.md
3. Review the ARCHITECTURE.md
4. Examine the source code comments
5. Check GTK3 documentation: https://docs.gtk.org/gtk3/

Happy coding! 🚀