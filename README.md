# whatsapp-for-linux

An unofficial WhatsApp linux client written in C++ with the help of gtkmm-3.0 and webkit2.

[![Build Status](https://travis-ci.com/eneshecan/whatsapp-for-linux.svg?branch=master)](https://travis-ci.com/eneshecan/whatsapp-for-linux)

![App Window](https://github.com/eneshecan/whatsapp-for-linux/blob/master/screenshot/app.png)


## Contributing
Please read [CONTRIBUTING.md](CONTRIBUTING.md).


## Dependencies

* cmake >= 3.2
* gtkmm-3.0
* webkit2gtk-4.0


## Build & Run

For development:

1. Create a debug build directory. `mkdir -p build/debug`
2. Build. `cmake -DCMAKE_BUILD_TYPE=Debug --build build/debug`
3. Create symbolic links to ui files in build folder. `ln -s ui/ build/debug/`
4. Run. `./build/debug/WhatsApp`

For full installation, add `-DINSTALL_RESOURCES=on` to the CMake options, and
use `cmake --install build/debug` or `make install -C build/debug` after
building (you'll probably need administrator privileges for the installation).
