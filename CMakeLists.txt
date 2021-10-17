cmake_minimum_required(VERSION 3.8)
project(whatsapp-for-linux VERSION 1.3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

file(WRITE "${CMAKE_SOURCE_DIR}/src/VERSION" ${PROJECT_VERSION})
file(WRITE "${CMAKE_SOURCE_DIR}/src/Version.hpp" "constexpr auto const VERSION = \"${PROJECT_VERSION}\";\n")

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/src/Resources.c
    COMMAND glib-compile-resources --target=${CMAKE_SOURCE_DIR}/src/Resources.c --generate-source gresource.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/resource
    COMMENT "Compiling ui resources"
)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM3 gtkmm-3.0 REQUIRED)
PKG_CHECK_MODULES(WEBKIT webkit2gtk-4.0 REQUIRED)
PKG_CHECK_MODULES(APP_INDICATOR appindicator3-0.1 REQUIRED)
include_directories(${GTKMM3_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS} ${APP_INDICATOR_INCLUDE_DIRS})

add_executable(whatsapp-for-linux src/main.cpp src/Version.hpp src/Resources.c src/Application.cpp src/MainWindow.cpp src/PhoneNumberDialog.cpp src/TrayIcon.cpp src/WebView.cpp src/Settings.cpp)
target_link_libraries(whatsapp-for-linux ${GTKMM3_LIBRARIES} ${WEBKIT_LIBRARIES} ${APP_INDICATOR_LIBRARIES})

install(TARGETS whatsapp-for-linux DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_SOURCE_DIR}/resource/desktop/whatsapp-for-linux.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resource/image/icons DESTINATION ${CMAKE_INSTALL_DATADIR})
