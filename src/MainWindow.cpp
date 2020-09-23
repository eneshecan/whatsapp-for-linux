#include "MainWindow.hpp"
#include "Version.hpp"
#include "Settings.hpp"
#include <gtkmm/grid.h>
#include <gtkmm/aboutdialog.h>

Gtk::Label* zoomLevelLabel = nullptr;

MainWindow::MainWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder)
    : Gtk::Window{cobject}
    , m_fullscreen{false}
{
    set_default_size(1280, 720);

    auto const appIcon16x16   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/16x16/apps/whatsapp-for-linux.png");
    auto const appIcon32x32   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/32x32/apps/whatsapp-for-linux.png");
    auto const appIcon64x64   = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/64x64/apps/whatsapp-for-linux.png");
    auto const appIcon128x128 = Gdk::Pixbuf::create_from_resource("/main/image/icons/hicolor/128x128/apps/whatsapp-for-linux.png");
    set_icon_list({appIcon16x16, appIcon32x32, appIcon64x64, appIcon128x128});

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
    headerMenuButton->set_image_from_icon_name("start-here");
    headerMenuButton->set_always_show_image();

    refBuilder->get_widget("zoom_level_label", zoomLevelLabel);

    Gtk::Button* zoomOutButton = nullptr;
    refBuilder->get_widget("zoom_out_button", zoomOutButton);
    zoomOutButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onZoomOut));

    Gtk::Button* zoomInButton = nullptr;
    refBuilder->get_widget("zoom_in_button", zoomInButton);
    zoomInButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onZoomIn));

    Gtk::Button* quitButton = nullptr;
    refBuilder->get_widget("quit_button", quitButton);
    quitButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onQuit));

    Gtk::Button* fullscreenButton = nullptr;
    refBuilder->get_widget("fullscreen_button", fullscreenButton);
    fullscreenButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onFullscreen));

    Gtk::Button* aboutButton = nullptr;
    refBuilder->get_widget("about_button", aboutButton);
    aboutButton->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::onAbout));

    signal_window_state_event().connect(sigc::mem_fun(this, &MainWindow::onWindowStateEvent));

    show_all();
}

bool MainWindow::onWindowStateEvent(GdkEventWindowState* event)
{
    m_fullscreen = event->new_window_state & GDK_WINDOW_STATE_FULLSCREEN;
    return false;
}

void MainWindow::onRefresh()
{
    m_webView.refresh();
}

void MainWindow::onQuit()
{
    close();
}

void MainWindow::onFullscreen()
{
    m_fullscreen ? unfullscreen() : fullscreen();
}

void MainWindow::onZoomIn()
{
    double newZoomLevel { m_webView.zoomIn() };
    newZoomLevel *= 100;

    zoomLevelLabel->set_text(std::to_string(static_cast<int>(newZoomLevel)) + "%");
}

void MainWindow::onZoomOut()
{
    double newZoomLevel { m_webView.zoomOut() };
    newZoomLevel *= 100;

    zoomLevelLabel->set_text(std::to_string(static_cast<int>(newZoomLevel)) + "%");
}

void MainWindow::onAbout()
{
    auto aboutDialog = Gtk::AboutDialog{};

    aboutDialog.set_title("About");
    aboutDialog.set_version(VERSION);
    aboutDialog.set_program_name("whatsapp-for-linux");
    aboutDialog.set_logo_icon_name("help-about");
    aboutDialog.set_comments("An unofficial WhatsApp linux client desktop application.");
    aboutDialog.set_website("https://github.com/eneshecan/whatsapp-for-linux");
    aboutDialog.set_website_label("Github Repo");
    aboutDialog.set_license_type(Gtk::LICENSE_GPL_3_0);

    aboutDialog.run();
}
