#include "TrayIcon.hpp"

// TODO Find a nice way to get our icon path
TrayIcon::TrayIcon()
    : m_appIndicator{app_indicator_new("com.github.whatsapp-for-linux.app-indicator", "/usr/local/share/icons/hicolor/16x16/apps/whatsapp-for-linux.png", APP_INDICATOR_CATEGORY_COMMUNICATIONS)}
    , m_popupMenu{}
    , m_signalActivate{}
    , m_signalQuit{}
{
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
