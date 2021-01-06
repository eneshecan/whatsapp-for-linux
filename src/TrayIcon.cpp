#include "TrayIcon.hpp"
#include <gtkmm/icontheme.h>

TrayIcon::TrayIcon()
    : m_appIndicator{app_indicator_new("com.github.whatsapp-for-linux.tray", "whatsapp-for-linux-tray-active", APP_INDICATOR_CATEGORY_COMMUNICATIONS)}
    , m_popupMenu{}
    , m_signalActivate{}
    , m_signalQuit{}
{
    auto const activePixbuf = Gdk::Pixbuf::create_from_resource("/main/image/tray/active.png");
    auto const attentionPixbuf = Gdk::Pixbuf::create_from_resource("/main/image/tray/attention.png");
    Gtk::IconTheme::get_default()->add_builtin_icon("whatsapp-for-linux-tray-active", Gtk::ICON_SIZE_MENU, activePixbuf);
    Gtk::IconTheme::get_default()->add_builtin_icon("whatsapp-for-linux-tray-attention", Gtk::ICON_SIZE_MENU, attentionPixbuf);
    app_indicator_set_attention_icon(m_appIndicator, "whatsapp-for-linux-tray-attention");

    auto const showMenuItem = Gtk::manage(new Gtk::MenuItem{"Open"});
    auto const quitMenuItem = Gtk::manage(new Gtk::MenuItem{"Quit"});
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
    return (app_indicator_get_status(m_appIndicator) != APP_INDICATOR_STATUS_PASSIVE);
}

sigc::signal<void> TrayIcon::signalActivate() const noexcept
{
    return m_signalActivate;
}

sigc::signal<void> TrayIcon::signalQuit() const noexcept
{
    return m_signalQuit;
}

void TrayIcon::setAttention(bool attention)
{
    if (!visible())
    {
        return;
    }

    app_indicator_set_status(m_appIndicator, (attention ? APP_INDICATOR_STATUS_ATTENTION : APP_INDICATOR_STATUS_ACTIVE));
}
