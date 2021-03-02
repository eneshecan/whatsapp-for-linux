#include <iostream>
#include "Application.hpp"
#include "MainWindow.hpp"

namespace
{
    void sigterm(int)
    {
        Application::instance().quit();
    }
}

int main(int argc, char** argv)
{
    auto app = Application{argc, argv, "com.github.whatsapp-for-linux"};

    signal(SIGINT,  sigterm);
    signal(SIGTERM, sigterm);
    signal(SIGPIPE, SIG_IGN);

    auto mainWindow = std::unique_ptr<MainWindow>{};
    try
    {
        auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/MainWindow.ui");

        MainWindow* window = nullptr;
        refBuilder->get_widget_derived("main_window", window);
        mainWindow.reset(window);
    }
    catch (Glib::Exception const& error)
    {
        std::cerr << "Failed to load ui resource: " << error.what() << std::endl;
        return 1;
    }

    return app.run(*mainWindow);
}
