#include "Settings.hpp"
#include <sys/stat.h>
#include <type_traits>
#include <fstream>
#include <iostream>
#include <giomm/file.h>

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

    void addGroupToFile(std::string_view group)
    {
        auto file = std::ofstream{CONFIG_FILE_PATH, std::ios::app};
        file << '\n' << '[' << group << ']' << '\n';
    }

    template<typename ValueType>
    void setValue(Glib::KeyFile& keyFile, Glib::ustring const& group, Glib::ustring const& key, ValueType const& value)
    {
        if constexpr (std::is_same<ValueType, bool>::value)
        {
            keyFile.set_boolean(group, key, value);
        }
        else if constexpr (std::is_same<ValueType, double>::value)
        {
            keyFile.set_double(group, key, value);
        }
        else if constexpr (std::is_same<ValueType, Glib::ustring>::value)
        {
            keyFile.set_string(group, key, value);
        }
        else
        {
            keyFile.set_value(group, key, value);
        }
    }

    template<typename ValueType>
    ValueType getValue(Glib::KeyFile const& keyFile, Glib::ustring const& group, Glib::ustring const& key, ValueType const& defaultValue = {})
    {
        try
        {
            if constexpr (std::is_same<ValueType, bool>::value)
            {
                return keyFile.get_boolean(group, key);
            }
            else if constexpr (std::is_same<ValueType, double>::value)
            {
                return keyFile.get_double(group, key);
            }
            else if constexpr (std::is_same<ValueType, Glib::ustring>::value)
            {
                return keyFile.get_string(group, key);
            }
            else
            {
                return keyFile.get_value(group, key);
            }
        }
        catch (Glib::KeyFileError const& error)
        {
            std::cerr << "Settings: " <<  error.what() << ", returning default value." << std::endl;
            return defaultValue;
        }
    }
}


Settings& Settings::getInstance()
{
    static auto instance = Settings{};
    return instance;
}

Settings::Settings()
    : m_keyFile{}
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

    m_keyFile.load_from_file(CONFIG_FILE_PATH);
    if (!m_keyFile.has_group(GROUP_GENERAL))
    {
        addGroupToFile(GROUP_GENERAL);
        m_keyFile.load_from_file(CONFIG_FILE_PATH);
    }
    if (!m_keyFile.has_group(GROUP_NETWORK))
    {
        addGroupToFile(GROUP_NETWORK);
        m_keyFile.load_from_file(CONFIG_FILE_PATH);
    }
}

Settings::~Settings()
{
    m_keyFile.save_to_file(CONFIG_FILE_PATH);
}

void Settings::setCloseToTray(bool enable)
{
    setValue(m_keyFile, GROUP_GENERAL, "close_to_tray", enable);
}

bool Settings::getCloseToTray() const
{
    return getValue(m_keyFile, GROUP_GENERAL, "close_to_tray", false);
}

void Settings::setAllowPermissions(bool allow)
{
    setValue(m_keyFile, GROUP_NETWORK, "allow_permissions", allow);
}

bool Settings::getAllowPermissions() const
{
    return getValue(m_keyFile, GROUP_NETWORK, "allow_permissions", false);
}

void Settings::setZoomLevel(double zoomLevel)
{
    setValue(m_keyFile, GROUP_GENERAL, "zoom_level", zoomLevel);
}

double Settings::getZoomLevel() const
{
    return getValue(m_keyFile, GROUP_GENERAL, "zoom_level", 1.0);
}

void Settings::setHeaderBar(bool enable)
{
    setValue(m_keyFile, GROUP_GENERAL, "header_bar", enable);
}

bool Settings::getHeaderBar() const
{
    return getValue(m_keyFile, GROUP_GENERAL, "header_bar", true);
}

void Settings::setStartInTray(bool enable)
{
    setValue(m_keyFile, GROUP_GENERAL, "start_in_tray", enable);
}

bool Settings::getStartInTray() const
{
    return getValue(m_keyFile, GROUP_GENERAL, "start_in_tray", false);
}

void Settings::setAutostart(bool autostart)
{
    auto destFile = Gio::File::create_for_path(AUTOSTART_DESKTOP_FILE_PATH);
    if (autostart)
    {
        auto const it = std::find_if(POSSIBLE_DESKTOP_FILE_PATHS.begin(), POSSIBLE_DESKTOP_FILE_PATHS.end(),[](auto const& elem){
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
            std::cerr << "Settings: Desktop file in autostart path does not exists" << std::endl;
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
