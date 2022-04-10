#include <cstdio>
#include <clocale>
#include <unistd.h>
#include <iostream>
#include "Config.hpp"
#include "ui/Application.hpp"
#include "ui/MainWindow.hpp"
#include "util/Settings.hpp"

namespace
{
    void redirectOutputToLogger()
    {
        auto const fl = ::popen("logger -i -s -t " WFL_NAME, "w");
        if (!fl)
        {
            auto const errorNumber = errno;
            std::cerr << "Failed to open pipe to logger: " << strerror(errorNumber) << std::endl;
            return;
        }

        auto const fd = ::fileno(fl);
        ::dup2(fd, STDERR_FILENO);
    }

    void sigterm(int)
    {
        wfl::ui::Application::getInstance().quit();
    }
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");

    redirectOutputToLogger();

    auto app = wfl::ui::Application{argc, argv, WFL_APP_ID, Gio::APPLICATION_HANDLES_OPEN};

    signal(SIGINT,  sigterm);
    signal(SIGTERM, sigterm);
    signal(SIGPIPE, SIG_IGN);

    auto mainWindow = std::unique_ptr<wfl::ui::MainWindow>{};

    try
    {
        auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/MainWindow.ui");

        wfl::ui::MainWindow* window = nullptr;
        refBuilder->get_widget_derived("main_window", window);
        mainWindow.reset(window);
    }
    catch (Glib::Exception const& error)
    {
        std::cerr << "Failed to load ui resource: " << error.what() << std::endl;
        return 1;
    }

    app.signal_open().connect([&app, &mainWindow](Gtk::Application::type_vec_files const& files, const Glib::ustring&)
    {
        if (!files.empty())
        {
            // Activate the application if it's not running
            app.activate();
            mainWindow->openUrl(files.at(0U)->get_uri());
        }
    });

    auto retCode = 0;

    if (wfl::util::Settings::getInstance().getStartInTray() && wfl::util::Settings::getInstance().getCloseToTray())
    {
        mainWindow->hide();
        wfl::ui::Application::getInstance().keepAlive();
        retCode = app.run();
    }
    else
    {
        retCode = app.run(*mainWindow);
    }

    return retCode;
}
