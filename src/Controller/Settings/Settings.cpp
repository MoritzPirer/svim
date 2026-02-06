#include "../../../inc/Controller/Settings/Settings.hpp"

using std::vector, std::string;

Settings::Settings() {
    m_boolean_settings = {
        {"show_character_count", {
            true,
            "Show character count",
            {"Displays the number of characters the file has in the metadata panel."}
        }},
        {"show_word_count", {
            true,
            "Show word count",
            {"Displays the number of words the file has in the metadata panel."}
        }},
        {"show_paragraph_count", {
            true,
            "Show paragraph count",
            {"Displays the number of paragraphs the file has in the metadata panel."}
        }},
        {"show_cursor_position", {
            true,
            "Show cursor position",
            {"Displays the the current position of the cursor in the metadata panel."}
        }},
        {"show_editor_mode", {
            true,
            "Show current mode",
            {"Display the current editor mode in the metadata panel."}
        }},
        {"show_seperator", {
            true,
            "Show metadata panel seperator",
            {"Display a line visually seperating the text area from the metadata panel."}
        }},
        {"show_file_name", {
            true,
            "Show file name",
            {"Display the name of the current file in the metadata panel."}
        }},
        {"show_save_icon", {
            true,
            "Show save state icon",
            {
                "Display the current save state of the file as an icon in the metadata panel.",
                "- [!] means that the file has never been saved",
                "- [*] means that there are unsaved changes",
                "- [=] means that all changes are saved"
            }
        }},
        {"show_save_text", {
            true,
            "Show save state text",
            {"Display the current save state of the file as a text in the metadata panel."}
        }},
        {"show_version", {
            true,
            "Show editor version",
            {"Display the current version of the editor in the metadata panel."}
        }},
        {"render_color", {
            true,
            "Render color",
            {"Render the user interface in color."}
        }},
        {"confirm_save", {
            true,
            "Confirm save",
            {"Display a confirmation message when changes were saved successfully."}
        }},
        {"do_numbering", {
            true,
            "Display paragraph numbers",
            {"Display the numbers of the paragraphs along the left edge of the text area."}
        }},
        {"do_relative_numbers", {
            false,
            "Relative Numbers",
            {
                "Use the distance to the cursor as the paragraph number for all paragraphs except the one containing the cursor.",
                "Only has an effect if 'Display paragraph numbers' is enabled."
            }
        }}
    };
}

bool Settings::isEnabled(const string& boolean_setting_name) const {
    if (m_boolean_settings.contains(boolean_setting_name)) {
        return m_boolean_settings.at(boolean_setting_name).value;
    }

    throw std::invalid_argument("Unknown setting '" + boolean_setting_name + "' requested!");
} 
    
const string& Settings::getDisplayName(const string& boolean_setting_name) const {
    if (m_boolean_settings.contains(boolean_setting_name)) {
        return m_boolean_settings.at(boolean_setting_name).display_name;
    }

    throw std::invalid_argument("Unknown setting '" + boolean_setting_name + "' requested!");
}
    
const vector<string>& Settings::getDescription(const string& boolean_setting_name) const {
    if (m_boolean_settings.contains(boolean_setting_name)) {
        return m_boolean_settings.at(boolean_setting_name).description;
    }

    throw std::invalid_argument("Unknown setting '" + boolean_setting_name + "' requested!");
}

void Settings::updateSetting(const string& setting_name, const BooleanSetting& setting) {
    if (m_boolean_settings.contains(setting_name)) {
        m_boolean_settings.at(setting_name) = setting;
        return;
    }

    throw std::invalid_argument("Unknown setting '" + setting_name + "' updated!");
}