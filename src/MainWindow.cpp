#include "MainWindow.hpp"
#include "Application.hpp"
#include "Version.hpp"
#include "Settings.hpp"
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/switch.h>
#include <gtkmm/aboutdialog.h>

namespace
{
    std::string zoomLevelToStr(double level)
    {
        return std::to_string(static_cast<int>(std::round(level * 100))).append("%");
    }
}

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
    m_webView.set_vexpand();

    Gtk::Button* refreshButton = nullptr;
    refBuilder->get_widget("refresh_button", refreshButton);
    refreshButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onRefresh));

    refBuilder->get_widget("header_bar", m_headerBar);

    Gtk::Switch* closeToTraySwitch = nullptr;
    refBuilder->get_widget("close_to_tray_switch", closeToTraySwitch);
    closeToTraySwitch->signal_state_set().connect(sigc::mem_fun(this, &MainWindow::onCloseToTray), false);

    Gtk::Switch* startInTraySwitch = nullptr;
    refBuilder->get_widget("start_in_tray_switch", startInTraySwitch);
    startInTraySwitch->signal_state_set().connect(sigc::mem_fun(this, &MainWindow::onStartInTray), false);

    Gtk::Button* fullscreenButton = nullptr;
    refBuilder->get_widget("fullscreen_button", fullscreenButton);
    fullscreenButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onFullscreen));

    Gtk::Label* zoomLevelLabel = nullptr;
    refBuilder->get_widget("zoom_level_label", zoomLevelLabel);
    zoomLevelLabel->set_label(zoomLevelToStr(m_webView.zoomLevel()));

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
    m_trayIcon.signalActivate().connect(sigc::mem_fun(this, &MainWindow::onShow));
    m_trayIcon.signalQuit().connect(sigc::mem_fun(this, &MainWindow::onQuit));

    show_all();

    m_trayIcon.setVisible(Settings::instance().closeToTray() || Settings::instance().startInTray());
    startInTraySwitch->set_state(Settings::instance().startInTray());
    closeToTraySwitch->set_state(Settings::instance().closeToTray());

    m_headerBar->set_visible(Settings::instance().headerBar());
}

bool MainWindow::on_key_press_event(GdkEventKey* keyEvent)
{
    switch (keyEvent->keyval)
    {
        case GDK_KEY_F11:
            onFullscreen();
            return true;

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

bool MainWindow::on_key_release_event(GdkEventKey* keyEvent)
{
    switch (keyEvent->keyval)
    {
        case GDK_KEY_Alt_L:
        {
            auto const visible = !m_headerBar->is_visible();
            m_headerBar->set_visible(visible);
            Settings::instance().setHeaderBar(visible);
            return true;
        }

        default:
            break;
    }

    return Gtk::ApplicationWindow::on_key_release_event(keyEvent);
}


bool MainWindow::on_window_state_event(GdkEventWindowState *windowStateEvent)
{
    m_fullscreen = (windowStateEvent->new_window_state & GDK_WINDOW_STATE_FULLSCREEN);

    return Gtk::ApplicationWindow::on_window_state_event(windowStateEvent);
}

bool MainWindow::on_delete_event(GdkEventAny* any_event)
{
    if (m_trayIcon.visible())
    {
        Application::instance().keepAlive();
        hide();
    }
    else
    {
        Application::instance().endKeepAlive();
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
        Application::instance().add_window(*this);
        show();
    }
}

void MainWindow::onQuit()
{
    if (m_trayIcon.visible())
    {
        m_trayIcon.setVisible(false);
    }
    close();
}

void MainWindow::onFullscreen()
{
    m_fullscreen ? unfullscreen() : fullscreen();
}

bool MainWindow::onCloseToTray(bool visible)
{
    m_trayIcon.setVisible(visible);
    Settings::instance().setCloseToTray(visible);

    return false;
}

bool MainWindow::onStartInTray(bool visible)
{
    Settings::instance().setStartInTray(visible);
    return false;
}

void MainWindow::onZoomIn(Gtk::Label* zoomLevelLabel)
{
    auto const level = m_webView.zoomIn();
    zoomLevelLabel->set_label(zoomLevelToStr(level));
}

void MainWindow::onZoomOut(Gtk::Label* zoomLevelLabel)
{
    auto const level = m_webView.zoomOut();
    zoomLevelLabel->set_label(zoomLevelToStr(level));
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
