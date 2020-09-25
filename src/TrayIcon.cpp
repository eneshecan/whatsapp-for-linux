#include "TrayIcon.hpp"


TrayIcon::TrayIcon(MainWindow* window, Glib::RefPtr<Gtk::Builder> const& refBuilder)
{
    AppIndicator* trayIcon = 
        app_indicator_new("whatsapp-for-linux", "whatsapp-for-linux", APP_INDICATOR_CATEGORY_COMMUNICATIONS);
    Gtk::Menu* menu = nullptr;
    refBuilder->get_widget("indicator_menu", menu);
    
    Gtk::MenuItem* quitMenuItem = nullptr;
    refBuilder->get_widget("quit_indmenu_item", quitMenuItem);
    quitMenuItem->signal_activate().connect(sigc::mem_fun(this, &TrayIcon::onQuit));
    
    Gtk::MenuItem* openMenuItem = nullptr;
    refBuilder->get_widget("open_indmenu_item", openMenuItem);
    openMenuItem->signal_activate().connect(sigc::bind(sigc::mem_fun(this, &TrayIcon::onOpen), window));

    app_indicator_set_status(trayIcon, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_menu(trayIcon, GTK_MENU(menu->gobj()));
    app_indicator_set_attention_icon (trayIcon, "whatsapp-for-linux");
};

void TrayIcon::onOpen(MainWindow* window) {
    window->set_skip_taskbar_hint(false);
    window->present();
}

void TrayIcon::onQuit() {
	exit(0);
}
