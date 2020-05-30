#include "WebView.hpp"
#include <gtkmm/messagedialog.h>
#include "Settings.hpp"


namespace
{
    constexpr auto const WHATSAPP_WEB_URI = "https://web.whatsapp.com/";


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
                Settings::instance().setAllowPermissions(true);
                break;
            case Gtk::RESPONSE_NO:
                webkit_permission_request_deny(request);
                Settings::instance().setAllowPermissions(false);
                break;
            default:
                break;
        }

        return TRUE;
    }

    gboolean contextMenu(WebKitWebView*, WebKitContextMenu*, GdkEvent*, WebKitHitTestResult*, gpointer)
    {
        return TRUE;
    }

    void initializeNotificationPermission(WebKitWebContext* context, gpointer)
    {
        if (Settings::instance().allowPermissions())
        {
            auto const origin = webkit_security_origin_new_for_uri(WHATSAPP_WEB_URI);
            auto const allowedOrigins = g_list_alloc();
            allowedOrigins->data = origin;

            webkit_web_context_initialize_notification_permissions(context, allowedOrigins, nullptr);
        }
    }
}


WebView::WebView()
    : Gtk::Widget{webkit_web_view_new()}
{
    auto const webContext = webkit_web_view_get_context(*this);

    g_signal_connect(*this, "permission-request", G_CALLBACK(permissionRequest), this);
    g_signal_connect(*this, "context-menu", G_CALLBACK(contextMenu), this);
    g_signal_connect(webContext, "initialize-notification-permissions", G_CALLBACK(initializeNotificationPermission), this);

    webkit_web_view_load_uri(*this, WHATSAPP_WEB_URI);
}

WebView::operator WebKitWebView*()
{
    return WEBKIT_WEB_VIEW(gobj());
}

void WebView::refresh()
{
    webkit_web_view_reload(*this);
}
