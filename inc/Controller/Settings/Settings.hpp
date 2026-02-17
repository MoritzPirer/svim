///
/// @file: Settings.hpp
/// @description: holds the individual customization settings and provides the defaults
///
/// @date: 2026-02-05
/// @author: Moritz Pirer
///

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <optional>
#include <unordered_map>

#include "BooleanSetting.hpp"

class Settings {
private:
    std::unordered_map<std::string, BooleanSetting> m_boolean_settings;

public:
    Settings();
    Settings(const Settings&) = default;
    ~Settings() = default;

    bool isEnabled(const std::string& boolean_setting_name) const;
    const std::string& getDisplayName(const std::string& boolean_setting_name) const;
    const std::vector<std::string>& getDescription(const std::string& boolean_setting_name) const;

    void updateSetting(const std::string& setting_name, const BooleanSetting& setting);

    int getTabWidth() const;
};

#endif //SETTINGS_HPP
