#pragma once

#include <gtkmm/widget.h>
#include <webkit2/webkit2.h>

namespace wfl::ui
{
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
            // This is intended to use only from webkit's callback. Do not use externally!
            void            setLoadStatus(WebKitLoadEvent loadEvent);
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

        private:
            double                              m_zoomLevel;
            sigc::signal<void, WebKitLoadEvent> m_signalLoadStatus;
            sigc::signal<void, bool>            m_signalNotification;
            WebKitLoadEvent                     m_loadStatus;
    };
}
