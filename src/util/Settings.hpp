#pragma once

#include "SettingMap.hpp"

namespace wfl::util
{
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
            void   setAutostart(bool autostart);
            bool   getAutostart();

        private:
            Settings();
            ~Settings();

        private:
            SettingMap m_settingMap;
    };
}
