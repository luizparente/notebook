#!/bin/bash
# Script to create macOS app bundle with all dependencies

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"
APP_NAME="Notebook.app"
APP_DIR="$PROJECT_ROOT/$APP_NAME"

echo "Creating macOS app bundle..."

# Clean and create app bundle structure
rm -rf "$APP_DIR"
mkdir -p "$APP_DIR/Contents/MacOS"
mkdir -p "$APP_DIR/Contents/Resources"
mkdir -p "$APP_DIR/Contents/Frameworks"
mkdir -p "$APP_DIR/Contents/share"

# Copy binary
echo "Copying executable..."
cp "$BUILD_DIR/bin/notebook" "$APP_DIR/Contents/MacOS/"

# Copy styles
echo "Copying styles..."
cp -r "$PROJECT_ROOT/styles" "$APP_DIR/Contents/Resources/"

# Create Info.plist
echo "Creating Info.plist..."
cat > "$APP_DIR/Contents/Info.plist" << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>notebook</string>
    <key>CFBundleIdentifier</key>
    <string>com.notebook.editor</string>
    <key>CFBundleName</key>
    <string>Notebook</string>
    <key>CFBundleDisplayName</key>
    <string>Notebook</string>
    <key>CFBundleVersion</key>
    <string>1.0.0</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>NSHighResolutionCapable</key>
    <true/>
    <key>LSMinimumSystemVersion</key>
    <string>10.13</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
</dict>
</plist>
EOF

# Function to copy a library and its dependencies recursively
copy_library() {
    local lib="$1"
    local dest="$2"

    if [ ! -f "$lib" ]; then
        return
    fi

    local libname=$(basename "$lib")

    # Skip system libraries
    if [[ "$lib" == /System/* ]] || [[ "$lib" == /usr/lib/libSystem.* ]] || [[ "$lib" == /usr/lib/libc++.* ]] || [[ "$lib" == /usr/lib/libobjc.* ]]; then
        return
    fi

    # Skip if already copied
    if [ -f "$dest/$libname" ]; then
        return
    fi

    echo "  Copying: $libname"
    cp "$lib" "$dest/"
    chmod +w "$dest/$libname"

    # Get dependencies of this library
    otool -L "$lib" | tail -n +2 | awk '{print $1}' | while read -r deplib; do
        # Skip @rpath, @executable_path, and system libs
        if [[ "$deplib" != @* ]] && [[ "$deplib" != /System/* ]] && [[ "$deplib" != /usr/lib/libSystem.* ]]; then
            copy_library "$deplib" "$dest"
        fi
    done
}

# Copy GTK and other dependencies
echo "Copying libraries and dependencies..."

# Determine homebrew prefix
if [ -d "/opt/homebrew" ]; then
    BREW_PREFIX="/opt/homebrew"
elif [ -d "/usr/local" ]; then
    BREW_PREFIX="/usr/local"
else
    echo "Error: Homebrew not found"
    exit 1
fi

echo "Using Homebrew prefix: $BREW_PREFIX"

# Get all dependencies from the binary
otool -L "$BUILD_DIR/bin/notebook" | tail -n +2 | awk '{print $1}' | while read -r lib; do
    # Skip @rpath and @executable_path references
    if [[ "$lib" == @* ]]; then
        # Resolve @rpath references
        if [[ "$lib" == @rpath/* ]]; then
            libname="${lib#@rpath/}"
            # Try to find in common locations
            for prefix in "$BREW_PREFIX" /usr/local /opt/homebrew; do
                if [ -f "$prefix/lib/$libname" ]; then
                    copy_library "$prefix/lib/$libname" "$APP_DIR/Contents/Frameworks"
                    break
                fi
            done
        fi
    elif [[ "$lib" != /System/* ]] && [[ "$lib" != /usr/lib/libSystem.* ]]; then
        copy_library "$lib" "$APP_DIR/Contents/Frameworks"
    fi
done

# Copy GTK runtime data files
echo "Copying GTK runtime files..."

if [ -d "$BREW_PREFIX/share/gtksourceview-4" ]; then
    cp -r "$BREW_PREFIX/share/gtksourceview-4" "$APP_DIR/Contents/share/"
fi

if [ -d "$BREW_PREFIX/share/glib-2.0" ]; then
    mkdir -p "$APP_DIR/Contents/share/glib-2.0"
    if [ -d "$BREW_PREFIX/share/glib-2.0/schemas" ]; then
        cp -r "$BREW_PREFIX/share/glib-2.0/schemas" "$APP_DIR/Contents/share/glib-2.0/"
    fi
fi

if [ -d "$BREW_PREFIX/share/icons/hicolor" ]; then
    mkdir -p "$APP_DIR/Contents/share/icons"
    cp -r "$BREW_PREFIX/share/icons/hicolor" "$APP_DIR/Contents/share/icons/"
fi

# Copy GDK pixbuf loaders
if [ -d "$BREW_PREFIX/lib/gdk-pixbuf-2.0" ]; then
    mkdir -p "$APP_DIR/Contents/lib"
    cp -r "$BREW_PREFIX/lib/gdk-pixbuf-2.0" "$APP_DIR/Contents/lib/"
fi

# Fix library paths in the binary
echo "Fixing library paths in executable..."
install_name_tool -add_rpath "@executable_path/../Frameworks" "$APP_DIR/Contents/MacOS/notebook" 2>/dev/null || true

# Fix library paths in all copied libraries
echo "Fixing library paths in frameworks..."
for lib in "$APP_DIR/Contents/Frameworks"/*.dylib; do
    if [ -f "$lib" ]; then
        libname=$(basename "$lib")

        # Change the library's own ID
        install_name_tool -id "@rpath/$libname" "$lib" 2>/dev/null || true

        # Fix dependencies
        otool -L "$lib" | tail -n +2 | awk '{print $1}' | while read -r deplib; do
            depname=$(basename "$deplib")

            # Skip system libraries and already fixed paths
            if [[ "$deplib" != @* ]] && [[ "$deplib" != /System/* ]] && [[ "$deplib" != /usr/lib/libSystem.* ]]; then
                if [ -f "$APP_DIR/Contents/Frameworks/$depname" ]; then
                    install_name_tool -change "$deplib" "@rpath/$depname" "$lib" 2>/dev/null || true
                fi
            fi
        done
    fi
done

# Fix library references in the main executable
echo "Fixing library references in main executable..."
otool -L "$APP_DIR/Contents/MacOS/notebook" | tail -n +2 | awk '{print $1}' | while read -r lib; do
    libname=$(basename "$lib")

    if [[ "$lib" != @* ]] && [[ "$lib" != /System/* ]] && [[ "$lib" != /usr/lib/libSystem.* ]]; then
        if [ -f "$APP_DIR/Contents/Frameworks/$libname" ]; then
            install_name_tool -change "$lib" "@rpath/$libname" "$APP_DIR/Contents/MacOS/notebook" 2>/dev/null || true
        fi
    fi
done

echo ""
echo "App bundle created: $APP_DIR"
echo ""
echo "To create a DMG, run:"
echo "  create-dmg --volname 'Notebook' --window-pos 200 120 --window-size 600 400 --icon-size 100 --app-drop-link 450 185 notebook-macos.dmg '$APP_DIR'"
echo ""
echo "To test the app:"
echo "  open '$APP_DIR'"
