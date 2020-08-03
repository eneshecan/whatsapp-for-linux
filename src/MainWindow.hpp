#pragma once

#include "WebView.hpp"
#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkmenuitem.h>


class MainWindow
    : public Gtk::Window
{
    public:
        MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder);
        ~MainWindow() override;

    private:
        bool onWindowStateEvent(GdkEventWindowState* event);

    private:
        void onRefresh();
        void onQuit();
        void onFullscreen();
        void onAbout();

    private:
        WebView             m_webView;
        bool                m_fullscreen;
};
