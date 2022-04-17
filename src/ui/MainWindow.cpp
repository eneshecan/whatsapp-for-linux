#include "MainWindow.hpp"
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/modelbutton.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/shortcutswindow.h>
#include "Application.hpp"
#include "Config.hpp"
#include "../util/Settings.hpp"

namespace wfl::ui
{
    MainWindow::MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder)
        : Gtk::ApplicationWindow{cobject}
        , m_trayIcon{}
        , m_webView{}
        , m_pendingUrl{}
        , m_phoneNumberDialog{nullptr}
        , m_headerBar{nullptr}
        , m_shortcutsWindow{nullptr}
        , m_fullscreen{false}
    {
        auto const appIcon16x16   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/16x16/apps/"   WFL_ICON ".png");
        auto const appIcon32x32   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/32x32/apps/"   WFL_ICON ".png");
        auto const appIcon64x64   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/64x64/apps/"   WFL_ICON ".png");
        auto const appIcon128x128 = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/128x128/apps/" WFL_ICON ".png");
        set_icon_list({appIcon16x16, appIcon32x32, appIcon64x64, appIcon128x128});
        set_default_icon(appIcon64x64);

        Gtk::Grid* mainGrid = nullptr;
        refBuilder->get_widget("main_grid", mainGrid);
        mainGrid->attach(m_webView, 0, 1, 1, 1);

        Gtk::Button* openPhoneNumberButton = nullptr;
        refBuilder->get_widget("open_phone_number_button", openPhoneNumberButton);
        openPhoneNumberButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onOpenPhoneNumber));

        Gtk::Button* refreshButton = nullptr;
        refBuilder->get_widget("refresh_button", refreshButton);
        refreshButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onRefresh));

        refBuilder->get_widget("header_bar", m_headerBar);

        Gtk::ModelButton* startInTrayButton = nullptr;
        refBuilder->get_widget("start_in_tray_button", startInTrayButton);
        startInTrayButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::onStartInTray), startInTrayButton));

        Gtk::ModelButton* autostartButton = nullptr;
        refBuilder->get_widget("autostart_button", autostartButton);
        autostartButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::onAutostart), autostartButton));

        Gtk::ModelButton* closeToTrayButton = nullptr;
        refBuilder->get_widget("close_to_tray_button", closeToTrayButton);
        closeToTrayButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::onCloseToTray), closeToTrayButton, startInTrayButton));

        Gtk::ModelButton* fullscreenButton = nullptr;
        refBuilder->get_widget("fullscreen_button", fullscreenButton);
        fullscreenButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onFullscreen));

        Gtk::Label* zoomLevelLabel = nullptr;
        refBuilder->get_widget("zoom_level_label", zoomLevelLabel);
        zoomLevelLabel->set_label(m_webView.getZoomLevelString());

        Gtk::Button* zoomInButton = nullptr;
        refBuilder->get_widget("zoom_in_button", zoomInButton);
        zoomInButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::onZoomIn), zoomLevelLabel));

        Gtk::Button* zoomOutButton = nullptr;
        refBuilder->get_widget("zoom_out_button", zoomOutButton);
        zoomOutButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::onZoomOut), zoomLevelLabel));

        Gtk::ModelButton* shortcutsButton = nullptr;
        refBuilder->get_widget("shortcuts_button", shortcutsButton);
        shortcutsButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onShortcuts));

        Gtk::ModelButton* aboutButton = nullptr;
        refBuilder->get_widget("about_button", aboutButton);
        aboutButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onAbout));

        Gtk::ModelButton* quitButton = nullptr;
        refBuilder->get_widget("quit_button", quitButton);
        quitButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onQuit));

        m_webView.signalLoadStatus().connect(sigc::mem_fun(*this, &MainWindow::onLoadStatusChanged));
        m_webView.signalNotification().connect(sigc::mem_fun(*this, &MainWindow::onNotificationChanged));
        m_webView.signalNotificationClicked().connect(sigc::mem_fun(*this, &MainWindow::onShow));
        m_trayIcon.signalShow().connect(sigc::mem_fun(*this, &MainWindow::onShow));
        m_trayIcon.signalAbout().connect(sigc::mem_fun(*this, &MainWindow::onAbout));
        m_trayIcon.signalQuit().connect(sigc::mem_fun(*this, &MainWindow::onQuit));

        show_all();

        m_trayIcon.setVisible(util::Settings::getInstance().getCloseToTray());
        closeToTrayButton->property_active() = m_trayIcon.isVisible();
        startInTrayButton->property_active() = util::Settings::getInstance().getStartInTray() && m_trayIcon.isVisible();
        startInTrayButton->set_sensitive(m_trayIcon.isVisible());
        autostartButton->property_active() = util::Settings::getInstance().getAutostart();

        m_headerBar->set_visible(util::Settings::getInstance().getHeaderBar());
    }

    void MainWindow::openUrl(std::string const& url)
    {
        if (m_webView.getLoadStatus() == WEBKIT_LOAD_FINISHED)
        {
            m_webView.sendRequest(url);
        }
        else
        {
            m_pendingUrl = url;
        }
    }

    bool MainWindow::on_key_press_event(GdkEventKey* keyEvent)
    {
        switch (keyEvent->keyval)
        {
            case GDK_KEY_F11:
                onFullscreen();
                return true;

            case GDK_KEY_H:
            case GDK_KEY_h:
                if (keyEvent->state & GDK_MOD1_MASK)
                {
                    auto const visible = !m_headerBar->is_visible();
                    m_headerBar->set_visible(visible);
                    util::Settings::getInstance().setHeaderBar(visible);
                    return true;
                }
                break;

            case GDK_KEY_Q:
            case GDK_KEY_q:
                if (keyEvent->state & GDK_CONTROL_MASK)
                {
                    onQuit();
                    return true;
                }
                break;

            case GDK_KEY_question:
                if (keyEvent->state & GDK_CONTROL_MASK)
                {
                    onShortcuts();
                    return true;
                }
                break;

            default:
                break;
        }

        return Gtk::ApplicationWindow::on_key_press_event(keyEvent);
    }

    bool MainWindow::on_window_state_event(GdkEventWindowState *windowStateEvent)
    {
        m_fullscreen = (windowStateEvent->new_window_state & GDK_WINDOW_STATE_FULLSCREEN);

        return Gtk::ApplicationWindow::on_window_state_event(windowStateEvent);
    }

    bool MainWindow::on_delete_event(GdkEventAny* any_event)
    {
        if (m_trayIcon.isVisible())
        {
            Application::getInstance().keepAlive();
            hide();
        }
        else
        {
            Application::getInstance().endKeepAlive();
        }

        return false;
    }

    void MainWindow::onRefresh()
    {
        m_webView.refresh();
    }

    void MainWindow::onLoadStatusChanged(WebKitLoadEvent loadEvent)
    {
        if ((loadEvent == WEBKIT_LOAD_FINISHED) && !m_pendingUrl.empty())
        {
            m_webView.sendRequest(m_pendingUrl);
            m_pendingUrl.clear();
        }
    }

    void MainWindow::onOpenPhoneNumber()
    {
        if (!m_phoneNumberDialog)
        {
            auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/PhoneNumberDialog.ui");
            refBuilder->get_widget_derived("phone_number_dialog", m_phoneNumberDialog);

            m_phoneNumberDialog->signal_response().connect(sigc::mem_fun(*this, &MainWindow::onPhoneNumberDialogResponse));
        }

        m_phoneNumberDialog->set_transient_for(*this);
        m_phoneNumberDialog->show_all();
    }

    void MainWindow::onPhoneNumberDialogResponse(int responseId)
    {
        if (responseId == Gtk::ResponseType::RESPONSE_OK)
        {
            auto const phoneNumber = m_phoneNumberDialog->getPhoneNumber();
            m_webView.openPhoneNumber(phoneNumber);
        }
    }

    void MainWindow::onNotificationChanged(bool attention)
    {
        if (!is_visible())
        {
            m_trayIcon.setAttention(attention);
        }
    }

    void MainWindow::onShow()
    {
        if (!is_visible())
        {
            m_trayIcon.setAttention(false);
            Application::getInstance().add_window(*this);
        }

        present();
    }

    void MainWindow::onQuit()
    {
        if (m_trayIcon.isVisible())
        {
            m_trayIcon.setVisible(false);
        }
        close();
    }

    void MainWindow::onCloseToTray(Gtk::ModelButton* closeToTrayButton, Gtk::ModelButton* startInTrayButton)
    {
        auto const visible = !closeToTrayButton->property_active();
        closeToTrayButton->property_active() = visible;

        m_trayIcon.setVisible(visible);
        util::Settings::getInstance().setCloseToTray(visible);

        if (!visible)
        {
            startInTrayButton->property_active() = false;
        }
        startInTrayButton->set_sensitive(visible);
    }

    void MainWindow::onStartInTray(Gtk::ModelButton* startInTrayButton)
    {
        auto const visible = !startInTrayButton->property_active();
        startInTrayButton->property_active() = visible;

        util::Settings::getInstance().setStartInTray(visible);
    }

    void MainWindow::onAutostart(Gtk::ModelButton* autostartButton)
    {
        auto const autostart = !autostartButton->property_active();
        autostartButton->property_active() = autostart;

        util::Settings::getInstance().setAutostart(autostart);
    }

    void MainWindow::onFullscreen()
    {
        m_fullscreen ? unfullscreen() : fullscreen();
    }

    void MainWindow::onZoomIn(Gtk::Label* zoomLevelLabel)
    {
        m_webView.zoomIn();
        zoomLevelLabel->set_label(m_webView.getZoomLevelString());
    }

    void MainWindow::onZoomOut(Gtk::Label* zoomLevelLabel)
    {
        m_webView.zoomOut();
        zoomLevelLabel->set_label(m_webView.getZoomLevelString());
    }

    void MainWindow::onShortcuts()
    {
        if (!m_shortcutsWindow)
        {
            auto const refBuilder = Gtk::Builder::create_from_resource("/main/ui/ShortcutsWindow.ui");
            refBuilder->get_widget("shortcuts_window", m_shortcutsWindow);
        }

        m_shortcutsWindow->set_transient_for(*this);
        m_shortcutsWindow->show_all();
    }

    void MainWindow::onAbout()
    {
        auto aboutDialog = Gtk::AboutDialog{};

        aboutDialog.set_title("About");
        aboutDialog.set_version(WFL_VERSION);
        aboutDialog.set_program_name(WFL_FRIENDLY_NAME);
        aboutDialog.set_comments(WFL_DESCRIPTION);
        aboutDialog.set_website(WFL_HOMEPAGE);
        aboutDialog.set_website_label("Github Repository");
        aboutDialog.set_license_type(Gtk::LICENSE_GPL_3_0);

        aboutDialog.set_transient_for(*this);
        aboutDialog.run();
    }
}
