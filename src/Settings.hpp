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
        void   setCloseToTray(bool enable);
        bool   closeToTray() const;
        void   setAllowPermissions(bool allow);
        bool   allowPermissions() const;
        void   setZoomLevel(double zoomLevel);
        double zoomLevel() const;
        void   setHeaderBar(bool enable);
        bool   headerBar() const;
        void   setStartAtTray(bool enable);
        bool   startAtTray() const;

    private:
        Settings();
        ~Settings();

    private:
        Glib::KeyFile m_keyFile;
};
