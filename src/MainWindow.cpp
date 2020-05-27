#include "MainWindow.h"
#include <gtkmm/grid.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/main.h>


namespace
{
    constexpr auto const WHATSAPP_WEB_URI = "https://web.whatsapp.com/";
}


MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Window{cobject}
    , m_fullscreen{false}
{
    set_default_size(1280, 720);

    auto const appIcon = Gdk::Pixbuf::create_from_file("icon.ico");
    // TODO Use set_icon_list instead.
    set_icon(appIcon);

    Gtk::Grid* mainGrid = nullptr;
    refBuilder->get_widget("main_grid", mainGrid);
    mainGrid->attach(m_webView, 0, 1);
    m_webView.set_vexpand();

    Gtk::MenuItem* refreshMenuItem = nullptr;
    refBuilder->get_widget("refresh_menu_item", refreshMenuItem);
    refreshMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onRefresh));

    Gtk::MenuItem* quitMenuItem = nullptr;
    refBuilder->get_widget("quit_menu_item", quitMenuItem);
    quitMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onQuit));

    Gtk::MenuItem* fullscreenMenuItem = nullptr;
    refBuilder->get_widget("fullscreen_menu_item", fullscreenMenuItem);
    fullscreenMenuItem->signal_activate().connect(sigc::mem_fun(this, &MainWindow::onFullscreen));

    signal_window_state_event().connect(sigc::mem_fun(this, &MainWindow::onWindowStateEvent));

    m_webView.loadUri(WHATSAPP_WEB_URI);
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
