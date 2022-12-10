#include "Application.hpp"
#include <gtkmm/settings.h>
#include "../util/Settings.hpp"

namespace wfl::ui
{
    Application* Application::m_instance = nullptr;

    Application& Application::getInstance()
    {
        return *m_instance;
    }

    Application::Application(int argc, char** argv, Glib::ustring const& id, Gio::ApplicationFlags flags)
        : Gtk::Application{argc, argv, id, flags}
        , m_onHold{false}
    {
        if (!m_instance)
        {
            m_instance = this;
        }
        else
        {
            throw std::runtime_error("Attempt to create more than one instance of the Application");
        }
    }

    void Application::keepAlive()
    {
        if (!m_onHold)
        {
            Gtk::Application::hold();
            m_onHold = true;
        }
    }

    void Application::endKeepAlive()
    {
        if (m_onHold)
        {
            Gtk::Application::release();
            m_onHold = false;
        }
    }

    void Application::on_activate()
    {
        auto const settings = Gtk::Settings::get_default();
        auto const preferDark = util::Settings::getInstance().getValue<bool>("appearance", "prefer-dark-theme");
        settings->property_gtk_application_prefer_dark_theme().set_value(preferDark);

        Gtk::Application::on_activate();
    }
}
