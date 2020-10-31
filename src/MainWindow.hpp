#pragma once

#include "WebView.hpp"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/headerbar.h>


class MainWindow
    : public Gtk::ApplicationWindow
{
    public:
        MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder);
        ~MainWindow() override;

    protected:
        bool on_key_press_event(GdkEventKey* keyEvent) override;
        bool on_window_state_event(GdkEventWindowState* windowStateEvent) override;

    private:
        void onRefresh();
        void onQuit();
        void onFullscreen();
        void onAbout();
        void onZoomIn();
        void onZoomOut();

    private:
        WebView             m_webView;
        Gtk::HeaderBar*     m_headerBar;
        bool                m_headerBarVisible;
        bool                m_fullscreen;
};
