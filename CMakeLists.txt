cmake_minimum_required(VERSION 3.8)
project(whatsapp-for-linux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/src/Resources.c
    COMMAND glib-compile-resources --target=${CMAKE_SOURCE_DIR}/src/Resources.c --generate-source gresource.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/resource
)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM3 gtkmm-3.0 REQUIRED)
PKG_CHECK_MODULES(WEBKIT webkit2gtk-4.0 REQUIRED)
include_directories(${GTKMM3_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS})

add_executable(whatsapp-for-linux src/Resources.c src/main.cpp src/MainWindow.cpp src/WebView.cpp src/Settings.cpp)
target_link_libraries(whatsapp-for-linux ${GTKMM3_LIBRARIES} ${WEBKIT_LIBRARIES})

install(TARGETS whatsapp-for-linux DESTINATION usr/bin)
install(FILES resource/desktop/whatsapp-for-linux.desktop DESTINATION usr/share/applications)
