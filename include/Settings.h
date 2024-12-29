#ifndef SETTINGS_H
#define SETTINGS_H

#include <SFML/Graphics.hpp>
#include <string>

class Settings {
public:
    Settings();
    bool loadFont(const std::string& fontPath);
    sf::Font& getFont();

private:
    sf::Font font;
};

#endif
