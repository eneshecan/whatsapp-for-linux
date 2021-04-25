#include "TrayIcon.hpp"
#include <gtkmm/icontheme.h>

TrayIcon::TrayIcon()
    : m_appIndicator{app_indicator_new("com.github.whatsapp-for-linux.tray", "", APP_INDICATOR_CATEGORY_COMMUNICATIONS)}
    , m_popupMenu{}
    , m_signalOpen{}
    , m_signalAbout{}
    , m_signalQuit{}
{
    auto const trayPixbuf = Gdk::Pixbuf::create_from_resource("/main/image/tray/whatsapp-for-linux-tray.png");
    auto const attentionPixbuf = Gdk::Pixbuf::create_from_resource("/main/image/tray/whatsapp-for-linux-tray-attention.png");
    Gtk::IconTheme::get_default()->add_builtin_icon("whatsapp-for-linux-tray", Gtk::ICON_SIZE_MENU, trayPixbuf);
    Gtk::IconTheme::get_default()->add_builtin_icon("whatsapp-for-linux-tray-attention", Gtk::ICON_SIZE_MENU, attentionPixbuf);
    app_indicator_set_icon(m_appIndicator, "whatsapp-for-linux-tray");
    app_indicator_set_attention_icon(m_appIndicator, "whatsapp-for-linux-tray-attention");

    auto const openMenuItem = Gtk::manage(new Gtk::MenuItem{"Open"});
    auto const aboutMenuItem = Gtk::manage(new Gtk::MenuItem{"About"});
    auto const quitMenuItem = Gtk::manage(new Gtk::MenuItem{"Quit"});
    m_popupMenu.append(*openMenuItem);
    m_popupMenu.append(*aboutMenuItem);
    m_popupMenu.append(*quitMenuItem);

    app_indicator_set_menu(m_appIndicator, m_popupMenu.gobj());

    openMenuItem->signal_activate().connect([this]{ m_signalOpen.emit(); });
    aboutMenuItem->signal_activate().connect([this]{ m_signalAbout.emit(); });
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
    return (app_indicator_get_status(m_appIndicator) != APP_INDICATOR_STATUS_PASSIVE);
}

void TrayIcon::setAttention(bool attention)
{
    if (!visible())
    {
        return;
    }

    app_indicator_set_status(m_appIndicator, (attention ? APP_INDICATOR_STATUS_ATTENTION : APP_INDICATOR_STATUS_ACTIVE));
}

sigc::signal<void> TrayIcon::signalOpen() const noexcept
{
    return m_signalOpen;
}

sigc::signal<void> TrayIcon::signalAbout() const noexcept
{
    return m_signalAbout;
}

sigc::signal<void> TrayIcon::signalQuit() const noexcept
{
    return m_signalQuit;
}
