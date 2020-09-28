#pragma once

#include <glibmm/keyfile.h>


class Settings
{
    public:
        static Settings& instance();

    public:
        Settings(Settings const&) = delete;
        Settings& operator=(Settings const&) = delete;

    public:
        void setAllowPermissions(bool allow);
        bool allowPermissions() const;
        void setCloseToTray(bool closetotray);
        bool closeToTray() const;

    private:
        Settings();
        ~Settings();

    private:
        Glib::KeyFile m_keyFile;
};
