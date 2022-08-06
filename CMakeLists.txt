cmake_minimum_required(VERSION 3.12)
project(
    whatsapp-for-linux
    VERSION 1.4.6
    DESCRIPTION "An unofficial WhatsApp desktop application for Linux"
    HOMEPAGE_URL "https://github.com/eneshecan/whatsapp-for-linux"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WFL_NAME ${CMAKE_PROJECT_NAME})
set(WFL_FRIENDLY_NAME "WhatsApp for Linux")
set(WFL_DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION})
set(WFL_HOMEPAGE ${CMAKE_PROJECT_HOMEPAGE_URL})
set(WFL_VERSION ${CMAKE_PROJECT_VERSION})
string(TIMESTAMP WFL_BUILD_DATE "%Y-%m-%d")
set(WFL_APP_ID "com.github.eneshecan.WhatsAppForLinux")
set(WFL_ICON ${WFL_APP_ID})

include(GNUInstallDirs)

message(STATUS "${CMAKE_PROJECT_NAME} version: ${WFL_VERSION}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generated/VERSION" ${WFL_VERSION})
configure_file(${CMAKE_SOURCE_DIR}/src/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/generated/Config.hpp)
configure_file(${CMAKE_SOURCE_DIR}/resource/desktop/${WFL_APP_ID}.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/generated/${WFL_APP_ID}.desktop)
configure_file(${CMAKE_SOURCE_DIR}/resource/desktop/${WFL_APP_ID}.appdata.xml.in ${CMAKE_CURRENT_BINARY_DIR}/generated/${WFL_APP_ID}.appdata.xml)
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

add_executable(${CMAKE_PROJECT_NAME}
    src/main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/generated/Resources.c
    src/util/Helper.cpp
    src/util/Settings.cpp
    src/ui/Application.cpp
    src/ui/MainWindow.cpp
    src/ui/PreferencesWindow.cpp
    src/ui/PhoneNumberDialog.cpp
    src/ui/TrayIcon.cpp
    src/ui/WebView.cpp
)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES} ${WEBKIT_LIBRARIES} ${APPINDICATOR_LIBRARIES})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/${WFL_APP_ID}.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/${WFL_APP_ID}.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resource/image/icons DESTINATION ${CMAKE_INSTALL_DATADIR})
