#ifndef SETTINGS_H
#define SETTINGS_H

#include <SFML/Graphics.hpp>
#include <string>

class Settings {
public:
    Settings();
    bool loadFont(const std::string& fontPath);
    bool loadBackground(const std::string& backgroundPath);
    sf::Font& getFont();
    sf::Texture& getBackgroundTexture();
    sf::Sprite& getBackgroundSprite();
    void updateBackgroundScale(const sf::Vector2u& windowSize);
private:
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};

#endif
