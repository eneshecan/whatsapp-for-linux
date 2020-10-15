#include "TrayIcon.hpp"
#include <gtkmm/menu.h>


TrayIcon::TrayIcon(Gtk::ApplicationWindow* window, Glib::RefPtr<Gtk::Builder> const& refBuilder)
    : m_trayIcon{app_indicator_new("whatsapp-for-linux", "whatsapp-for-linux", APP_INDICATOR_CATEGORY_COMMUNICATIONS)}
{
    Gtk::Menu* menu = nullptr;
    refBuilder->get_widget("indicator_menu", menu);
    
    Gtk::MenuItem* quitMenuItem = nullptr;
    refBuilder->get_widget("quit_indmenu_item", quitMenuItem);
    quitMenuItem->signal_activate().connect(sigc::bind(sigc::mem_fun(this, &TrayIcon::onQuit), window));
    
    Gtk::MenuItem* openMenuItem = nullptr;
    refBuilder->get_widget("open_indmenu_item", openMenuItem);
    openMenuItem->signal_activate().connect(sigc::bind(sigc::mem_fun(this, &TrayIcon::onOpen), window));

    app_indicator_set_status(m_trayIcon, APP_INDICATOR_STATUS_PASSIVE);
    app_indicator_set_menu(m_trayIcon, GTK_MENU(menu->gobj()));
    app_indicator_set_attention_icon(m_trayIcon, "whatsapp-for-linux");
}

void TrayIcon::set_active(bool active)
{
    app_indicator_set_status(m_trayIcon, active ? APP_INDICATOR_STATUS_ACTIVE : APP_INDICATOR_STATUS_PASSIVE);
}

bool TrayIcon::active() const
{
    return (app_indicator_get_status(m_trayIcon) == APP_INDICATOR_STATUS_ACTIVE);
}

void TrayIcon::onOpen(Gtk::ApplicationWindow* window)
{
    window->show();
}

void TrayIcon::onQuit(Gtk::ApplicationWindow* window)
{
    window->get_application()->quit();
}
