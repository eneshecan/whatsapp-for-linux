#pragma once

#include "gtkmm/statusicon.h"
#include "gtkmm/menu.h"

class TrayIcon
    : private Gtk::StatusIcon
{
    public:
        TrayIcon();
        ~TrayIcon() override = default;

    public:
        void setVisible(bool visible);
        bool visible() const;

    public:
        sigc::signal<void> signalActivate() const;
        sigc::signal<void> signalQuit() const;

    private:
        void on_popup_menu(guint button, guint32 activate_time) override;

    private:
        Gtk::Menu          m_popupMenu;
        sigc::signal<void> m_signalActivate;
        sigc::signal<void> m_signalQuit;
};
