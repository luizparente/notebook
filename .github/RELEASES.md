# GitHub Releases Quick Start

This repository is configured to **automatically build and release** binaries for every commit to `master`.

## For Users: Download Binaries

### 1. Go to Releases

Click on **[Releases](../../releases)** in the right sidebar, or go to:
```
https://github.com/YOUR_USERNAME/YOUR_REPO/releases
```

### 2. Download for Your Platform

| Platform | File to Download | Size |
|----------|-----------------|------|
| **Linux** (all distros) | `notebook-linux-x86_64.AppImage` | ~60 MB |
| **Windows** (portable) | `notebook-windows-x86_64.zip` | ~50 MB |
| **macOS** (Intel) | `notebook-macos-x86_64.dmg` | ~60 MB |
| **macOS** (Apple Silicon) | `notebook-macos-arm64.dmg` | ~60 MB |

### 3. Run the Application

#### Linux
```bash
chmod +x notebook-linux-x86_64.AppImage
./notebook-linux-x86_64.AppImage
```

#### Windows
1. Extract the ZIP file
2. Double-click `notebook.bat`

#### macOS
1. Open the DMG file
2. Drag `Notebook.app` to Applications
3. Launch from Applications

**Note:** On first launch, macOS may ask for permission. Right-click → Open to allow.

## For Developers: Triggering Releases

### Automatic (Recommended)

Every push to `master` automatically creates a release:

```bash
git add .
git commit -m "Add new feature"
git push origin master
```

Within 30-50 minutes, a new release appears with binaries for all platforms.

### Manual Trigger

1. Go to **Actions** tab
2. Select **Build and Release** workflow
3. Click **Run workflow**
4. Select `master` branch
5. Click green **Run workflow** button

## Release Naming

Releases are tagged with the commit SHA:
```
build-a1b2c3d4
```

Each release includes:
- Release title with commit SHA
- Commit message in description
- 4 binary files (Linux, Windows, macOS Intel, macOS ARM)

## What's Included

All binaries are **self-contained** and include:

- ✅ GTK+ 3.0 runtime
- ✅ GtkSourceView 4.0
- ✅ All required libraries
- ✅ Themes and icons
- ✅ Custom syntax highlighting styles
- ✅ No external dependencies needed

## Requirements

### For End Users
- **Linux:** Any modern distribution (no dependencies)
- **Windows:** Windows 7 or later (no dependencies)
- **macOS:** macOS 10.13+ for Intel, macOS 11+ for ARM

### For Developers
- GitHub repository with Actions enabled
- No secrets or tokens required (uses built-in `GITHUB_TOKEN`)

## Build Status

Check build status on the **Actions** tab:
- ✅ Green checkmark = successful build
- ❌ Red X = build failed (check logs)
- 🟡 Yellow dot = build in progress

## Troubleshooting

### Build Failed
- Check Actions tab for error logs
- Verify dependencies haven't changed
- Try manual workflow trigger

### Binary Won't Run
- **Linux:** Check FUSE is installed (`sudo apt install fuse libfuse2`)
- **Windows:** Run `notebook.bat`, not `notebook.exe`
- **macOS:** Right-click → Open (first time only)

### Release Already Exists
- Delete old release from Releases page
- Or wait for next commit (each commit = unique SHA)

## Customization

See [RELEASING.md](../../RELEASING.md) for:
- Changing release frequency
- Versioning schemes
- Adding release notes
- Testing builds locally

## Support

- 📖 [Full Release Documentation](../../RELEASING.md)
- 🔧 [Workflow Details](.github/workflows/README.md)
- 🐛 [Report Issues](../../issues)
- 💬 [Discussions](../../discussions)

---

**Pro Tip:** Star the repository to get notified of new releases! ⭐