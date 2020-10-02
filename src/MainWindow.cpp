#include "MainWindow.hpp"
#include "Version.hpp"
#include "Settings.hpp"
#include <gtkmm/grid.h>
#include <gtkmm/aboutdialog.h>


MainWindow::MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder, Glib::RefPtr<Gtk::Application> const& app)
    : Gtk::Window{cobject}
    , m_trayIcon{TrayIcon{this, refBuilder}}
    , m_fullscreen{false}
    , m_closetotray{Settings::instance().closeToTray()}
{
    set_default_size(1280, 720);

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
    refreshButton->set_image_from_icon_name("view-refresh");
    refreshButton->set_always_show_image();
    refreshButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onRefresh));

    Gtk::Button* headerMenuButton = nullptr;
    refBuilder->get_widget("header_menu_button", headerMenuButton);
    headerMenuButton->set_image_from_icon_name("view-more");
    headerMenuButton->set_always_show_image();

    Gtk::CheckMenuItem* closetotrayMenuItem = nullptr;
    refBuilder->get_widget("closetotray_menu_item", closetotrayMenuItem);
    closetotrayMenuItem->set_active(m_closetotray);
    m_trayIcon.set_active(m_closetotray);
    closetotrayMenuItem->signal_toggled().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onClosetotray), closetotrayMenuItem));

    Gtk::MenuItem* quitMenuItem = nullptr;
    refBuilder->get_widget("quit_menu_item", quitMenuItem);
    quitMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onQuit));

    Gtk::MenuItem* fullscreenMenuItem = nullptr;
    refBuilder->get_widget("fullscreen_menu_item", fullscreenMenuItem);
    fullscreenMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onFullscreen));

    Gtk::MenuItem* aboutMenuItem = nullptr;
    refBuilder->get_widget("about_menu_item", aboutMenuItem);
    aboutMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onAbout));

    Gtk::MenuItem* zoomInMenuItem = nullptr;
    refBuilder->get_widget("zoomin_menu_item", zoomInMenuItem);
    zoomInMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onZoomIn));

    Gtk::MenuItem* zoomOutMenuItem = nullptr;
    refBuilder->get_widget("zoomout_menu_item", zoomOutMenuItem);
    zoomOutMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onZoomOut));

    signal_window_state_event().connect(sigc::mem_fun(this, &MainWindow::onWindowStateEvent));
    signal_delete_event().connect(sigc::bind(sigc::mem_fun(this, &MainWindow::onClose), app));

    show_all();
}

bool MainWindow::onWindowStateEvent(GdkEventWindowState* event)
{
    m_fullscreen = (event->new_window_state & GDK_WINDOW_STATE_FULLSCREEN);
    return false;
}

bool MainWindow::onClose(GdkEventAny* event, Glib::RefPtr<Gtk::Application> const& app)
{
    if ( m_closetotray == true ) {
        app->hold();
        hide();
        return true;
    } else {
        return false;
    }
}

void MainWindow::onRefresh()
{
    m_webView.refresh();
}

void MainWindow::onQuit()
{
    exit(0);
}

void MainWindow::onClosetotray(Gtk::CheckMenuItem* item)
{
    m_closetotray = item->get_active();
    Settings::instance().setCloseToTray(m_closetotray);
    m_trayIcon.set_active(m_closetotray);
}

void MainWindow::onFullscreen()
{
    m_fullscreen ? unfullscreen() : fullscreen();
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
    aboutDialog.set_logo_icon_name("whatsapp-for-linux");
    aboutDialog.set_comments("An unofficial WhatsApp linux client desktop application.");
    aboutDialog.set_website("https://github.com/eneshecan/whatsapp-for-linux");
    aboutDialog.set_website_label("Github Repo");
    aboutDialog.set_license_type(Gtk::LICENSE_GPL_3_0);

    aboutDialog.run();
}
