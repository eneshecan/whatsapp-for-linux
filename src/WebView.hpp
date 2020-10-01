#pragma once

#include <gtkmm/widget.h>
#include <webkit2/webkit2.h>


class WebView
    : public Gtk::Widget
{
    public:
        WebView();
        ~WebView() override;

    public:
        operator WebKitWebView*();

    public:
        void refresh();
        void zoomIn();
        void zoomOut();

    private:
        double m_zoomLevel;
};
