#include "MainWindow.hpp"
#include "Application.hpp"
#include "Version.hpp"
#include "Settings.hpp"
#include <gtkmm/menuitem.h>
#include <gtkmm/grid.h>
#include <gtkmm/aboutdialog.h>

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

    Gtk::CheckMenuItem* closeToTrayMenuItem = nullptr;
    refBuilder->get_widget("close_to_tray_menu_item", closeToTrayMenuItem);
    closeToTrayMenuItem->signal_toggled().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onCloseToTray), closeToTrayMenuItem));

    Gtk::MenuItem* fullscreenMenuItem = nullptr;
    refBuilder->get_widget("fullscreen_menu_item", fullscreenMenuItem);
    fullscreenMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onFullscreen));

    Gtk::MenuItem* zoomInMenuItem = nullptr;
    refBuilder->get_widget("zoomin_menu_item", zoomInMenuItem);
    zoomInMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onZoomIn));

    Gtk::MenuItem* zoomOutMenuItem = nullptr;
    refBuilder->get_widget("zoomout_menu_item", zoomOutMenuItem);
    zoomOutMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onZoomOut));

    Gtk::MenuItem* aboutMenuItem = nullptr;
    refBuilder->get_widget("about_menu_item", aboutMenuItem);
    aboutMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onAbout));

    Gtk::MenuItem* quitMenuItem = nullptr;
    refBuilder->get_widget("quit_menu_item", quitMenuItem);
    quitMenuItem->signal_activate().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onQuit), false));

    m_trayIcon.signalActivate().connect(sigc::mem_fun(this, &MainWindow::onShow));
    m_trayIcon.signalQuit().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onQuit), true));

    show_all();

    m_trayIcon.setVisible(Settings::instance().closeToTray());
    closeToTrayMenuItem->set_active(m_trayIcon.visible());

    m_headerBar->set_visible(Settings::instance().headerBar());
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
            return Gtk::ApplicationWindow::on_key_press_event(keyEvent);
    }
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

void MainWindow::onQuit(bool immediate)
{
    if (immediate && m_trayIcon.visible())
    {
        m_trayIcon.setVisible(false);
    }
    close();
}

void MainWindow::onFullscreen()
{
    m_fullscreen ? unfullscreen() : fullscreen();
}

void MainWindow::onCloseToTray(Gtk::CheckMenuItem* menuItem)
{
    auto const visible = menuItem->get_active();
    m_trayIcon.setVisible(visible);
    Settings::instance().setCloseToTray(visible);
}

void MainWindow::onZoomIn()
{
    m_webView.zoomIn();
}

void MainWindow::onZoomOut()
{
    m_webView.zoomOut();
}

void MainWindow::onAbout()
{
    auto aboutDialog = Gtk::AboutDialog{};

    aboutDialog.set_title("About");
    aboutDialog.set_version(VERSION);
    aboutDialog.set_program_name("whatsapp-for-linux");
    aboutDialog.set_comments("An unofficial WhatsApp linux client desktop application.");
    aboutDialog.set_website("https://github.com/eneshecan/whatsapp-for-linux");
    aboutDialog.set_website_label("Github Repo");
    aboutDialog.set_license_type(Gtk::LICENSE_GPL_3_0);

    aboutDialog.run();
}
