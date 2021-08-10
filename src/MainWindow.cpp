#include "MainWindow.hpp"
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/modelbutton.h>
#include <gtkmm/aboutdialog.h>
#include "Application.hpp"
#include "Version.hpp"
#include "Settings.hpp"

MainWindow::MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder)
    : Gtk::ApplicationWindow{cobject}
    , m_trayIcon{}
    , m_webView{}
    , m_fullscreen{false}
{
    auto const appIcon16x16   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/16x16/apps/whatsapp-for-linux.png");
    auto const appIcon32x32   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/32x32/apps/whatsapp-for-linux.png");
    auto const appIcon64x64   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/64x64/apps/whatsapp-for-linux.png");
    auto const appIcon128x128 = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/128x128/apps/whatsapp-for-linux.png");
    set_icon_list({appIcon16x16, appIcon32x32, appIcon64x64, appIcon128x128});
    set_default_icon(appIcon64x64);

    Gtk::Grid* mainGrid = nullptr;
    refBuilder->get_widget("main_grid", mainGrid);
    mainGrid->attach(m_webView, 0, 1, 1, 1);

    Gtk::Button* refreshButton = nullptr;
    refBuilder->get_widget("refresh_button", refreshButton);
    refreshButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onRefresh));

    refBuilder->get_widget("header_bar", m_headerBar);

    Gtk::ModelButton* startInTrayButton = nullptr;
    refBuilder->get_widget("start_in_tray_button", startInTrayButton);
    startInTrayButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onStartInTray), startInTrayButton));

    Gtk::ModelButton* autostartButton = nullptr;
    refBuilder->get_widget("autostart_button", autostartButton);
    autostartButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onAutostart), autostartButton));

    Gtk::ModelButton* closeToTrayButton = nullptr;
    refBuilder->get_widget("close_to_tray_button", closeToTrayButton);
    closeToTrayButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onCloseToTray), closeToTrayButton, startInTrayButton));

    Gtk::Button* fullscreenButton = nullptr;
    refBuilder->get_widget("fullscreen_button", fullscreenButton);
    fullscreenButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onFullscreen));

    Gtk::Label* zoomLevelLabel = nullptr;
    refBuilder->get_widget("zoom_level_label", zoomLevelLabel);
    zoomLevelLabel->set_label(m_webView.getZoomLevelString());

    Gtk::Button* zoomInButton = nullptr;
    refBuilder->get_widget("zoom_in_button", zoomInButton);
    zoomInButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onZoomIn), zoomLevelLabel));

    Gtk::Button* zoomOutButton = nullptr;
    refBuilder->get_widget("zoom_out_button", zoomOutButton);
    zoomOutButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onZoomOut), zoomLevelLabel));

    Gtk::Button* aboutButton = nullptr;
    refBuilder->get_widget("about_button", aboutButton);
    aboutButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onAbout));

    Gtk::Button* quitButton = nullptr;
    refBuilder->get_widget("quit_button", quitButton);
    quitButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onQuit));

    m_webView.signalNotification().connect(sigc::mem_fun(&m_trayIcon, &TrayIcon::setAttention));
    m_trayIcon.signalOpen().connect(sigc::mem_fun(this, &MainWindow::onShow));
    m_trayIcon.signalAbout().connect(sigc::mem_fun(this, &MainWindow::onAbout));
    m_trayIcon.signalQuit().connect(sigc::mem_fun(this, &MainWindow::onQuit));

    show_all();

    m_trayIcon.setVisible(Settings::getInstance().getCloseToTray());
    closeToTrayButton->property_active() = m_trayIcon.isVisible();
    startInTrayButton->property_active() = Settings::getInstance().getStartInTray() && m_trayIcon.isVisible();
    startInTrayButton->set_sensitive(m_trayIcon.isVisible());
    autostartButton->property_active() = Settings::getInstance().getAutostart();

    m_headerBar->set_visible(Settings::getInstance().getHeaderBar());
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
                Settings::getInstance().setHeaderBar(visible);
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

void MainWindow::onShow()
{
    if (!is_visible())
    {
        Application::getInstance().add_window(*this);
        show();
    }
}

void MainWindow::onQuit()
{
    if (m_trayIcon.isVisible())
    {
        m_trayIcon.setVisible(false);
    }
    close();
}

void MainWindow::onFullscreen()
{
    m_fullscreen ? unfullscreen() : fullscreen();
}

void MainWindow::onCloseToTray(Gtk::ModelButton* closeToTrayButton, Gtk::ModelButton* startInTrayButton)
{
    auto const visible = !closeToTrayButton->property_active();
    closeToTrayButton->property_active() = visible;

    m_trayIcon.setVisible(visible);
    Settings::getInstance().setCloseToTray(visible);

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

    Settings::getInstance().setStartInTray(visible);
}

void MainWindow::onAutostart(Gtk::ModelButton* autostartButton)
{
    auto const autostart = !autostartButton->property_active();
    autostartButton->property_active() = autostart;

    Settings::getInstance().setAutostart(autostart);
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

void MainWindow::onAbout()
{
    auto aboutDialog = Gtk::AboutDialog{};

    aboutDialog.set_title("About");
    aboutDialog.set_version(VERSION);
    aboutDialog.set_program_name("Whatsapp for Linux");
    aboutDialog.set_comments("An unofficial WhatsApp desktop application for linux.");
    aboutDialog.set_website("https://github.com/eneshecan/whatsapp-for-linux");
    aboutDialog.set_website_label("Github Repository");
    aboutDialog.set_license_type(Gtk::LICENSE_GPL_3_0);

    aboutDialog.run();
}
