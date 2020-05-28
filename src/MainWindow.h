#pragma once

#include "WebView.h"
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

    private:
        WebView m_webView;
        bool    m_fullscreen;
};
