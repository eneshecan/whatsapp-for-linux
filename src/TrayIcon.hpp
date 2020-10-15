#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <libappindicator/app-indicator.h>


class TrayIcon
{
    public:
        TrayIcon(Gtk::ApplicationWindow* window, Glib::RefPtr<Gtk::Builder> const& refBuilder);

    public:
        void set_active(bool active);
        bool active() const;

    private:
        void onOpen(Gtk::ApplicationWindow* window);
        void onQuit(Gtk::ApplicationWindow* window);

    private:
        AppIndicator* m_trayIcon;
};
