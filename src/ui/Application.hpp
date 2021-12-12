#pragma once

#include <gtkmm/application.h>

namespace wfl::ui
{
    class Application
        : public Gtk::Application
    {
        public:
            static Application& getInstance();

        public:
            Application(int argc, char** argv, Glib::ustring const& id, Gio::ApplicationFlags flags = Gio::APPLICATION_FLAGS_NONE);

        public:
            void keepAlive();
            void endKeepAlive();

        private:
            static Application* m_instance;

        private:
            bool m_onHold;
    };
}
