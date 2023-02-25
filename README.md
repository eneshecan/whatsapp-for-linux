# WhatsApp for Linux

An unofficial WhatsApp desktop application for Linux.

<p align="left">
    <a href="https://flathub.org/apps/details/com.github.eneshecan.WhatsAppForLinux">
        <img align="center" alt="Download on Flathub" src="https://flathub.org/assets/badges/flathub-badge-en.png" width="120">
    </a>
    <a href="https://snapcraft.io/whatsapp-for-linux">
        <img align="center" alt="Get it from the Snap Store" src="https://snapcraft.io/static/images/badges/en/snap-store-black.svg" width="120">
    </a>
    <a href="https://aur.archlinux.org/packages/whatsapp-for-linux">
        <img align="center" alt="Get it from the AUR" src="https://archlinux.org/static/logos/archlinux-logo-dark-scalable.518881f04ca9.svg" width="120">
    </a>
    <a href="https://gpo.zugaina.org/Overlays/guru/net-im/whatsapp-for-linux">
        <img align="center" alt="Get it from the Gentoo Overlay Guru" src="https://gpo.zugaina.org/img/logo.png" width="120">
    </a>
    <a href="https://github.com/NixOS/nixpkgs/blob/master/pkgs/applications/networking/instant-messengers/whatsapp-for-linux/default.nix">
        <img align="center" alt="Get it from the nixpkgs" src="https://nixos.org/logo/nixos-hires.png" width="120">
    </a>
    <a href="https://github.com/OpenMandrivaAssociation/whatsapp-for-linux/tree/master">
        <img align="center" alt="Get it from the OpenMandrive Cooker" src="https://www.openmandriva.org/squelettes/img/OM-300pl.png" width="120">
    </a>
        <a href="https://madb.mageia.org/package/show/name/whatsapp-for-linux">
        <img align="center" alt="Get it from Mageia" src="https://www.mageia.org/g/media/logo/mageia-2013.png" width="120">
    </a>
</p>

[![Action Status](https://github.com/eneshecan/whatsapp-for-linux/workflows/Linter/badge.svg)](https://github.com/eneshecan/whatsapp-for-linux/actions/workflows/linter.yml)
[![Action Status](https://github.com/eneshecan/whatsapp-for-linux/workflows/Build/badge.svg)](https://github.com/eneshecan/whatsapp-for-linux/actions/workflows/build.yml)
[![Action Status](https://github.com/eneshecan/whatsapp-for-linux/workflows/Install/badge.svg)](https://github.com/eneshecan/whatsapp-for-linux/actions/workflows/install.yml)
[![Action Status](https://github.com/eneshecan/whatsapp-for-linux/workflows/Release/badge.svg)](https://github.com/eneshecan/whatsapp-for-linux/actions/workflows/release.yml)
[![POEditor](https://img.shields.io/badge/Translations-POEditor-brightgreen)](https://poeditor.com/join/project/jMGkxVn3vN)

![App Window](screenshot/app.png)


## About

WhatsApp for Linux is an unofficial WhatsApp desktop application written in C++ with the help of gtkmm and WebKitGtk libraries.
Check out [wiki](https://github.com/eneshecan/whatsapp-for-linux/wiki) for further details.


## Features

* Features come with WhatsApp Web except audio/video capture and calls
  * WhatsApp specific keyboard shortcuts work with *Alt* key instead of *Cmd*
* Zoom in/out
* System tray icon
* Notification sounds
* Autostart with system
* Fullscreen mode
* Show/Hide headerbar by pressing *Alt+H*
* Localization support in system language
* Spell check in system language. You might need to install the corresponding dictionary to get this working i.e. `aspell-en` package for English
* Open chat by phone number


## Using WhatsApp for Linux

The application is available from a number of Linux distributions:

[![Packaging Status](https://repology.org/badge/vertical-allrepos/whatsapp-for-linux.svg)](https://repology.org/project/whatsapp-for-linux/versions)


## Dependencies

* cmake >= 3.12
* intltool
* gtkmm-3.0
* webkit2gtk-4.0
* ayatana-appindicator3-0.1
* libcanberra


## Build & Run

### Development

```bash
# Create a debug build directory and go into it
mkdir -p build/debug && cd build/debug

# Build the project
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr ../..
make -j4

# Optionally, to update the default translation file
make update-translation

# Run
./whatsapp-for-linux
```

### Local installation

```bash
# Run inside the build directory once the application is built
# You'll probably need administrator privileges for this
make install
```

### Uninstall

```bash
# Run inside the build directory if you want to uninstall all files
# install_manifest.txt file is created when you run make install
xargs rm < install_manifest.txt
```


## Packaging

### Debian

```bash
# Don't forget to update the version number (0) in debian/changelog before this
# Build the package.
dpkg-buildpackage -uc -us -ui
```

### Snap

```bash
# Build the package. Pass --use-lxd option in a virtual environment
snapcraft
```

### AppImage

```bash
# Make sure that the application is installed into the `<Project Root>/AppDir` directory
make install DESTDIR=../../AppDir

# Build the package
appimage-builder --skip-test --recipe ./appimage/AppImageBuilder.yml
```


## Contributing

Please read [contributing](CONTRIBUTING.md).

### Code Contributors

[![Code Contributors](https://opencollective.com/whatsapp-for-linux/contributors.svg?width=880&button=false)](https://github.com/eneshecan/whatsapp-for-linux/graphs/contributors)
