#pragma once

#include "WebView.hpp"
#include <gtkmm/window.h>
#include <gtkmm/builder.h>


class MainWindow
    : public Gtk::Window
{
    public:
        MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder);
        ~MainWindow() override = default;

    private:
        bool onWindowStateEvent(GdkEventWindowState* event);

    private:
        void onRefresh();
        void onQuit();
        void onFullscreen();
        void onAbout();
        void onZoomIn();
        void onZoomOut();

    private:
        WebView             m_webView;
        bool                m_fullscreen;
};
