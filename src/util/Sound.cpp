#include "Sound.hpp"
#include <iostream>
#include "Config.hpp"

namespace wfl::util
{
    Sound::Sound()
        : m_context{nullptr}
    {
        ca_context_create(&m_context);

        ca_context_change_props(m_context, CA_PROP_APPLICATION_ID, WFL_APP_ID, nullptr);
        ca_context_change_props(m_context, CA_PROP_APPLICATION_NAME, WFL_FRIENDLY_NAME, nullptr);
        ca_context_change_props(m_context, CA_PROP_APPLICATION_VERSION, WFL_VERSION, nullptr);
        ca_context_change_props(m_context, CA_PROP_APPLICATION_ICON_NAME, WFL_ICON, nullptr);
        ca_context_change_props(m_context, CA_PROP_EVENT_ID, 0U, nullptr);
        ca_context_change_props(m_context, CA_PROP_EVENT_DESCRIPTION, WFL_FRIENDLY_NAME " Notification", nullptr);
    }

    Sound::~Sound()
    {
        ca_context_destroy(m_context);
    }

    void Sound::play(std::string_view name)
    {
        auto ret = ca_context_play(m_context, 0U, CA_PROP_EVENT_ID, name.data(), nullptr);
        if (ret < 0)
        {
            std::cerr << "Sound: Failed to play " << name << ": " << ca_strerror(ret) << std::endl;
        }
    }
}
