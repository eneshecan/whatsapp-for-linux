#include "Settings.hpp"
#include <fstream>
#include <iostream>


namespace
{
    auto const CONFIG_DIR              = std::string{getenv("HOME")} + "/.config/whatsapp-for-linux";
    auto const CONFIG_FILE_PATH        = CONFIG_DIR + "/settings.conf";
    constexpr auto const GROUP_GENERAL = "General";
    constexpr auto const GROUP_NETWORK = "Network";


    void addGroupToFile(std::string_view group)
    {
        auto file = std::ofstream{CONFIG_FILE_PATH, std::ios::app};
        file << '\n' << '[' << group << ']' << '\n';
    }
}


Settings& Settings::instance()
{
    static auto instance = Settings{};
    return instance;
}

Settings::Settings()
{
    auto inputFile = std::ifstream{CONFIG_FILE_PATH};
    if (!inputFile.good())
    {
        auto const createDirCommand = "mkdir -p " + CONFIG_DIR;
        system(createDirCommand.c_str());

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

void Settings::setAllowPermissions(bool allow)
{
    m_keyFile.set_boolean(GROUP_NETWORK, "allow_permissions", allow);
}

bool Settings::allowPermissions() const
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

double Settings::zoomLevel() const
{
    auto zoomLevel { 1.0 };
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
