#pragma once

#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/switch.h>
#include <gtkmm/comboboxtext.h>
#include "TrayIcon.hpp"
#include "WebView.hpp"

namespace wfl::ui
{
    class PreferencesWindow
        : public Gtk::Window
    {
        public:
            PreferencesWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder, TrayIcon& trayIcon, WebView& webView);

        private:
            bool onCloseToTray(bool state);
            bool onStartInTray(bool state);
            bool onAutostart(bool state);
            bool onAllowPermissions(bool state);
            void onHardwareAcceleration();

        private:
            TrayIcon*          m_trayIcon;
            WebView*           m_webView;

            Gtk::Switch*       m_switchCloseToTray;
            Gtk::Switch*       m_switchStartInTray;
            Gtk::Switch*       m_switchAutostart;
            Gtk::ComboBoxText* m_comboboxHwAccel;
            Gtk::Switch*       m_switchAllowPermissions;
    };
}
