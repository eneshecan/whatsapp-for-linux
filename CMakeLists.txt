cmake_minimum_required(VERSION 3.2)
project(WhatsApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Resource options.
option(INSTALL_RESOURCES "Install the resources." OFF)
set(RESOURCE_PATH "share/whatsapp-for-linux" CACHE PATH "Path (relative to CMAKE_INSTALL_PREFIX) the resources will be installed to.")

# Figure out the resource path.
if(INSTALL_RESOURCES)
    set(RESOURCE_DIR ${CMAKE_INSTALL_PREFIX}/${RESOURCE_PATH})
else()
    set(RESOURCE_DIR)
endif()

# Process Config.hpp.in.
configure_file(src/Config.hpp.in Config.hpp)

# So that the source code can #include config.hpp
include_directories(${CMAKE_CURRENT_BINARY_DIR})

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM3 gtkmm-3.0 REQUIRED)
PKG_CHECK_MODULES(WEBKIT webkit2gtk-4.0 REQUIRED)
include_directories(${GTKMM3_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS})

add_executable(WhatsApp src/main.cpp src/MainWindow.cpp src/WebView.cpp src/Settings.cpp)
target_link_libraries(WhatsApp ${GTKMM3_LIBRARIES} ${WEBKIT_LIBRARIES})

# Installation.
install(TARGETS WhatsApp DESTINATION bin)
if(INSTALL_RESOURCES)
    install(DIRECTORY ui DESTINATION ${RESOURCE_PATH})
endif()
