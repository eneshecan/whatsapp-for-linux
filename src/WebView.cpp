#include "WebView.hpp"
#include <gtkmm/messagedialog.h>


namespace
{
    gboolean permissionRequest(WebKitWebView*, WebKitPermissionRequest* request, GtkWindow*)
    {
        // TODO Think about handling this in MainWindow by signaling and holding a ref to the request in WebView.
        auto dialog = Gtk::MessageDialog{"Notification Request", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO};
        dialog.set_secondary_text("Do you allow WhatsApp to send you notifications?");

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
