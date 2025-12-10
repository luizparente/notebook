# Releasing Guide

This document explains how the automated release process works for the Notebook application.

## Overview

Every commit pushed to the `master` branch automatically triggers a build and release workflow that:

1. Creates a new GitHub release
2. Builds self-contained binaries for all supported platforms
3. Uploads the binaries as release assets

**No manual steps required** - everything is automated via GitHub Actions.

## Supported Platforms

- **Linux** - x86_64 AppImage (works on all distributions)
- **Windows** - x86_64 portable ZIP package
- **macOS** - x86_64 DMG (Intel Macs)
- **macOS** - ARM64 DMG (Apple Silicon Macs)

## How It Works

### 1. Push to Master

```bash
git add .
git commit -m "Fix window close bug"
git push origin master
```

### 2. Automatic Build

GitHub Actions workflow (`.github/workflows/release.yml`) is triggered:

- **Job 1:** Create release with tag `build-XXXXXXXX` (8-char commit SHA)
- **Job 2:** Build Linux AppImage
- **Job 3:** Build Windows ZIP package
- **Job 4:** Build macOS Intel DMG
- **Job 5:** Build macOS ARM DMG

### 3. Release Created

A new release appears on the [Releases page](../../releases) with:

- Release title: `Build XXXXXXXX`
- Tag: `build-XXXXXXXX`
- Description: Commit message and download links
- Assets: All platform binaries

## Release Naming

Releases are automatically tagged using the short commit SHA:

```
build-a1b2c3d4
```

Where `a1b2c3d4` is the first 8 characters of the commit SHA.

## Binary Details

### Linux AppImage

**File:** `notebook-linux-x86_64.AppImage`

**Size:** ~50-80 MB

**Contents:**
- Notebook executable
- GTK+ 3.0 libraries
- GtkSourceView 4.0 libraries
- All dependencies (glib, cairo, pango, etc.)
- Icons, themes, and schemas
- Custom styles

**Usage:**
```bash
chmod +x notebook-linux-x86_64.AppImage
./notebook-linux-x86_64.AppImage
```

**Requirements:** None - works on all Linux distributions

### Windows ZIP Package

**File:** `notebook-windows-x86_64.zip`

**Size:** ~40-60 MB

**Contents:**
- `notebook.exe` - Main executable
- `notebook.bat` - Launcher script (sets up environment)
- `bin/` - All required DLLs
- `share/` - GTK runtime files, themes, icons, schemas
- `lib/` - GdkPixbuf loaders
- `styles/` - Custom editor themes
- `README.txt` - Usage instructions

**Usage:**
1. Extract ZIP to any location
2. Double-click `notebook.bat`

**Requirements:** None - all DLLs included

### macOS DMG (Intel)

**File:** `notebook-macos-x86_64.dmg`

**Size:** ~50-80 MB

**Contents:**
- `Notebook.app` bundle with:
  - Binary in `MacOS/`
  - Frameworks (dylibs) in `Frameworks/`
  - Resources in `Resources/`
  - GTK runtime files in `share/`

**Usage:**
1. Open DMG
2. Drag Notebook.app to Applications
3. Launch from Applications or Launchpad

**Requirements:** macOS 10.13+ (Intel Macs)

### macOS DMG (Apple Silicon)

**File:** `notebook-macos-arm64.dmg`

**Size:** ~50-80 MB

**Contents:** Same as Intel version, but compiled for ARM64

**Usage:** Same as Intel version

**Requirements:** macOS 11+ (Apple Silicon Macs)

## Manual Release (Optional)

If you need to manually trigger a release without pushing to master:

1. Go to repository on GitHub
2. Click **Actions** tab
3. Select **Build and Release** workflow
4. Click **Run workflow** button
5. Select branch (usually `master`)
6. Click **Run workflow**

## Testing Builds Locally

Before pushing to master, you can test the build scripts locally:

### Linux

```bash
# Install dependencies
sudo apt-get install libgtk-3-dev libgtksourceview-4-dev pkg-config

# Build
make release

# Create AppImage
./scripts/bundle-linux.sh
```

