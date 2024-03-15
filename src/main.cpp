#include <clocale>
#include <glibmm/i18n.h>
#include "Config.hpp"
#include "ui/Application.hpp"
#include "util/Helper.hpp"
#include <gtkmm/main.h>
#include <glibmm/optioncontext.h>
#include <glibmm/optiongroup.h>
#include <iostream>

namespace
{
    void sigterm(int)
    {
        wfl::ui::Application::getInstance().quit();
    }
}

void handleVersionCommandLineOption(int argc, char** argv, bool& version)
{
    Glib::OptionGroup   optionGroup("options", "Command Line Options", "Show command line options");
    Glib::OptionContext optionContext;

    Glib::OptionEntry versionEntry;
    versionEntry.set_long_name("version");
    versionEntry.set_short_name('v');
    versionEntry.set_description("Show the version of the program");
    versionEntry.set_arg_description("VERSION");
    optionGroup.add_entry(versionEntry, version);

    optionContext.set_main_group(optionGroup);

    Gtk::Main kit(argc, argv, optionContext);
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");

    bindtextdomain(GETTEXT_PACKAGE, WFL_LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

    bool version = false;
    handleVersionCommandLineOption(argc, argv, version);

    if (version)
    {
        std::cout << "Version: " << WFL_VERSION << std::endl;
        return 0;
    }

    auto app = wfl::ui::Application{argc, argv};

    wfl::util::redirectOutputToLogger();

    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);
    signal(SIGPIPE, SIG_IGN);

    return app.run();
}