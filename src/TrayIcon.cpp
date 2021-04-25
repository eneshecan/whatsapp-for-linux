#include "TrayIcon.hpp"
#include <utility>
#include <gtkmm/icontheme.h>

namespace
{
    std::pair<char const*, char const*> getTrayIconNames()
    {
        constexpr auto const WHATSAPP_TRAY                     = "whatsapp-tray";
        constexpr auto const WHATSAPP_TRAY_ATTENTION           = "whatsapp-tray-attention";
        constexpr auto const WHATSAPP_FOR_LINUX_TRAY           = "whatsapp-for-linux-tray";
        constexpr auto const WHATSAPP_FOR_LINUX_TRAY_ATTENTION = "whatsapp-for-linux-tray-attention";

        auto const iconTheme = Gtk::IconTheme::get_default();
        if (iconTheme->has_icon(WHATSAPP_TRAY) && iconTheme->has_icon(WHATSAPP_TRAY_ATTENTION))
        {
            return {WHATSAPP_TRAY, WHATSAPP_TRAY_ATTENTION};
        }
        else if (iconTheme->has_icon(WHATSAPP_FOR_LINUX_TRAY) && iconTheme->has_icon(WHATSAPP_FOR_LINUX_TRAY_ATTENTION))
        {
            return {WHATSAPP_FOR_LINUX_TRAY, WHATSAPP_FOR_LINUX_TRAY_ATTENTION};
        }
        else
        {
            auto const trayPixbuf = Gdk::Pixbuf::create_from_resource("/main/image/tray/whatsapp-for-linux-tray.png");
            auto const attentionPixbuf = Gdk::Pixbuf::create_from_resource("/main/image/tray/whatsapp-for-linux-tray-attention.png");
            iconTheme->add_builtin_icon(WHATSAPP_FOR_LINUX_TRAY, Gtk::ICON_SIZE_MENU, trayPixbuf);
            iconTheme->add_builtin_icon(WHATSAPP_FOR_LINUX_TRAY_ATTENTION, Gtk::ICON_SIZE_MENU, attentionPixbuf);
            if (iconTheme->has_icon(WHATSAPP_FOR_LINUX_TRAY) && iconTheme->has_icon(WHATSAPP_FOR_LINUX_TRAY_ATTENTION))
            {
                return {WHATSAPP_FOR_LINUX_TRAY, WHATSAPP_FOR_LINUX_TRAY_ATTENTION};
            }
            else
            {
                return {"indicator-messages", "indicator-messages-new"};
            }
        }
    }
}

TrayIcon::TrayIcon()
    : m_appIndicator{app_indicator_new("com.github.whatsapp-for-linux.tray", "", APP_INDICATOR_CATEGORY_COMMUNICATIONS)}
    , m_popupMenu{}
    , m_signalOpen{}
    , m_signalAbout{}
    , m_signalQuit{}
{
    auto const [trayIconName, attentionIconName] = getTrayIconNames();
    app_indicator_set_icon_full(m_appIndicator, trayIconName, "Whatsapp for Linux Tray");
    app_indicator_set_attention_icon_full(m_appIndicator, attentionIconName, "Whatsapp for Linux Tray Attention");

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
