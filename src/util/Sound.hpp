#pragma once

#include <canberra.h>
#include <string_view>

namespace wfl::util
{
    class Sound
    {
        public:
            Sound();
            ~Sound();

        public:
            void play(std::string_view name);

        private:
            ca_context* m_context;
    };
}
