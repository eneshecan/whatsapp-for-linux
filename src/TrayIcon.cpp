#include "TrayIcon.hpp"
#include <gtkmm/icontheme.h>

TrayIcon::TrayIcon()
    : m_appIndicator{app_indicator_new("com.github.whatsapp-for-linux.indicator", "whatsapp-for-linux-indicator", APP_INDICATOR_CATEGORY_COMMUNICATIONS)}
    , m_popupMenu{}
    , m_signalActivate{}
    , m_signalQuit{}
{
    auto const pixbuf = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/16x16/apps/whatsapp-for-linux.png");
    Gtk::IconTheme::get_default()->add_builtin_icon("whatsapp-for-linux-indicator", Gtk::ICON_SIZE_MENU, pixbuf);

    auto const showMenuItem = Gtk::make_managed<Gtk::MenuItem>("Open");
    auto const quitMenuItem = Gtk::make_managed<Gtk::MenuItem>("Quit");
    m_popupMenu.append(*showMenuItem);
    m_popupMenu.append(*quitMenuItem);

    app_indicator_set_menu(m_appIndicator, m_popupMenu.gobj());

    showMenuItem->signal_activate().connect([this]{ m_signalActivate.emit(); });
    quitMenuItem->signal_activate().connect([this]{ m_signalQuit.emit(); });

    m_popupMenu.show_all();

    app_indicator_set_status(m_appIndicator, APP_INDICATOR_STATUS_PASSIVE);
    app_indicator_set_menu(m_appIndicator, m_popupMenu.gobj());
}

void TrayIcon::setVisible(bool visible)
{
    app_indicator_set_status(m_appIndicator, (visible ? APP_INDICATOR_STATUS_ACTIVE : APP_INDICATOR_STATUS_PASSIVE));
}

bool TrayIcon::visible() const
{
    return (app_indicator_get_status(m_appIndicator) == APP_INDICATOR_STATUS_ACTIVE);
}

sigc::signal<void> TrayIcon::signalActivate() const
{
    return m_signalActivate;
}

sigc::signal<void> TrayIcon::signalQuit() const
{
    return m_signalQuit;
}
