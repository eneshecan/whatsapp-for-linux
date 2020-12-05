#include <iostream>
#include "MainWindow.hpp"

int main(int argc, char** argv)
{
    auto const app = Gtk::Application::create(argc, argv, "com.github.whatsapp-for-linux");

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
        std::cerr << "Failed load ui resource: " << error.what() << std::endl;
        return 1;
    }

    return app->run(*mainWindow);
}
