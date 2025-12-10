#!/bin/bash
# Script to bundle Windows dependencies for portable distribution

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"
DIST_DIR="$PROJECT_ROOT/dist/notebook"

echo "Creating Windows distribution package..."

# Clean and create dist directory
rm -rf "$PROJECT_ROOT/dist"
mkdir -p "$DIST_DIR/bin"
mkdir -p "$DIST_DIR/share/glib-2.0/schemas"
mkdir -p "$DIST_DIR/lib/gdk-pixbuf-2.0/2.10.0"

# Copy executable
echo "Copying executable..."
cp "$BUILD_DIR/bin/notebook.exe" "$DIST_DIR/"

# Copy styles
echo "Copying styles..."
cp -r "$PROJECT_ROOT/styles" "$DIST_DIR/"

# Find and copy all DLL dependencies
echo "Copying DLL dependencies..."
ldd "$BUILD_DIR/bin/notebook.exe" | grep mingw64 | awk '{print $3}' | while read -r dll; do
    if [ -f "$dll" ]; then
        cp "$dll" "$DIST_DIR/bin/"
    fi
done

# Copy GTK runtime files
echo "Copying GTK runtime files..."
if [ -d "/mingw64/share/glib-2.0/schemas" ]; then
    cp /mingw64/share/glib-2.0/schemas/*.xml "$DIST_DIR/share/glib-2.0/schemas/"
    glib-compile-schemas "$DIST_DIR/share/glib-2.0/schemas/"
fi

if [ -d "/mingw64/share/icons" ]; then
    cp -r /mingw64/share/icons "$DIST_DIR/share/"
fi

if [ -d "/mingw64/share/gtksourceview-4" ]; then
    cp -r /mingw64/share/gtksourceview-4 "$DIST_DIR/share/"
fi

# Copy GDK pixbuf loaders
if [ -d "/mingw64/lib/gdk-pixbuf-2.0" ]; then
    cp -r /mingw64/lib/gdk-pixbuf-2.0/2.10.0/loaders "$DIST_DIR/lib/gdk-pixbuf-2.0/2.10.0/"
fi

# Generate pixbuf loaders cache
if command -v gdk-pixbuf-query-loaders &> /dev/null; then
    gdk-pixbuf-query-loaders > "$DIST_DIR/lib/gdk-pixbuf-2.0/2.10.0/loaders.cache"
fi

# Create launcher batch file
cat > "$DIST_DIR/notebook.bat" << 'EOF'
@echo off
setlocal

:: Set up paths for bundled dependencies
set PATH=%~dp0bin;%PATH%
set GTK_DATA_PREFIX=%~dp0
set GDK_PIXBUF_MODULEDIR=%~dp0lib\gdk-pixbuf-2.0\2.10.0\loaders
set GDK_PIXBUF_MODULE_FILE=%~dp0lib\gdk-pixbuf-2.0\2.10.0\loaders.cache
set XDG_DATA_DIRS=%~dp0share;%XDG_DATA_DIRS%
set GSETTINGS_SCHEMA_DIR=%~dp0share\glib-2.0\schemas

:: Run the application
"%~dp0notebook.exe" %*
endlocal
EOF

# Create README
cat > "$DIST_DIR/README.txt" << 'EOF'
Notebook Text Editor for Windows
==================================

Installation:
-------------
This is a portable application. No installation required.
Simply extract the ZIP file and run notebook.bat

Usage:
------
Double-click notebook.bat to launch the application.

All dependencies are included in this package.

Features:
---------
- Syntax highlighting
- Line numbers
- Dark and light themes
- Auto-indent
- File operations (New, Open, Save, Save As)
- Text editing operations (Cut, Copy, Paste, Select All)

Keyboard Shortcuts:
-------------------
Ctrl+N - New document
Ctrl+O - Open file
Ctrl+S - Save
Ctrl+Shift+S - Save As
Ctrl+Q - Quit
Ctrl+X - Cut
Ctrl+C - Copy
Ctrl+V - Paste
Ctrl+A - Select All
EOF

# Create LICENSE file if it exists in project root
if [ -f "$PROJECT_ROOT/LICENSE" ]; then
    cp "$PROJECT_ROOT/LICENSE" "$DIST_DIR/"
fi

echo "Windows distribution package created in: $DIST_DIR"
echo ""
echo "To create a ZIP archive, run:"
echo "  cd dist && zip -r notebook-windows-x86_64.zip notebook"
