#pragma once

#include <libappindicator/app-indicator.h>
#include <gtkmm/menu.h>

class TrayIcon
{
    public:
        TrayIcon();
        ~TrayIcon() = default;

    public:
        void setVisible(bool visible);
        bool visible() const;

    public:
        sigc::signal<void> signalActivate() const;
        sigc::signal<void> signalQuit() const;

    private:
        AppIndicator*      m_appIndicator;
        Gtk::Menu          m_popupMenu;
        sigc::signal<void> m_signalActivate;
        sigc::signal<void> m_signalQuit;
};
