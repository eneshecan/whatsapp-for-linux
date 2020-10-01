#pragma once

#include "WebView.hpp"
#include "TrayIcon.hpp"
#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkmenuitem.h>


class MainWindow
    : public Gtk::Window
{
    public:
        MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder);
        ~MainWindow() override = default;

    private:
        bool onWindowStateEvent(GdkEventWindowState* event);
        bool onClose(GdkEventAny* event);

    private:
        void onRefresh();
        void onQuit();
        void onClosetotray(Gtk::CheckMenuItem* item);
        void onFullscreen();
        void onAbout();
        void onZoomIn();
        void onZoomOut();

    private:
        WebView             m_webView;
        TrayIcon            m_trayIcon;
        bool                m_fullscreen;
        bool                m_closetotray;
};
