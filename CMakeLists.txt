cmake_minimum_required(VERSION 3.12)
project(
        whatsapp-for-linux
        VERSION 1.5.2
        DESCRIPTION "An unofficial WhatsApp desktop application for Linux"
        HOMEPAGE_URL "https://github.com/eneshecan/whatsapp-for-linux"
        LANGUAGES C CXX
    )

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WFL_NAME ${CMAKE_PROJECT_NAME})
set(WFL_FRIENDLY_NAME "WhatsApp for Linux")
set(WFL_DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION})
set(WFL_HOMEPAGE ${CMAKE_PROJECT_HOMEPAGE_URL})
set(WFL_VERSION ${CMAKE_PROJECT_VERSION})
string(TIMESTAMP WFL_BUILD_DATE "%Y-%m-%d")
set(WFL_APP_ID "com.github.eneshecan.WhatsAppForLinux")
set(WFL_ICON ${WFL_APP_ID})
set(GETTEXT_PACKAGE ${WFL_NAME})
set(WFL_LOCALEDIR "${CMAKE_INSTALL_FULL_LOCALEDIR}")

message(STATUS "${CMAKE_PROJECT_NAME} version: ${WFL_VERSION}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generated/VERSION" ${WFL_VERSION})
configure_file(${CMAKE_SOURCE_DIR}/src/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/generated/Config.hpp)
configure_file(${CMAKE_SOURCE_DIR}/resource/desktop/${WFL_APP_ID}.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/generated/${WFL_APP_ID}.desktop)
configure_file(${CMAKE_SOURCE_DIR}/resource/desktop/${WFL_APP_ID}.appdata.xml.in ${CMAKE_CURRENT_BINARY_DIR}/generated/${WFL_APP_ID}.appdata.xml)

message(STATUS "Compiling UI resources")
execute_process(
        COMMAND glib-compile-resources --target=${CMAKE_CURRENT_BINARY_DIR}/generated/Resources.c --generate-source gresource.xml
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/resource
    )

find_package(PkgConfig REQUIRED)

find_package(Intl REQUIRED)
find_package(Gettext REQUIRED)
include_directories(${Intl_INCLUDE_DIRS})
link_directories(${INTL_LIBRARY_DIRS})

pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS})

pkg_check_modules(WEBKIT webkit2gtk-4.0)
if (NOT WEBKIT_FOUND)
    pkg_check_modules(WEBKIT webkit2gtk-4.1 REQUIRED)
endif()
include_directories(${WEBKIT_INCLUDE_DIRS})

pkg_check_modules(APPINDICATOR ayatana-appindicator3-0.1 REQUIRED)
include_directories(${APPINDICATOR_INCLUDE_DIRS})

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
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES} ${WEBKIT_LIBRARIES} ${APPINDICATOR_LIBRARIES} ${Intl_LIBRARIES})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/${WFL_APP_ID}.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/${WFL_APP_ID}.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resource/image/icons DESTINATION ${CMAKE_INSTALL_DATADIR})

add_subdirectory(po)
