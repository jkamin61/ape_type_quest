#ifndef WORD_H
#define WORD_H

#include <SFML/Graphics.hpp>
#include <string>

class Word {
public:
    Word(const std::string& text, const sf::Vector2f& position, const sf::Font& font, float speedFactor = 0.0f);
    void move();
    bool isOffScreen(float bound) const;
    const sf::Text& getText() const;
    void resetPosition(const sf::Vector2f& newPosition);
    void markAsMatched();
    bool isMatchExpired(float duration) const;
    bool matched() const;

private:
    sf::Text wordText;
    sf::Vector2f speed;
    bool isMatched;
    sf::Clock matchTimer;
};

#endif
