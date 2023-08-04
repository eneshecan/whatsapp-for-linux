cmake_minimum_required(VERSION 3.12)
project(
        whatsapp-for-linux
        VERSION 1.6.4
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

add_subdirectory(src)
add_subdirectory(resource)
add_subdirectory(po)
