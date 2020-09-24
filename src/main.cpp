#include "MainWindow.hpp"
#include "gtkmm.h"
#include "/usr/include/libappindicator3-0.1/libappindicator/app-indicator.h"


int main(int argc, char** argv)
{
    auto const app = Gtk::Application::create(argc, argv, "com.github.whatsapp-for-linux");

    auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/MainWindow.ui");

    auto pMainWindow = std::unique_ptr<MainWindow>{};

    MainWindow* mainWindow = nullptr;
    refBuilder->get_widget_derived("main_window", mainWindow);
    pMainWindow.reset(mainWindow);
    
    AppIndicator* indicator = 
        app_indicator_new("whatsapp-for-linux", "whatsapp-for-linux", APP_INDICATOR_CATEGORY_HARDWARE);
    Gtk::Menu indicatorMenu;
    Gtk::MenuItem quit("Quit");
 
    indicatorMenu.append(quit);
 
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_menu(indicator, indicatorMenu.gobj());
    app_indicator_set_attention_icon (indicator, "whatsapp-for-linux");

    return app->run(*mainWindow);
}
