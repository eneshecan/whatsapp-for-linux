#include "MainWindow.hpp"
#include "gtkmm.h"
#include "libappindicator/app-indicator.h"


void indMenuOpen(GtkMenu *menu, MainWindow window) {
    window.present();
}

void indMenuQuit(GtkMenu *menu, gpointer data) {
	exit(0);
}

int main(int argc, char** argv)
{
    auto const app = Gtk::Application::create(argc, argv, "com.github.whatsapp-for-linux");

    auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/MainWindow.ui");

    auto pMainWindow = std::unique_ptr<MainWindow>{};

    MainWindow* mainWindow = nullptr;
    refBuilder->get_widget_derived("main_window", mainWindow);
    pMainWindow.reset(mainWindow);
    g_signal_connect(mainWindow, "delete_event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);

    /* Indicator */
    AppIndicator* indicator = 
        app_indicator_new("whatsapp-for-linux", "whatsapp-for-linux", APP_INDICATOR_CATEGORY_HARDWARE);
    GtkWidget *menu, *item;
    menu = gtk_menu_new();

    /* Menu item Open WhatsApp */
    item = gtk_menu_item_new_with_label("Open WhatsApp");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
    g_signal_connect(item, "activate", G_CALLBACK(indMenuOpen), mainWindow);
    gtk_widget_show(item);

    /* Menu item Quit */
    item = gtk_menu_item_new_with_label("Quit");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
    g_signal_connect(item, "activate", G_CALLBACK(indMenuQuit), NULL);
    gtk_widget_show(item);

    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_menu(indicator, GTK_MENU(menu));
    app_indicator_set_attention_icon (indicator, "whatsapp-for-linux");

    return app->run(*mainWindow);
}
