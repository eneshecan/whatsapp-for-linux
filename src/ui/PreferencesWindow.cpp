#include "PreferencesWindow.hpp"
#include "../util/Settings.hpp"

namespace wfl::ui
{
    PreferencesWindow::PreferencesWindow(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& refBuilder, TrayIcon& trayIcon, WebView& webView)
        : Gtk::Window{cobject}
        , m_trayIcon{&trayIcon}
        , m_webView{&webView}
        , m_switchCloseToTray{nullptr}
        , m_switchStartInTray{nullptr}
        , m_switchAutostart{nullptr}
        , m_comboboxHwAccel{nullptr}
        , m_switchAllowPermissions{nullptr}
    {
        refBuilder->get_widget("switch_close_to_tray", m_switchCloseToTray);
        m_switchCloseToTray->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onCloseToTray), false);

        refBuilder->get_widget("switch_start_in_tray", m_switchStartInTray);
        m_switchStartInTray->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onStartInTray), false);

        refBuilder->get_widget("switch_autostart", m_switchAutostart);
        m_switchAutostart->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onAutostart), false);

        refBuilder->get_widget("combobox_hw_accel", m_comboboxHwAccel);
        m_comboboxHwAccel->signal_changed().connect(sigc::mem_fun(*this, &PreferencesWindow::onHardwareAcceleration));
        m_comboboxHwAccel->append("On Demand");
        m_comboboxHwAccel->append("Always");
        m_comboboxHwAccel->append("Never");

        refBuilder->get_widget("switch_allow_permissions", m_switchAllowPermissions);
        m_switchAllowPermissions->signal_state_set().connect(sigc::mem_fun(*this, &PreferencesWindow::onAllowPermissions), false);

        m_switchCloseToTray->set_state(m_trayIcon->isVisible());
        m_switchStartInTray->set_state(util::Settings::getInstance().getStartInTray() && m_trayIcon->isVisible());
        m_switchStartInTray->set_sensitive(m_trayIcon->isVisible());
        m_switchAutostart->set_state(util::Settings::getInstance().getAutostart());
        m_comboboxHwAccel->set_active(util::Settings::getInstance().getHwAccel());
        m_switchAllowPermissions->set_state(util::Settings::getInstance().getAllowPermissions());
    }

    bool PreferencesWindow::onCloseToTray(bool state)
    {
        std::cerr << "State " << state << std::endl;
        m_trayIcon->setVisible(state);
        util::Settings::getInstance().setCloseToTray(state);

        if (!state)
        {
            m_switchStartInTray->set_state(false);
        }
        m_switchStartInTray->set_sensitive(state);

        return false;
    }

    bool PreferencesWindow::onStartInTray(bool state)
    {
        util::Settings::getInstance().setStartInTray(state);

        return false;
    }

    bool PreferencesWindow::onAutostart(bool state)
    {
        util::Settings::getInstance().setAutostart(state);

        return false;
    }

    bool PreferencesWindow::onAllowPermissions(bool state)
    {
        util::Settings::getInstance().setAllowPermissions(state);

        return false;
    }

    void PreferencesWindow::onHardwareAcceleration()
    {
        auto active = m_comboboxHwAccel->get_active_row_number();
        m_webView->setHwAccelPolicy(static_cast<WebKitHardwareAccelerationPolicy>(active));
        util::Settings::getInstance().setHwAccel(active);
    }
}
