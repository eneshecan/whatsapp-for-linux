cmake_minimum_required(VERSION 3.8)
project(
    whatsapp-for-linux
    VERSION 1.4.2
    DESCRIPTION "An unofficial WhatsApp desktop application for Linux"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WFL_VERSION ${CMAKE_PROJECT_VERSION})
set(WFL_APP_ID "com.github.eneshecan.WhatsAppForLinux")

include(GNUInstallDirs)

message(STATUS "${CMAKE_PROJECT_NAME} version: ${WFL_VERSION}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generated/VERSION" ${WFL_VERSION})
configure_file(${CMAKE_SOURCE_DIR}/src/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/generated/Config.hpp)
execute_process(
    COMMAND glib-compile-resources --target=${CMAKE_CURRENT_BINARY_DIR}/generated/Resources.c --generate-source gresource.xml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/resource
)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM gtkmm-3.0 REQUIRED)
PKG_CHECK_MODULES(WEBKIT webkit2gtk-4.0 REQUIRED)
PKG_CHECK_MODULES(APPINDICATOR ayatana-appindicator3-0.1 REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS} ${APPINDICATOR_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated/)

add_executable(whatsapp-for-linux
    src/main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/generated/Resources.c
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
