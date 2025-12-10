# GitHub Actions Workflows

This directory contains GitHub Actions workflows for automating builds and releases.

## Workflow: Build and Release

**File:** `release.yml`

**Trigger:** Every push to the `master` branch

**Purpose:** Automatically builds self-contained binaries for Linux, Windows, and macOS, and creates a GitHub release with all artifacts.

### What it Does

1. **Creates a Release** - Generates a new GitHub release tagged with the commit SHA
2. **Builds Linux AppImage** - Self-contained executable for Linux (all distributions)
3. **Builds Windows Package** - Portable ZIP package with all DLLs and dependencies
4. **Builds macOS DMG** - Separate packages for Intel (x86_64) and Apple Silicon (ARM64)

### Release Artifacts

Each commit to `master` produces the following downloadable files:

| Platform | File | Description |
|----------|------|-------------|
| Linux | `notebook-linux-x86_64.AppImage` | Self-contained AppImage (works on all distros) |
| Windows | `notebook-windows-x86_64.zip` | Portable package (extract and run) |
| macOS Intel | `notebook-macos-x86_64.dmg` | DMG installer for Intel Macs |
| macOS ARM | `notebook-macos-arm64.dmg` | DMG installer for Apple Silicon Macs |

### Self-Contained Binaries

All binaries include their dependencies and require **no external installation**:

- **Linux AppImage**: Bundles GTK3, GtkSourceView, and all required libraries
- **Windows ZIP**: Includes all MinGW DLLs, GTK runtime files, schemas, and icons
- **macOS DMG**: App bundle contains all Homebrew libraries and GTK resources

### Usage

#### For Users

1. Go to the [Releases page](../../releases)
2. Download the appropriate file for your platform
3. Run it directly:
   - **Linux**: `chmod +x notebook-linux-x86_64.AppImage && ./notebook-linux-x86_64.AppImage`
   - **Windows**: Extract ZIP, double-click `notebook.bat`
   - **macOS**: Open DMG, drag to Applications, run

#### For Developers

The workflow runs automatically on every push to `master`. No manual intervention needed.

To manually trigger a build:
1. Go to Actions tab in GitHub
2. Select "Build and Release" workflow
3. Click "Run workflow"

### Build Process Details

#### Linux (Ubuntu 20.04)

1. Install GTK3 and GtkSourceView development libraries
2. Build with `make release`
3. Create AppDir structure with binary and resources
4. Use `linuxdeploy` with GTK plugin to bundle dependencies
5. Generate AppImage with embedded libraries

**Dependencies bundled:**
- GTK+ 3.0
- GtkSourceView 4.0
- All shared libraries (glib, cairo, pango, etc.)
- Icons and themes
- GSettings schemas

#### Windows (MSYS2/MinGW64)

1. Set up MSYS2 with MinGW64 toolchain
2. Install GTK3 and GtkSourceView
3. Build with `make release`
4. Copy all DLL dependencies using `ldd`
5. Bundle GTK runtime files, schemas, and icons
6. Create launcher script (notebook.bat) to set environment
7. Package as ZIP archive

**Dependencies bundled:**
- All MinGW64 DLLs (GTK, GLib, Cairo, Pango, etc.)
- GTK themes and icons
- GSettings schemas (compiled)
- GdkPixbuf loaders

#### macOS (Intel and ARM)

1. Install dependencies via Homebrew
2. Build with `make release`
3. Create `.app` bundle structure
4. Copy binary and resources
5. Recursively copy all dylib dependencies
6. Fix library paths using `install_name_tool`
7. Create DMG installer

**Dependencies bundled:**
- All Homebrew libraries (GTK, GLib, etc.)
- GtkSourceView language specs
- Icons and themes
- Fixed `@rpath` references

### Local Testing

You can test the build scripts locally:

```bash
# Linux
./scripts/bundle-linux.sh

# Windows (in MSYS2)
./scripts/bundle-windows.sh

# macOS
./scripts/bundle-macos.sh
```

### Troubleshooting

#### Release Creation Fails

**Problem:** Release with same tag already exists

**Solution:** The workflow creates releases tagged with commit SHA. If rebuilding the same commit, delete the old release first.

#### Missing Dependencies in Binary

**Problem:** Users report missing library errors

**Solution:** 
- Linux: Check `linuxdeploy` output for warnings
- Windows: Verify all DLLs are captured by `ldd`
- macOS: Check `otool -L` output and ensure all libs are bundled

#### Large Binary Size

**Problem:** Binaries are too large

**Expected sizes:**
- Linux AppImage: ~50-80 MB (compressed)
- Windows ZIP: ~40-60 MB (compressed)
- macOS DMG: ~50-80 MB

**Why:** GTK+ and dependencies are large. This is normal for self-contained GUI apps.

#### Build Fails on Specific Platform

**Check:**
1. GitHub Actions logs for specific error
2. Package manager changes (apt/pacman/brew updates)
3. Library version mismatches
4. File paths in scripts

### Customization

#### Change Release Versioning

Edit `.github/workflows/release.yml`:

```yaml
tag_name: build-${{ steps.slug.outputs.sha8 }}
```

Change to use version tags, dates, or other schemes.

#### Add More Platforms

To add ARM Linux, 32-bit builds, etc.:

1. Add new job to `release.yml`
2. Use appropriate runner (e.g., `ubuntu-latest` with QEMU)
3. Install cross-compilation toolchain
4. Build and upload artifact

#### Customize App Bundle

Edit the respective bundling scripts in `scripts/`:
- `bundle-linux.sh` - AppImage customization
- `bundle-windows.sh` - Windows package layout
- `bundle-macos.sh` - macOS app bundle structure

### Requirements

**Repository Settings:**
- Enable GitHub Actions in repository settings
- Ensure `GITHUB_TOKEN` has release permissions (enabled by default)

**No secrets required** - the workflow uses the built-in `GITHUB_TOKEN`.

### Performance

Average build times:
- Linux: ~5-10 minutes
- Windows: ~10-15 minutes (MSYS2 setup is slow)
- macOS Intel: ~8-12 minutes
- macOS ARM: ~8-12 minutes

**Total:** ~30-50 minutes per commit

To speed up:
- Cache MSYS2 packages
- Cache Homebrew installations
- Use artifact caching for unchanged dependencies

### Contributing

When modifying workflows:

1. Test locally with the bundling scripts first
2. Check Actions tab for errors after pushing
3. Download and test artifacts on target platform
4. Update this README with any changes

### References

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [linuxdeploy](https://github.com/linuxdeploy/linuxdeploy)
- [MSYS2](https://www.msys2.org/)
- [Homebrew](https://brew.sh/)
- [create-dmg](https://github.com/create-dmg/create-dmg)