#pragma once


#include <gtkmm/widget.h>
#include <webkit2/webkit2.h>


class WebView
    : public Gtk::Widget
{
    public:
        WebView();
        ~WebView() override = default;

        operator WebKitWebView*();

    public:
        void loadUri(const Glib::ustring& uri);
        void refresh();

    public:
        sigc::signal<void, Glib::ustring> signalUriChanged();
        sigc::signal<void, bool>          signalLoadingChanged();

    private:
        sigc::signal<void, Glib::ustring> m_signalUriChanged;
        sigc::signal<void, bool>          m_signalLoadingChanged;
};
