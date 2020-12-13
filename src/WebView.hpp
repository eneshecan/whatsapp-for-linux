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
        void   refresh();
        void   zoomIn();
        void   zoomOut();
        double zoomLevel() const;

    public:
        sigc::signal<void, double> signalZoomLevel() const;

    private:
        double                     m_zoomLevel;
        sigc::signal<void, double> m_signalZoomLevel;
};
