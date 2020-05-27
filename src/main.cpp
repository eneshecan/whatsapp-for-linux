#include "MainWindow.h"


int main(int argc, char** argv)
{
    auto app = Gtk::Application::create(argc, argv, {});

    auto refBuilder = Gtk::Builder::create_from_file("MainWindow.ui");

    MainWindow* pMainWindow = nullptr;
    refBuilder->get_widget_derived("main_window", pMainWindow);

    return app->run(*pMainWindow);
}