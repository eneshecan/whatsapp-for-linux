#pragma once

#include "TrayIcon.hpp"
#include "WebView.hpp"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/checkmenuitem.h>

class MainWindow
    : public Gtk::ApplicationWindow
{
    public:
        MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder);
        ~MainWindow() override = default;

    protected:
        bool on_key_release_event(GdkEventKey* keyEvent) override;
        bool on_window_state_event(GdkEventWindowState* windowStateEvent) override;
        bool on_delete_event(GdkEventAny* any_event) override;

    private:
        void onRefresh();
        void onShow();
        void onQuit(bool immediate);
        void onCloseToTray(Gtk::CheckMenuItem* menuItem);
        void onFullscreen();
        void onAbout();
        void onZoomIn();
        void onZoomOut();

    private:
        TrayIcon            m_trayIcon;
        WebView             m_webView;
        Gtk::HeaderBar*     m_headerBar;
        bool                m_fullscreen;
};
