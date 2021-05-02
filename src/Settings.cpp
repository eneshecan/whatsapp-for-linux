#include "Settings.hpp"
#include <sys/stat.h>
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
}


Settings& Settings::getInstance()
{
    static auto instance = Settings{};
    return instance;
}

Settings::Settings()
    : m_keyFile{}
{
    auto inputFile = std::ifstream{CONFIG_FILE_PATH};
    if (!inputFile.good())
    {
        if (mkdir(CONFIG_APP_DIR.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        {
            std::cerr << "Settings: Failed to create config directory: " << strerror(errno) << std::endl;
            return;
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
    m_keyFile.set_boolean(GROUP_GENERAL, "close_to_tray", enable);
}

bool Settings::getCloseToTray() const
{
    auto enable = false;
    try
    {
        enable = m_keyFile.get_boolean(GROUP_GENERAL, "close_to_tray");
    }
    catch (Glib::KeyFileError const& error)
    {
        std::cerr << "Settings: " <<  error.what() << std::endl;
    }

    return enable;
}

void Settings::setAllowPermissions(bool allow)
{
    m_keyFile.set_boolean(GROUP_NETWORK, "allow_permissions", allow);
}

bool Settings::getAllowPermissions() const
{
    auto allow = false;
    try
    {
        allow = m_keyFile.get_boolean(GROUP_NETWORK, "allow_permissions");
    }
    catch (Glib::KeyFileError const& error)
    {
        std::cerr << "Settings: " <<  error.what() << std::endl;
    }

    return allow;
}

void Settings::setZoomLevel(double zoomLevel)
{
    m_keyFile.set_double(GROUP_GENERAL, "zoom_level", zoomLevel);
}

double Settings::getZoomLevel() const
{
    auto zoomLevel = 1.0;
    try
    {
        zoomLevel = m_keyFile.get_double(GROUP_GENERAL, "zoom_level");
    }
    catch (Glib::KeyFileError const& error)
    {
        std::cerr << "Settings: " << error.what() << std::endl;
    }

    return zoomLevel;
}

void Settings::setHeaderBar(bool enable)
{
    m_keyFile.set_boolean(GROUP_GENERAL, "header_bar", enable);
}

bool Settings::getHeaderBar() const
{
    auto enable = true;
    try
    {
        enable = m_keyFile.get_boolean(GROUP_GENERAL, "header_bar");
    }
    catch (Glib::KeyFileError const& error)
    {
        std::cerr << "Settings: " <<  error.what() << std::endl;
    }

    return enable;
}

void Settings::setStartInTray(bool enable)
{
    m_keyFile.set_boolean(GROUP_GENERAL, "start_in_tray", enable);
}

bool Settings::getStartInTray() const
{
    auto enable = false;
    try
    {
        enable = m_keyFile.get_boolean(GROUP_GENERAL, "start_in_tray");
    }
    catch (Glib::KeyFileError const& error)
    {
        std::cerr << "Settings: " << error.what() << std::endl;
    }

    return enable;
}

void Settings::setAutostart(bool autostart)
{
    auto gfileDest = Gio::File::create_for_path(AUTOSTART_DESKTOP_FILE_PATH);
    if(autostart)
    {
        auto it = std::find_if(POSSIBLE_DESKTOP_FILE_PATHS.begin(), POSSIBLE_DESKTOP_FILE_PATHS.end(),[](auto const& elem){
            auto gfile = Gio::File::create_for_path(elem);
            return gfile->query_exists();
        });

        if (it == POSSIBLE_DESKTOP_FILE_PATHS.end())
        {
            std::cerr << "Settings: Failed to find desktop file" << std::endl;
            return;
        }
        auto gfileSrc = Gio::File::create_for_path(*it);
        if(!gfileDest->query_exists())
            gfileSrc->copy(gfileDest);
    }
    else
    {
        if(!gfileDest->query_exists())
        {
            std::cerr << "Settings: Desktop file in autostart path does not exists" << std::endl;
            return;
        }
        gfileDest->remove();
    }
}

bool Settings::getAutostart()
{
    auto gfileDesktop = Gio::File::create_for_path(AUTOSTART_DESKTOP_FILE_PATH);
    return gfileDesktop->query_exists();
}
