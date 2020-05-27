#include "WebView.h"
#include <gtkmm/messagedialog.h>


namespace
{
    static void loadChanged(WebKitWebView* webView, WebKitLoadEvent loadEvent, gpointer userData)
    {
        auto webViewWidget = reinterpret_cast<WebView*>(userData);
        if (!webViewWidget)
        {
            return;
        }

        switch(loadEvent)
        {
            case WEBKIT_LOAD_STARTED:
                webViewWidget->signalLoadingChanged().emit(true);
                break;
            case WEBKIT_LOAD_COMMITTED:
                webViewWidget->signalUriChanged().emit(webkit_web_view_get_uri(webView));
                break;
            case WEBKIT_LOAD_FINISHED:
                webViewWidget->signalLoadingChanged().emit(false);
                break;
            default: break;
        }
    }

    static gboolean permissionRequest(WebKitWebView*, WebKitPermissionRequest* request, GtkWindow*)
    {
        auto dialog = Gtk::MessageDialog{"Notification Request", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO};
        dialog.set_secondary_text("Do you want WhatsApp to send you notifications?");

        auto const result = dialog.run();
        switch (result)
        {
            case Gtk::RESPONSE_YES:
                webkit_permission_request_allow(request);
                break;
            case Gtk::RESPONSE_NO:
                webkit_permission_request_deny(request);
                break;
            default:
                break;
        }

        return TRUE;
    }
}


WebView::WebView()
    : Gtk::Widget{webkit_web_view_new()}
{
    g_signal_connect(*this, "load-changed", G_CALLBACK(loadChanged), this);
    g_signal_connect(*this, "permission-request", G_CALLBACK(permissionRequest), this);
}

WebView::operator WebKitWebView*()
{
    return WEBKIT_WEB_VIEW(gobj());
}

void WebView::loadUri(const Glib::ustring &uri)
{
    webkit_web_view_load_uri(*this, uri.c_str());
}

void WebView::refresh()
{
    webkit_web_view_reload(*this);
}

sigc::signal<void, Glib::ustring> WebView::signalUriChanged()
{
    return m_signalUriChanged;
}

sigc::signal<void, bool>  WebView::signalLoadingChanged()
{
    return m_signalLoadingChanged;
}
