#include "TrayIcon.hpp"


TrayIcon::TrayIcon(Gtk::Window* window, Glib::RefPtr<Gtk::Builder> const& refBuilder)
    : t_trayIcon{app_indicator_new("whatsapp-for-linux", "whatsapp-for-linux", APP_INDICATOR_CATEGORY_COMMUNICATIONS)}
{
    Gtk::Menu* menu = nullptr;
    refBuilder->get_widget("indicator_menu", menu);
    
    Gtk::MenuItem* quitMenuItem = nullptr;
    refBuilder->get_widget("quit_indmenu_item", quitMenuItem);
    quitMenuItem->signal_activate().connect(sigc::mem_fun(this, &TrayIcon::onQuit));
    
    Gtk::MenuItem* openMenuItem = nullptr;
    refBuilder->get_widget("open_indmenu_item", openMenuItem);
    openMenuItem->signal_activate().connect(sigc::bind(sigc::mem_fun(this, &TrayIcon::onOpen), window));

    app_indicator_set_status(t_trayIcon, APP_INDICATOR_STATUS_PASSIVE);
    app_indicator_set_menu(t_trayIcon, GTK_MENU(menu->gobj()));
    app_indicator_set_attention_icon (t_trayIcon, "whatsapp-for-linux");
}

void TrayIcon::set_active(bool active)
{
    app_indicator_set_status(t_trayIcon, active ? APP_INDICATOR_STATUS_ACTIVE : APP_INDICATOR_STATUS_PASSIVE);
}

void TrayIcon::onOpen(Gtk::Window* window)
{
    window->set_skip_taskbar_hint(false);
    window->present();
}

void TrayIcon::onQuit()
{
    exit(0);
}
