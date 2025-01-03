#include "../include/Word.h"
#include <SFML/Graphics.hpp>

Word::Word(const std::string& text, const sf::Vector2f& position, const sf::Font& font, float speedFactor)
    : speed( 0.01 + speedFactor, 0), wordText(font, text, 24) {
    wordText.setPosition(position);
}

void Word::move() {
    wordText.move(speed);
}

bool Word::isOffScreen(float bound) const {
    return wordText.getPosition().x > bound;
}

const sf::Text& Word::getText() const {
    return wordText;
}
