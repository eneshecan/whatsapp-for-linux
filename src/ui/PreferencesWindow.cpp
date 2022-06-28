#include "PreferencesWindow.hpp"
#include "../util/Settings.hpp"

namespace wfl::ui
{
    PreferencesWindow::PreferencesWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder, TrayIcon& trayIcon, WebView& webView)
        : Gtk::Window{cobject}
        , m_trayIcon{&trayIcon}
        , m_webView{&webView}
        , m_switchStartInTray{nullptr}
        , m_comboboxHwAccel{nullptr}
    {
        Gtk::Switch* switchCloseToTray = nullptr;
        refBuilder->get_widget("switch_close_to_tray", switchCloseToTray);
        switchCloseToTray->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onCloseToTrayChanged), false);

        refBuilder->get_widget("switch_start_in_tray", m_switchStartInTray);
        m_switchStartInTray->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onStartInTrayChanged), false);

        Gtk::Switch* switchAutostart = nullptr;
        refBuilder->get_widget("switch_autostart", switchAutostart);
        switchAutostart->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onAutostartChanged), false);

        refBuilder->get_widget("combobox_hw_accel", m_comboboxHwAccel);
        m_comboboxHwAccel->signal_changed().connect(sigc::mem_fun(*this, &PreferencesWindow::onHwAccelChanged));
        m_comboboxHwAccel->append("On Demand");
        m_comboboxHwAccel->append("Always");
        m_comboboxHwAccel->append("Never");

        Gtk::Switch* switchAllowPermissions = nullptr;
        refBuilder->get_widget("switch_allow_permissions", switchAllowPermissions);
        switchAllowPermissions->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onAllowPermissionsChanged), false);

        switchCloseToTray->set_state(m_trayIcon->isVisible());
        m_switchStartInTray->set_state(util::Settings::getInstance().getStartInTray() && m_trayIcon->isVisible());
        m_switchStartInTray->set_sensitive(m_trayIcon->isVisible());
        switchAutostart->set_state(util::Settings::getInstance().getAutostart());
        m_comboboxHwAccel->set_active(util::Settings::getInstance().getHwAccel());
        switchAllowPermissions->set_state(util::Settings::getInstance().getAllowPermissions());
    }

    bool PreferencesWindow::onCloseToTrayChanged(bool state)
    {
        m_trayIcon->setVisible(state);
        util::Settings::getInstance().setCloseToTray(state);

        if (!state)
        {
            m_switchStartInTray->set_state(false);
        }
        m_switchStartInTray->set_sensitive(state);

        return false;
    }

    bool PreferencesWindow::onStartInTrayChanged(bool state) const
    {
        util::Settings::getInstance().setStartInTray(state);

        return false;
    }

    bool PreferencesWindow::onAutostartChanged(bool state) const
    {
        util::Settings::getInstance().setAutostart(state);

        return false;
    }

    bool PreferencesWindow::onAllowPermissionsChanged(bool state) const
    {
        util::Settings::getInstance().setAllowPermissions(state);

        return false;
    }

    void PreferencesWindow::onHwAccelChanged()
    {
        auto active = m_comboboxHwAccel->get_active_row_number();
        m_webView->setHwAccelPolicy(static_cast<WebKitHardwareAccelerationPolicy>(active));
        util::Settings::getInstance().setHwAccel(active);
    }
}
