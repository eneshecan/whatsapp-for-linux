#include "MainWindow.hpp"


int main(int argc, char** argv)
{
    auto const app = Gtk::Application::create(argc, argv, "com.github.whatsapp-for-linux");

    auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/MainWindow.ui");

    MainWindow* mainWindow = nullptr;
    refBuilder->get_widget_derived("main_window", mainWindow);

    return app->run(*mainWindow);
}