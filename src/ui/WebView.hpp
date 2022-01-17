#pragma once

#include <gtkmm/widget.h>
#include <webkit2/webkit2.h>

namespace wfl::ui
{
    namespace detail
    {
        void loadChanged(WebKitWebView*, WebKitLoadEvent loadEvent, gpointer userData);
    }

    class WebView
        : public Gtk::Widget
    {
        public:
            WebView();
            ~WebView() override;

        public:
            operator WebKitWebView*();

        public:
            void            refresh();
            WebKitLoadEvent getLoadStatus() const noexcept;
            void            sendRequest(std::string url);
            void            openPhoneNumber(std::string const& phoneNumber);
            void            zoomIn();
            void            zoomOut();
            double          getZoomLevel() const noexcept;
            std::string     getZoomLevelString() const noexcept;

        public:
            sigc::signal<void, WebKitLoadEvent> signalLoadStatus() const noexcept;
            sigc::signal<void, bool>            signalNotification() const noexcept;
            sigc::signal<void>                  signalNotificationClicked() const noexcept;

        private:
            void        setLoadStatus(WebKitLoadEvent loadEvent);
            friend void detail::loadChanged(WebKitWebView*, WebKitLoadEvent, gpointer);

        private:
            WebKitLoadEvent                     m_loadStatus;
            double                              m_zoomLevel;
            sigc::signal<void, WebKitLoadEvent> m_signalLoadStatus;
            sigc::signal<void, bool>            m_signalNotification;
            sigc::signal<void>                  m_signalNotificationClicked;
    };
}
