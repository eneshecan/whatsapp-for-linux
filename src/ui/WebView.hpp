#pragma once

#include <gtkmm/widget.h>
#include <webkit2/webkit2.h>

namespace wfl::ui
{
    namespace detail
    {
        void loadChanged(WebKitWebView*, WebKitLoadEvent loadEvent, gpointer userData);
    }

    class WebView : public Gtk::Widget
    {
        public:
            WebView();
            ~WebView() override;

            operator WebKitWebView*();

        public:
            void            refresh();
            WebKitLoadEvent getLoadStatus() const noexcept;
            void            setHwAccelPolicy(WebKitHardwareAccelerationPolicy policy);
            void            sendRequest(std::string url);
            void            openPhoneNumber(std::string const& phoneNumber);
            void            zoomIn();
            void            zoomOut();
            void            resetZoom();
            double          getZoomLevel();
            std::string     getZoomLevelString();
            void            setMinFontSize(unsigned int fontSize);

            sigc::signal<void, WebKitLoadEvent> signalLoadStatus() const noexcept;
            sigc::signal<void, bool>            signalNotification() const noexcept;
            sigc::signal<void>                  signalNotificationClicked() const noexcept;

        private:
            void onLoadStatusChanged(WebKitLoadEvent loadEvent);
            bool onTimeout();

            friend void detail::loadChanged(WebKitWebView*, WebKitLoadEvent, gpointer);

        private:
            WebKitLoadEvent                     m_loadStatus;
            bool                                m_stoppedResponding;
            sigc::signal<void, WebKitLoadEvent> m_signalLoadStatus;
            sigc::signal<void, bool>            m_signalNotification;
            sigc::signal<void>                  m_signalNotificationClicked;
    };
}
