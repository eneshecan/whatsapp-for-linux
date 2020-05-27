#pragma once

#include "WebView.h"
#include <gtkmm/window.h>
#include <gtkmm/builder.h>


class MainWindow
    : public Gtk::Window
{
    public:
        MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
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
