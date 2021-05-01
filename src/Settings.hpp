#pragma once

#include <glibmm/keyfile.h>

class Settings
{
    public:
        static Settings& getInstance();

    public:
        Settings(Settings const&) = delete;
        Settings& operator=(Settings const&) = delete;

    public:
        void   setCloseToTray(bool enable);
        bool   getCloseToTray() const;
        void   setAllowPermissions(bool allow);
        bool   getAllowPermissions() const;
        void   setZoomLevel(double zoomLevel);
        double getZoomLevel() const;
        void   setHeaderBar(bool enable);
        bool   getHeaderBar() const;
        void   setStartInTray(bool enable);
        bool   getStartInTray() const;

    private:
        Settings();
        ~Settings();

    private:
        Glib::KeyFile m_keyFile;
};
