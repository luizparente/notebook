#!/bin/bash
# Script to create Linux AppImage with all dependencies

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"
APPDIR="$PROJECT_ROOT/AppDir"

echo "Creating Linux AppImage..."

# Clean and create AppDir structure
rm -rf "$APPDIR"
mkdir -p "$APPDIR/usr/bin"
mkdir -p "$APPDIR/usr/share/applications"
mkdir -p "$APPDIR/usr/share/icons/hicolor/256x256/apps"
mkdir -p "$APPDIR/usr/share/styles"
mkdir -p "$APPDIR/usr/share/metainfo"

# Copy binary
echo "Copying executable..."
cp "$BUILD_DIR/bin/notebook" "$APPDIR/usr/bin/"

# Copy styles
echo "Copying styles..."
cp -r "$PROJECT_ROOT/styles"/* "$APPDIR/usr/share/styles/"

# Create desktop file
echo "Creating desktop file..."
cat > "$APPDIR/usr/share/applications/notebook.desktop" << 'EOF'
[Desktop Entry]
Name=Notebook
Comment=A simple text editor with dark and light themes
Exec=notebook
Icon=notebook
Type=Application
Categories=Utility;TextEditor;GTK;
Terminal=false
StartupNotify=true
MimeType=text/plain;
EOF

# Create AppStream metadata
cat > "$APPDIR/usr/share/metainfo/notebook.appdata.xml" << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<component type="desktop-application">
  <id>com.notebook.editor</id>
  <metadata_license>CC0-1.0</metadata_license>
  <project_license>MIT</project_license>
  <name>Notebook</name>
  <summary>A simple text editor</summary>
  <description>
    <p>
      Notebook is a lightweight text editor built with GTK3 and GtkSourceView.
      It features syntax highlighting, line numbers, dark and light themes,
      and all the basic text editing operations you need.
    </p>
  </description>
  <categories>
    <category>Utility</category>
    <category>TextEditor</category>
  </categories>
</component>
EOF

# Create icon (simple SVG)
echo "Creating icon..."
cat > "$APPDIR/usr/share/icons/hicolor/256x256/apps/notebook.svg" << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<svg width="256" height="256" version="1.1" xmlns="http://www.w3.org/2000/svg">
  <defs>
    <linearGradient id="gradient" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" style="stop-color:#5294e2;stop-opacity:1" />
      <stop offset="100%" style="stop-color:#2b5d8f;stop-opacity:1" />
    </linearGradient>
  </defs>
  <rect width="256" height="256" rx="24" fill="url(#gradient)"/>
  <rect x="40" y="40" width="176" height="176" rx="12" fill="#ffffff" opacity="0.95"/>
  <line x1="60" y1="80" x2="196" y2="80" stroke="#5294e2" stroke-width="4" stroke-linecap="round"/>
  <line x1="60" y1="110" x2="196" y2="110" stroke="#5294e2" stroke-width="4" stroke-linecap="round"/>
  <line x1="60" y1="140" x2="150" y2="140" stroke="#5294e2" stroke-width="4" stroke-linecap="round"/>
  <line x1="60" y1="170" x2="180" y2="170" stroke="#5294e2" stroke-width="4" stroke-linecap="round"/>
</svg>
EOF

# Also create PNG versions for better compatibility
if command -v convert &> /dev/null; then
    echo "Generating PNG icons..."
    for size in 16 32 48 64 128 256; do
        mkdir -p "$APPDIR/usr/share/icons/hicolor/${size}x${size}/apps"
        convert -background none "$APPDIR/usr/share/icons/hicolor/256x256/apps/notebook.svg" \
                -resize ${size}x${size} \
                "$APPDIR/usr/share/icons/hicolor/${size}x${size}/apps/notebook.png"
    done
elif command -v rsvg-convert &> /dev/null; then
    echo "Generating PNG icons..."
    for size in 16 32 48 64 128 256; do
        mkdir -p "$APPDIR/usr/share/icons/hicolor/${size}x${size}/apps"
        rsvg-convert -w $size -h $size \
                     "$APPDIR/usr/share/icons/hicolor/256x256/apps/notebook.svg" \
                     -o "$APPDIR/usr/share/icons/hicolor/${size}x${size}/apps/notebook.png"
    done
else
    echo "Warning: Neither ImageMagick nor librsvg found. PNG icons will not be generated."
fi

# Create AppRun script
echo "Creating AppRun..."
cat > "$APPDIR/AppRun" << 'EOF'
#!/bin/bash
# AppRun script for Notebook AppImage

HERE="$(dirname "$(readlink -f "${0}")")"

# Set up environment
export PATH="${HERE}/usr/bin:${PATH}"
export LD_LIBRARY_PATH="${HERE}/usr/lib:${LD_LIBRARY_PATH}"
export XDG_DATA_DIRS="${HERE}/usr/share:${XDG_DATA_DIRS:-/usr/local/share:/usr/share}"
export GSETTINGS_SCHEMA_DIR="${HERE}/usr/share/glib-2.0/schemas:${GSETTINGS_SCHEMA_DIR}"
export GDK_PIXBUF_MODULEDIR="${HERE}/usr/lib/gdk-pixbuf-2.0/2.10.0/loaders"
export GDK_PIXBUF_MODULE_FILE="${HERE}/usr/lib/gdk-pixbuf-2.0/2.10.0/loaders.cache"

# GTK settings
export GTK_THEME="${GTK_THEME:-Adwaita}"
export GTK_DATA_PREFIX="${HERE}/usr"

# GtkSourceView
export GTKSOURCEVIEW_DIR="${HERE}/usr/share/gtksourceview-4"

# Execute the application
exec "${HERE}/usr/bin/notebook" "$@"
EOF

chmod +x "$APPDIR/AppRun"

# Download linuxdeploy if not present
LINUXDEPLOY="$PROJECT_ROOT/linuxdeploy-x86_64.AppImage"
LINUXDEPLOY_PLUGIN_GTK="$PROJECT_ROOT/linuxdeploy-plugin-gtk.sh"

if [ ! -f "$LINUXDEPLOY" ]; then
    echo "Downloading linuxdeploy..."
    wget -q -O "$LINUXDEPLOY" \
        "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
    chmod +x "$LINUXDEPLOY"
fi

if [ ! -f "$LINUXDEPLOY_PLUGIN_GTK" ]; then
    echo "Downloading linuxdeploy GTK plugin..."
    wget -q -O "$LINUXDEPLOY_PLUGIN_GTK" \
        "https://raw.githubusercontent.com/linuxdeploy/linuxdeploy-plugin-gtk/master/linuxdeploy-plugin-gtk.sh"
    chmod +x "$LINUXDEPLOY_PLUGIN_GTK"
fi

# Build AppImage
echo "Building AppImage..."
export DEPLOY_GTK_VERSION=3
export OUTPUT="notebook-linux-x86_64.AppImage"

"$LINUXDEPLOY" \
    --appdir "$APPDIR" \
    --plugin gtk \
    --output appimage \
    --desktop-file "$APPDIR/usr/share/applications/notebook.desktop" \
    --icon-file "$APPDIR/usr/share/icons/hicolor/256x256/apps/notebook.svg"

echo ""
echo "AppImage created: $OUTPUT"
echo ""
echo "To test the AppImage:"
echo "  chmod +x $OUTPUT"
echo "  ./$OUTPUT"
echo ""
echo "To install system-wide:"
echo "  sudo cp $OUTPUT /usr/local/bin/notebook"
