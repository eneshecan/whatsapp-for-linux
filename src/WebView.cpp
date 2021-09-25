#include "WebView.hpp"
#include <iostream>
#include <string>
#include <optional>
#include <locale>
#include <gtkmm/messagedialog.h>
#include <gtkmm/filechooserdialog.h>
#include "Settings.hpp"

namespace
{
    constexpr auto const WHATSAPP_WEB_URI = "https://web.whatsapp.com";


    std::optional<std::string> getSystemLanguage()
    {
        try
        {
            auto const lang = std::locale{""}.name();
            return lang.substr(0, lang.find('.'));
        }
        catch (std::runtime_error const& error)
        {
            std::cerr << "WebView: Failed to get system language: " << error.what() << std::endl;
            return std::nullopt;
        }
    }

    gboolean permissionRequest(WebKitWebView*, WebKitPermissionRequest* request, GtkWindow*)
    {
        auto dialog = Gtk::MessageDialog{"Notification Request", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO};
        dialog.set_secondary_text("Do you allow WhatsApp to send you notifications?");

        auto const result = dialog.run();
        switch (result)
        {
            case Gtk::RESPONSE_YES:
                webkit_permission_request_allow(request);
                Settings::getInstance().setAllowPermissions(true);
                break;
            case Gtk::RESPONSE_NO:
                webkit_permission_request_deny(request);
                Settings::getInstance().setAllowPermissions(false);
                break;
            default:
                break;
        }

        return TRUE;
    }

    gboolean decidePolicy(WebKitWebView*, WebKitPolicyDecision* decision, WebKitPolicyDecisionType decisionType, gpointer)
    {
        switch (decisionType)
        {
            case WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION:
            {
                auto const navigationDecision = WEBKIT_NAVIGATION_POLICY_DECISION(decision);
                auto const navigationAction = webkit_navigation_policy_decision_get_navigation_action(navigationDecision);
                auto const request = webkit_navigation_action_get_request(navigationAction);
                auto const uri = webkit_uri_request_get_uri(request);

                if (GError* error = nullptr; !gtk_show_uri_on_window(nullptr, uri, GDK_CURRENT_TIME, &error))
                {
                    std::cerr << "WebView: Failed to show uri: " << error->message << std::endl;
                }
                return TRUE;
            }

            default:
                return FALSE;
        }
    }

    gboolean downloadDecideDestination(WebKitDownload* download, char* suggestedFilename, gpointer)
    {
        auto dialog = Gtk::FileChooserDialog{"Save File", Gtk::FILE_CHOOSER_ACTION_SAVE};
        dialog.add_button("Ok", Gtk::RESPONSE_OK);
        dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
        dialog.set_current_name(suggestedFilename);

        auto const result = dialog.run();
        switch (result)
        {
            case Gtk::RESPONSE_OK:
            {
                auto const destination = "file://" + dialog.get_filename();
                webkit_download_set_destination(download, destination.c_str());
                return TRUE;
            }

            case Gtk::RESPONSE_CANCEL:
            default:
                webkit_download_cancel(download);
                return FALSE;
        }
    }

    void downloadStarted(WebKitWebContext*, WebKitDownload* download, gpointer)
    {
        g_signal_connect(download, "decide-destination", G_CALLBACK(downloadDecideDestination), nullptr);
    }

    void initializeNotificationPermission(WebKitWebContext* context, gpointer)
    {
        if (Settings::getInstance().getAllowPermissions())
        {
            auto const origin = webkit_security_origin_new_for_uri(WHATSAPP_WEB_URI);
            auto const allowedOrigins = g_list_alloc();
            allowedOrigins->data = origin;

            webkit_web_context_initialize_notification_permissions(context, allowedOrigins, nullptr);
        }
        else
        {
            webkit_web_context_initialize_notification_permissions(context, nullptr, nullptr);
        }
    }

    void notificationDestroyed(WebKitNotification*, gpointer userData)
    {
        auto const webView = reinterpret_cast<WebView*>(userData);
        if (webView)
        {
            webView->signalNotification().emit(false);
        }
    }

    gboolean showNotification(WebKitWebView*, WebKitNotification* notification, gpointer userData)
    {
        auto const webView = reinterpret_cast<WebView*>(userData);
        if (webView)
        {
            webView->signalNotification().emit(true);
        }

        g_signal_connect(notification, "clicked", G_CALLBACK(notificationDestroyed), webView);
        g_signal_connect(notification, "closed", G_CALLBACK(notificationDestroyed), webView);

        return FALSE;
    }
}


WebView::WebView()
    : Gtk::Widget{webkit_web_view_new()}
    , m_zoomLevel{Settings::getInstance().getZoomLevel()}
    , m_signalNotification{}
{
    auto const webContext = webkit_web_view_get_context(*this);

    g_signal_connect(*this, "permission-request", G_CALLBACK(permissionRequest), nullptr);
    g_signal_connect(*this, "decide-policy", G_CALLBACK(decidePolicy), nullptr);
    g_signal_connect(*this, "show-notification", G_CALLBACK(showNotification), this);
    g_signal_connect(webContext, "download-started", G_CALLBACK(downloadStarted), nullptr);
    g_signal_connect(webContext, "initialize-notification-permissions", G_CALLBACK(initializeNotificationPermission), nullptr);

    if (auto const lang = getSystemLanguage(); lang.has_value())
    {
        gchar const* const spellCheckingLangs[] = {lang.value().c_str(), nullptr};
        webkit_web_context_set_spell_checking_enabled(webContext, TRUE);
        webkit_web_context_set_spell_checking_languages(webContext, spellCheckingLangs);
    }

    auto const settings = webkit_web_view_get_settings(*this);
    webkit_settings_set_enable_developer_extras(settings, TRUE);

    webkit_web_view_set_zoom_level(*this, m_zoomLevel);
    webkit_web_view_load_uri(*this, WHATSAPP_WEB_URI);
}

WebView::~WebView()
{
    Settings::getInstance().setZoomLevel(m_zoomLevel);
}

WebView::operator WebKitWebView*()
{
    return WEBKIT_WEB_VIEW(gobj());
}

void WebView::refresh()
{
    webkit_web_view_reload(*this);
}

void WebView::zoomIn()
{
    m_zoomLevel = webkit_web_view_get_zoom_level(*this);
    if (m_zoomLevel < 2)
    {
        m_zoomLevel += 0.05;
        webkit_web_view_set_zoom_level(*this, m_zoomLevel);
    }
}

void WebView::zoomOut()
{
    m_zoomLevel = webkit_web_view_get_zoom_level(*this);
    if (m_zoomLevel > 0.5)
    {
        m_zoomLevel -= 0.05;
        webkit_web_view_set_zoom_level(*this, m_zoomLevel);
    }
}

double WebView::getZoomLevel() const noexcept
{
    return m_zoomLevel;
}

std::string WebView::getZoomLevelString() const noexcept
{
    return std::to_string(static_cast<int>(std::round(getZoomLevel() * 100))).append("%");
}

sigc::signal<void, bool> WebView::signalNotification() const noexcept
{
    return m_signalNotification;
}
