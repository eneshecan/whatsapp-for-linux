#pragma once

#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/menu.h>
#include <libappindicator/app-indicator.h>


class TrayIcon
{
    public:
        TrayIcon(Gtk::Window* window, Glib::RefPtr<Gtk::Builder> const& refBuilder);

    public:
        void set_active(bool active);

    private:
        void onOpen(Gtk::Window* window);
        void onQuit();

    private:
        AppIndicator*        t_trayIcon;
};
