#include "TrayIcon.hpp"

TrayIcon::TrayIcon()
    : Gtk::StatusIcon{Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/16x16/apps/whatsapp-for-linux.png")}
{
    auto const showMenuItem = Gtk::make_managed<Gtk::MenuItem>("Open");
    auto const quitMenuItem = Gtk::make_managed<Gtk::MenuItem>("Quit");
    m_popupMenu.append(*showMenuItem);
    m_popupMenu.append(*quitMenuItem);

    showMenuItem->signal_activate().connect([this]{ m_signalActivate.emit(); });
    signal_activate().connect([this](){ m_signalActivate.emit(); });
    quitMenuItem->signal_activate().connect([this]{ m_signalQuit.emit(); });

    m_popupMenu.show_all();
}

void TrayIcon::setVisible(bool visible)
{
    set_visible(visible);
}

bool TrayIcon::visible() const
{
    return get_visible();
}

sigc::signal<void> TrayIcon::signalActivate() const
{
    return m_signalActivate;
}

sigc::signal<void> TrayIcon::signalQuit() const
{
    return m_signalQuit;
}

void TrayIcon::on_popup_menu(guint button, guint32 activate_time)
{
    m_popupMenu.popup(button, activate_time);
}
