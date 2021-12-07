#pragma once

#include <iostream>
#include <type_traits>
#include <glibmm/keyfile.h>

namespace wfl::util
{
    class SettingMap
    {
        public:
            SettingMap()  = default;
            ~SettingMap() = default;

        public:
            bool loadFromFile(std::string const& fileName);
            bool saveToFile(std::string const& fileName);

            template<typename ValueType>
            void setValue(Glib::ustring const& groupName, Glib::ustring const& key, ValueType const& value);

            template<typename ValueType>
            ValueType getValue(Glib::ustring const& groupName, Glib::ustring const& key, ValueType const& defaultValue = {}) const noexcept;

        private:
            Glib::KeyFile m_map;
    };




    inline bool SettingMap::loadFromFile(std::string const& fileName)
    {
        try
        {
            return m_map.load_from_file(fileName);
        }
        catch (Glib::Error const& error)
        {
            std::cerr << "SettingMap: Failed to load setting map from file: " << error.what() << std::endl;
            return false;
        }
    }

    inline bool SettingMap::saveToFile(std::string const& fileName)
    {
        try
        {
            return m_map.save_to_file(fileName);
        }
        catch (Glib::Error const& error)
        {
            std::cerr << "SettingMap: Failed to save setting map to file: " << error.what() << std::endl;
            return false;
        }
    }

    template<typename ValueType>
    inline void SettingMap::setValue(Glib::ustring const& groupName, Glib::ustring const& key, ValueType const& value)
    {
        if constexpr (std::is_same<ValueType, bool>::value)
        {
            m_map.set_boolean(groupName, key, value);
        }
        else if constexpr (std::is_same<ValueType, double>::value)
        {
            m_map.set_double(groupName, key, value);
        }
        else if constexpr (std::is_same<ValueType, Glib::ustring>::value)
        {
            m_map.set_string(groupName, key, value);
        }
        else
        {
            m_map.set_value(groupName, key, value);
        }
    }

    template<typename ValueType>
    inline ValueType SettingMap::getValue(Glib::ustring const& groupName, Glib::ustring const& key, ValueType const& defaultValue) const noexcept
    {
        try
        {
            if constexpr (std::is_same<ValueType, bool>::value)
            {
                return m_map.get_boolean(groupName, key);
            }
            else if constexpr (std::is_same<ValueType, double>::value)
            {
                return m_map.get_double(groupName, key);
            }
            else if constexpr (std::is_same<ValueType, Glib::ustring>::value)
            {
                return m_map.get_string(groupName, key);
            }
            else
            {
                return m_map.get_value(groupName, key);
            }
        }
        catch (Glib::KeyFileError const& error)
        {
            std::cerr << "SettingMap: " <<  error.what() << ", returning default value." << std::endl;
            return defaultValue;
        }
    }
}
