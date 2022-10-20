#include "Settings.hpp"
#include <iostream>
#include <glibmm/miscutils.h>
#include <giomm/file.h>
#include "Config.hpp"

namespace wfl::util
{
    namespace
    {
        auto const CONFIG_DIR                  = Glib::get_user_config_dir();
        auto const CONFIG_APP_DIR              = CONFIG_DIR + "/" WFL_NAME;
        auto const CONFIG_FILE_PATH            = CONFIG_APP_DIR + "/settings.conf";
        auto const AUTOSTART_DESKTOP_FILE_PATH = CONFIG_DIR + "/autostart/" WFL_APP_ID ".desktop";
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
}
