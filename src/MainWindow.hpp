#pragma once

#include "WebView.hpp"
#include "TrayIcon.hpp"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkmenuitem.h>


class MainWindow
    : public Gtk::ApplicationWindow
{
    public:
        MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder);
        virtual ~MainWindow() override;

    private:
        bool onWindowStateEvent(GdkEventWindowState* event);
        bool onDelete(GdkEventAny*);

    private:
        void onRefresh();
        void onQuit();
        void onCloseToTray(Gtk::CheckMenuItem* item);
        void onFullscreen();
        void onAbout();
        void onZoomIn();
        void onZoomOut();

    private:
        WebView  m_webView;
        TrayIcon m_trayIcon;
        bool     m_fullscreen;
};
