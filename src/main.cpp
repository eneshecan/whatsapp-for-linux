#include <clocale>
#include <glibmm/i18n.h>
#include "Config.hpp"
#include "ui/Application.hpp"
#include "util/Helper.hpp"

namespace
{
    void sigterm(int)
    {
        wfl::ui::Application::getInstance().quit();
    }
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");

    bindtextdomain(GETTEXT_PACKAGE, WFL_LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

    auto app = wfl::ui::Application{argc, argv};

    wfl::util::redirectOutputToLogger();

    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);
    signal(SIGPIPE, SIG_IGN);

    return app.run();
}
