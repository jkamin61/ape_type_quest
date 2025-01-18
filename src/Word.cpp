#include "../include/Word.h"
#include <SFML/Graphics.hpp>

Word::Word(const std::string &text, const sf::Vector2f &position, const sf::Font &font, float speedFactor)
    : speed(0.01 + speedFactor, 0), wordText(font, text, 32), isMatched(false) {
    wordText.setPosition(position);
    wordText.setFillColor(sf::Color::White);
}

void Word::move() {
        wordText.move(speed);
}

bool Word::isOffScreen(float bound) const {
    return wordText.getPosition().x > bound;
}

const sf::Text &Word::getText() const {
    return wordText;
}

void Word::markAsMatched() {
    isMatched = true;
    wordText.setFillColor(sf::Color::Green);
    matchTimer.restart();
}

bool Word::isMatchExpired(float duration) const {
    return isMatched && matchTimer.getElapsedTime().asSeconds() > duration;
}

bool Word::matched() const {
    return isMatched;
}

void Word::resetPosition(const sf::Vector2f &newPosition) {
    wordText.setPosition(newPosition);
    wordText.setFillColor(sf::Color::White);
    isMatched = false;
}
