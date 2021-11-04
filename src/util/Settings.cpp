#include "Settings.hpp"
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <giomm/file.h>

namespace wfl::util
{
    namespace
    {
        auto const CONFIG_DIR                            = std::string{g_get_user_config_dir()};
        auto const CONFIG_APP_DIR                        = CONFIG_DIR +  "/whatsapp-for-linux";
        auto const CONFIG_FILE_PATH                      = CONFIG_APP_DIR + "/settings.conf";
        auto const AUTOSTART_DESKTOP_FILE_PATH           = CONFIG_DIR + "/autostart/whatsapp-for-linux.desktop";
        constexpr auto const GROUP_GENERAL               = "General";
        constexpr auto const GROUP_NETWORK               = "Network";
        constexpr auto const POSSIBLE_DESKTOP_FILE_PATHS = {
            "/usr/local/share/applications/whatsapp-for-linux.desktop",
            "/usr/share/applications/whatsapp-for-linux.desktop",
            "/snap/whatsapp-for-linux/current/share/applications/whatsapp-for-linux.desktop"
        };
    }


    Settings& Settings::getInstance()
    {
        static auto instance = Settings{};
        return instance;
    }

    Settings::Settings()
        : m_settingMap{}
    {
        auto const inputFile = std::ifstream{CONFIG_FILE_PATH};
        if (!inputFile.good())
        {
            if (mkdir(CONFIG_APP_DIR.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            {
                auto const errorNumber = errno;
                if (errorNumber != EEXIST)
                {
                    std::cerr << "Settings: Failed to create config directory: " << strerror(errorNumber) << std::endl;
                    return;
                }
            }
            std::ofstream{CONFIG_FILE_PATH};
        }

        m_settingMap.loadFromFile(CONFIG_FILE_PATH);
    }

    Settings::~Settings()
    {
        m_settingMap.saveToFile(CONFIG_FILE_PATH);
    }

    void Settings::setCloseToTray(bool enable)
    {
        m_settingMap.setValue(GROUP_GENERAL, "close_to_tray", enable);
    }

    bool Settings::getCloseToTray() const
    {
        return m_settingMap.getValue(GROUP_GENERAL, "close_to_tray", false);
    }

    void Settings::setAllowPermissions(bool allow)
    {
        m_settingMap.setValue(GROUP_NETWORK, "allow_permissions", allow);
    }

    bool Settings::getAllowPermissions() const
    {
        return m_settingMap.getValue(GROUP_NETWORK, "allow_permissions", false);
    }

    void Settings::setZoomLevel(double zoomLevel)
    {
        m_settingMap.setValue(GROUP_GENERAL, "zoom_level", zoomLevel);
    }

    double Settings::getZoomLevel() const
    {
        return m_settingMap.getValue(GROUP_GENERAL, "zoom_level", 1.0);
    }

    void Settings::setHeaderBar(bool enable)
    {
        m_settingMap.setValue(GROUP_GENERAL, "header_bar", enable);
    }

    bool Settings::getHeaderBar() const
    {
        return m_settingMap.getValue(GROUP_GENERAL, "header_bar", true);
    }

    void Settings::setStartInTray(bool enable)
    {
        m_settingMap.setValue(GROUP_GENERAL, "start_in_tray", enable);
    }

    bool Settings::getStartInTray() const
    {
        return m_settingMap.getValue(GROUP_GENERAL, "start_in_tray", false);
    }

    void Settings::setAutostart(bool autostart)
    {
        auto destFile = Gio::File::create_for_path(AUTOSTART_DESKTOP_FILE_PATH);
        if (autostart)
        {
            auto const it = std::find_if(POSSIBLE_DESKTOP_FILE_PATHS.begin(), POSSIBLE_DESKTOP_FILE_PATHS.end(),[](auto const& elem)
                {
                    auto const file = Gio::File::create_for_path(elem);
                    return file->query_exists();
                });

            if (it == POSSIBLE_DESKTOP_FILE_PATHS.end())
            {
                std::cerr << "Settings: Failed to find desktop file" << std::endl;
                return;
            }
            auto const srcFile = Gio::File::create_for_path(*it);
            if (!destFile->query_exists())
            {
                srcFile->copy(destFile);
            }
        }
        else
        {
            if (!destFile->query_exists())
            {
                std::cerr << "Settings: Desktop file in autostart path does not exist" << std::endl;
                return;
            }
            destFile->remove();
        }
    }

    bool Settings::getAutostart()
    {
        auto const desktopFile = Gio::File::create_for_path(AUTOSTART_DESKTOP_FILE_PATH);
        return desktopFile->query_exists();
    }
}
