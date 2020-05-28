#include "MainWindow.h"


int main(int argc, char** argv)
{
    auto app = Gtk::Application::create(argc, argv, {});

    auto refBuilder = Gtk::Builder::create_from_file("ui/MainWindow.ui");

    MainWindow* mainWindow = nullptr;
    refBuilder->get_widget_derived("main_window", mainWindow);

    return app->run(*mainWindow);
}