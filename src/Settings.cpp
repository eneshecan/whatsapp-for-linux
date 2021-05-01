#include "Settings.hpp"
#include <sys/stat.h>
#include <fstream>
#include <iostream>

namespace
{
    auto const CONFIG_DIR              = std::string{g_get_user_config_dir()} + "/whatsapp-for-linux";
    auto const CONFIG_FILE_PATH        = CONFIG_DIR + "/settings.conf";
    constexpr auto const GROUP_GENERAL = "General";
    constexpr auto const GROUP_NETWORK = "Network";

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
        if (mkdir(CONFIG_DIR.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
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
