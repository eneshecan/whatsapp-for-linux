#pragma once

#include <gtkmm/widget.h>
#include <webkit2/webkit2.h>


class WebView
    : public Gtk::Widget
{
    public:
        WebView();
        ~WebView() override = default;

    public:
        operator WebKitWebView*();

    public:
        void refresh();
        void setDarkMode(bool enable);
        bool darkMode() const;

    private:
        bool m_darkMode;
};
