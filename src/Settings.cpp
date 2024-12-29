#include "../include/Settings.h"
#include <filesystem>

Settings::Settings() {}

bool Settings::loadFont(const std::string& fontPath) {
    if (!font.openFromFile(fontPath)) {
        return false;
    }
    return true;
}

sf::Font& Settings::getFont() {
    return font;
}
