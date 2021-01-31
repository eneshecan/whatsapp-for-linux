#pragma once

#include <libappindicator/app-indicator.h>
#include <gtkmm/menu.h>

class TrayIcon
{
    public:
        TrayIcon();

    public:
        void setVisible(bool visible);
        bool visible() const;
        void setAttention(bool attention);

    public:
        sigc::signal<void> signalActivate() const noexcept;
        sigc::signal<void> signalQuit() const noexcept;

    private:
        AppIndicator*      m_appIndicator;
        Gtk::Menu          m_popupMenu;
        sigc::signal<void> m_signalActivate;
        sigc::signal<void> m_signalQuit;
};
