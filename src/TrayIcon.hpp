#pragma once

#include "MainWindow.hpp"
#include <gtkmm/menu.h>
#include <libappindicator/app-indicator.h>


class TrayIcon
{
    public:
        TrayIcon(MainWindow* window, Glib::RefPtr<Gtk::Builder> const& refBuilder);

    public:
        void set_active(bool active);

    private:
        void onOpen(MainWindow* window);
        void onQuit();

    private:
        AppIndicator*        t_trayIcon;
};
