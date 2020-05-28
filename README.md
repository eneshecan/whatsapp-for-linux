# whatsapp-for-linux

An unofficial WhatsApp linux client written in C++ with the help of gtkmm-3.0 and webkit2.


## Dependencies

* cmake >= 3.2
* gtkmm-3.0
* webkit2gtk-4.0


## Build & Run

1. Create a debug build directory. `mkdir build/debug`
2. Build. `cmake -DCMAKE_BUILD_TYPE=Debug --build build/debug`
3. Create symbolic links to ui files in build folder. `ln -s ui/ build/debug/`
4. Run. `./build/debug/WhatsApp`
