#include "../include/Settings.h"
#include <filesystem>

Settings::Settings(): backgroundSprite(backgroundTexture) {
}

bool Settings::loadFont(const std::string& fontPath) {
    try {
        font = sf::Font(fontPath);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool Settings::loadBackground(const std::string& backgroundPath) {
    try {
        if (!backgroundTexture.loadFromFile(backgroundPath)) {
            return false;
        }

        backgroundSprite.setTexture(backgroundTexture);

        backgroundSprite.setScale(
            {
                static_cast<float>(1920) / backgroundTexture.getSize().x,
                static_cast<float>(1080) / backgroundTexture.getSize().y
            }
        );

        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

void Settings::updateBackgroundScale(const sf::Vector2u& windowSize) {
    backgroundSprite.setScale(
        {static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
        static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
    });
}

sf::Font& Settings::getFont() {
    return font;
}

sf::Texture& Settings::getBackgroundTexture() {
    return backgroundTexture;
}

sf::Sprite& Settings::getBackgroundSprite() {
    return backgroundSprite;
}