### Windows (MSYS2)

```bash
# In MSYS2 MinGW64 shell
pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-gtksourceview4

# Build
make release

# Create package
./scripts/bundle-windows.sh
```

### macOS

```bash
# Install dependencies
brew install gtk+3 gtksourceview4

# Build
make release

# Create app bundle
./scripts/bundle-macos.sh
```

## Troubleshooting

### Release Already Exists

**Error:** `Release with tag 'build-XXXXXXXX' already exists`

**Solution:** 
- Delete the existing release from GitHub
- Or amend your commit to change the SHA
- Or wait for a new commit

### Build Fails

**Check:**
1. GitHub Actions logs under **Actions** tab
2. Look for red X next to commit on main page
3. Click on the failed job to see error details

**Common issues:**
- Dependency version changes
- Out of disk space on runner
- Network timeout downloading tools

### Missing Libraries in Binary

**Symptom:** Users report "library not found" errors

**Fix:**
- Linux: Check linuxdeploy output for missed dependencies
- Windows: Run `ldd notebook.exe` and ensure all DLLs are in `bin/`
- macOS: Run `otool -L Notebook.app/Contents/MacOS/notebook`

### Binary Won't Run

**Linux:**
```bash
# Check AppImage is executable
chmod +x notebook-linux-x86_64.AppImage

# Check FUSE is available
sudo apt-get install fuse libfuse2

# Extract and run directly
./notebook-linux-x86_64.AppImage --appimage-extract
./squashfs-root/AppRun
```

**Windows:**
- Ensure running `notebook.bat`, not `notebook.exe` directly
- Check Windows Defender/antivirus isn't blocking
- Verify DLLs are in `bin/` directory

**macOS:**
- Right-click app, select Open (first launch only)
- Check System Preferences → Security for blocked apps
- Remove quarantine: `xattr -cr Notebook.app`

## Best Practices

### Commit Messages

Since commit messages appear in release descriptions:

```bash
# Good
git commit -m "Fix window not closing properly on exit"

# Bad
git commit -m "stuff"
```

### Testing Before Release

1. Test locally first
2. Push to a feature branch
3. Manually trigger workflow to test
4. Merge to master when confirmed working

### Version Tags

To create versioned releases instead of commit-based:

1. Tag your commit: `git tag v1.0.0`
2. Push tag: `git push origin v1.0.0`
3. Modify workflow to trigger on tags instead of master pushes

## Customization

### Change Release Frequency

Edit `.github/workflows/release.yml`:

```yaml
# Release on every push to master (current)
on:
  push:
    branches:
      - master

# Release only on version tags
on:
  push:
    tags:
      - 'v*'

# Release manually only
on:
  workflow_dispatch:
```

### Change Release Naming

Edit release creation step in workflow:

```yaml
tag_name: v${{ github.run_number }}  # Use run number
tag_name: release-$(date +%Y%m%d)    # Use date
tag_name: ${{ github.ref_name }}     # Use branch/tag name
```

### Add Release Notes

Modify the workflow to generate changelog:

```yaml
- name: Generate Changelog
  run: git log --oneline HEAD~10..HEAD > CHANGES.txt

- name: Create Release
  with:
    body_path: CHANGES.txt
```

## Workflow Performance

Typical build times:

| Platform | Duration |
|----------|----------|
| Linux AppImage | 5-10 min |
| Windows ZIP | 10-15 min |
| macOS Intel | 8-12 min |
| macOS ARM | 8-12 min |
| **Total** | **30-50 min** |

All jobs run in parallel, so total time ≈ slowest job.

## Cost

GitHub Actions is **free** for public repositories with:
- 2,000 minutes/month free tier
- 20 concurrent jobs

Current workflow uses ~40 minutes per release.

## Further Reading

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [Creating Releases](https://docs.github.com/en/repositories/releasing-projects-on-github)
- [Workflow Syntax](https://docs.github.com/en/actions/reference/workflow-syntax-for-github-actions)