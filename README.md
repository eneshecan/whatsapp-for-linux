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

### Development:

1. Create a debug build directory. `mkdir -p build/debug`
2. Build. `cmake -DCMAKE_BUILD_TYPE=Debug --build build/debug`
3. Run. `./build/debug/WhatsApp`

### Full installation
use `cmake --install build/debug` or `make install -C build/debug` after
building (You'll probably need administrator privileges for the installation).
