#include "Settings.hpp"
#include <iostream>
#include <giomm/file.h>
#include "Config.hpp"

namespace wfl::util
{
    namespace
    {
        auto const CONFIG_DIR                  = std::string{g_get_user_config_dir()};
        auto const CONFIG_APP_DIR              = CONFIG_DIR + "/" WFL_NAME;
        auto const CONFIG_FILE_PATH            = CONFIG_APP_DIR + "/settings.conf";
        auto const AUTOSTART_DESKTOP_FILE_PATH = CONFIG_DIR + "/autostart/" WFL_APP_ID ".desktop";

        constexpr auto const GROUP_GENERAL     = "General";
        constexpr auto const GROUP_NETWORK     = "Network";
    }


    Settings& Settings::getInstance()
    {
        static auto instance = Settings{};
        return instance;
    }

    Settings::Settings()
        : m_settingMap{}
    {
        if (auto const configFile = Gio::File::create_for_path(CONFIG_FILE_PATH); !configFile->query_exists())
        {
            if (auto configDir = Gio::File::create_for_path(CONFIG_APP_DIR); !configDir->query_exists())
            {
                if (!configDir->make_directory())
                {
                    std::cerr << "Settings: Failed to create config directory" << std::endl;
                    return;
                }
            }

            if (!configFile->create_file())
            {
                std::cerr << "Settings: Failed to create config file" << std::endl;
                return;
            }
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
        if (auto autostartDir = Gio::File::create_for_path(CONFIG_DIR + "/autostart"); !autostartDir->query_exists())
        {
            if (!autostartDir->make_directory())
            {
                std::cerr << "Settings: Failed to create autostart directory" << std::endl;
                return;
            }
        }

        auto destDesktopFile = Gio::File::create_for_path(AUTOSTART_DESKTOP_FILE_PATH);
        if (autostart)
        {
            constexpr auto const possible_desktop_file_paths = std::array<char const*, 3>{
                "/usr/local/share/applications/" WFL_APP_ID ".desktop",
                "/usr/share/applications/" WFL_APP_ID ".desktop",
                "/snap/whatsapp-for-linux/current/share/applications/" WFL_APP_ID ".desktop"
            };

            auto const it = std::find_if(possible_desktop_file_paths.begin(), possible_desktop_file_paths.end(),[](auto const& elem)
                {
                    auto const file = Gio::File::create_for_path(elem);
                    return file->query_exists();
                });

            if (it == possible_desktop_file_paths.end())
            {
                std::cerr << "Settings: Failed to find desktop file" << std::endl;
                return;
            }
            auto const srcDesktopFile = Gio::File::create_for_path(*it);
            if (!destDesktopFile->query_exists())
            {
                srcDesktopFile->copy(destDesktopFile);
            }
        }
        else
        {
            if (!destDesktopFile->query_exists())
            {
                std::cerr << "Settings: Desktop file in autostart path does not exist" << std::endl;
                return;
            }
            destDesktopFile->remove();
        }
    }

    bool Settings::getAutostart()
    {
        auto const desktopFile = Gio::File::create_for_path(AUTOSTART_DESKTOP_FILE_PATH);
        return desktopFile->query_exists();
    }

    void Settings::setHwAccel(int hwAccel)
    {
        m_settingMap.setValue(GROUP_GENERAL, "hw_accel", hwAccel);
    }

    int Settings::getHwAccel() const
    {
        return m_settingMap.getValue(GROUP_GENERAL, "hw_accel", 1);
    }
}
