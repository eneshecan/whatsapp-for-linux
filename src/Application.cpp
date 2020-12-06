#include "Application.hpp"

Application* Application::m_instance = nullptr;

Application& Application::instance()
{
    return *m_instance;
}

Application::Application(int argc, char** argv, Glib::ustring const& id)
    : Gtk::Application{argc, argv, id}
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
