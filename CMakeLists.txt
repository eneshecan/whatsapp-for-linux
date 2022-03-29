cmake_minimum_required(VERSION 3.8)
project(whatsapp-for-linux VERSION 1.4.2)

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
PKG_CHECK_MODULES(GTKMM gtkmm-3.0 REQUIRED)
PKG_CHECK_MODULES(WEBKIT webkit2gtk-4.0 REQUIRED)
PKG_CHECK_MODULES(APPINDICATOR ayatana-appindicator3-0.1 REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS} ${APPINDICATOR_INCLUDE_DIRS})

add_executable(whatsapp-for-linux
    src/main.cpp
    src/Version.hpp
    src/Resources.c
    src/util/Settings.cpp
    src/ui/Application.cpp
    src/ui/MainWindow.cpp
    src/ui/PhoneNumberDialog.cpp
    src/ui/TrayIcon.cpp
    src/ui/WebView.cpp
)
target_link_libraries(whatsapp-for-linux ${GTKMM_LIBRARIES} ${WEBKIT_LIBRARIES} ${APPINDICATOR_LIBRARIES})

install(TARGETS whatsapp-for-linux DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_SOURCE_DIR}/resource/desktop/com.github.eneshecan.WhatsAppForLinux.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES ${CMAKE_SOURCE_DIR}/resource/desktop/com.github.eneshecan.WhatsAppForLinux.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resource/image/icons DESTINATION ${CMAKE_INSTALL_DATADIR})
