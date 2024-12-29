#ifndef WORD_H
#define WORD_H

#include <SFML/Graphics.hpp>
#include <string>

class Word {
public:
    Word(const std::string& text, const sf::Vector2f& position, const sf::Font& font, float speedFactor = 0.0f);
    void move();
    bool isOffScreen() const;
    const sf::Text& getText() const;

private:
    sf::Text wordText;
    sf::Vector2f speed;
};

#endif
