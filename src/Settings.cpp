#include "Settings.hpp"
#include <fstream>
#include <iostream>


namespace
{
    constexpr auto const FILE_NAME     = "settings.ini";
    constexpr auto const GROUP_NETWORK = "network";
}


Settings& Settings::instance()
{
    static auto instance = Settings{};
    return instance;
}

Settings::Settings()
{
    auto inputFile = std::ifstream{FILE_NAME};
    if (!inputFile.good())
    {
        auto outputFile = std::ofstream{FILE_NAME};
        outputFile << '[' << GROUP_NETWORK << ']';
    }

    keyFile.load_from_file(FILE_NAME);
}

Settings::~Settings()
{
    keyFile.save_to_file(FILE_NAME);
}

void Settings::setAllowPermissions(bool allow)
{
    keyFile.set_boolean(GROUP_NETWORK, "allow_permissions", allow);
}

bool Settings::allowPermissions() const
{
    auto allow = false;
    try
    {
        allow = keyFile.get_boolean(GROUP_NETWORK, "allow_permissions");
    }
    catch (Glib::KeyFileError const& error)
    {
        std::cerr << "Settings: " <<  error.what() << std::endl;
    }

    return allow;
}
