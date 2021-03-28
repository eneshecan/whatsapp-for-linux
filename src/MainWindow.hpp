#pragma once

#include "TrayIcon.hpp"
#include "WebView.hpp"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/headerbar.h>

class MainWindow
    : public Gtk::ApplicationWindow
{
    public:
        MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder);

    protected:
        bool on_key_press_event(GdkEventKey* keyEvent) override;
        bool on_key_release_event(GdkEventKey* keyEvent) override;
        bool on_window_state_event(GdkEventWindowState* windowStateEvent) override;
        bool on_delete_event(GdkEventAny* any_event) override;

    private:
        void onRefresh();
        void onShow();
        void onQuit();
        bool onCloseToTray(bool state);
        bool onStartInTray(bool visible);
        void onFullscreen();
        void onAbout();
        void onZoomIn(Gtk::Label* zoomLevelLabel);
        void onZoomOut(Gtk::Label* zoomLevelLabel);

    private:
        TrayIcon        m_trayIcon;
        WebView         m_webView;
        Gtk::HeaderBar* m_headerBar;
        bool            m_fullscreen;
};
