#include "../include/Game.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <ctime>
#include "fmt/core.h"
#include <algorithm>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Monkey Typer", sf::Style::Default),
      score(0), level(1), roundCounter(0), allWordsGuessed(false), gameStarted(false) {
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(0)));

    if (!settings.loadFont("assets/fonts/breatheiii.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    loadWordsFromFile("assets/words.txt");
    checkWord();
    setupStartButton();
}

void Game::render() {
    window.clear();

    for (const auto &word: words) {
        window.draw(word.getText());
    }

    sf::RectangleShape inputBox(sf::Vector2f(780.f, 40.f));
    inputBox.setPosition({10.f, static_cast<float>(window.getSize().y)});
    inputBox.setFillColor(sf::Color(0, 0, 0, 100));
    inputBox.setOutlineColor(sf::Color::White);
    inputBox.setOutlineThickness(2.f);
    window.draw(inputBox);

    sf::Text typed(settings.getFont(), typedText, 20);
    typed.setPosition({15.f, static_cast<float>(window.getSize().y) - 45.f});
    typed.setFillColor(sf::Color::White);
    window.draw(typed);

    sf::Text scoreText(settings.getFont(), fmt::format("Score: {}", score), 20);
    scoreText.setPosition({200.f, static_cast<float>(window.getSize().y) - 35.f});
    scoreText.setFillColor(sf::Color::White);
    window.draw(scoreText);

    sf::Text levelText(settings.getFont(), fmt::format("Level: {}", level), 20);
    levelText.setPosition({400.f, static_cast<float>(window.getSize().y) - 35.f});
    levelText.setFillColor(sf::Color::White);
    window.draw(levelText);

    sf::Text roundText(settings.getFont(), fmt::format("Round: {}", roundCounter), 20);
    roundText.setPosition({600.f, static_cast<float>(window.getSize().y) - 35.f});
    roundText.setFillColor(sf::Color::White);
    window.draw(roundText);

    window.display();
}

void Game::update() {

    std::erase_if(words,
                  [this](const Word &word) {
                      if (word.isOffScreen()) {
                          gameOver();
                          return true;
                      }
                      return false;
                  });

    if (score >= level * 100) {
        level++;
    }

    loadNextWords();

    for (auto &word: words) {
        word.move();
    }
}

void Game::setupStartButton() {
    startButton.setSize(sf::Vector2f(200, 50));
    startButton.setFillColor(sf::Color::Black);

    float centerX = window.getSize().x / 2.0f - startButton.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f - startButton.getSize().y / 2.0f;
    startButton.setPosition({centerX, centerY});

    gameStarted = false;
}

void Game::startGame() {
    gameStarted = true;
    loadNextWords();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        if (gameStarted) {
            update();
            render();
        } else {
            renderStartScreen();
        }
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto *textEvent = event->getIf<sf::Event::TextEntered>()) {
            if (textEvent->unicode == '\b' && !typedText.empty()) {
                typedText.pop_back();
            } else if (textEvent->unicode == '\r' || textEvent->unicode == '\n') {
                checkWord();
            } else if (textEvent->unicode < 128) {
                typedText += static_cast<char>(textEvent->unicode);
            }
        } else if (event->is<sf::Event::MouseButtonPressed>()) {
            if (!gameStarted && startButton.getGlobalBounds().contains(
                    window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                startGame();
            }
        }
    }
}

void Game::renderStartScreen() {
    window.clear();

    window.draw(startButton);

    sf::Text startText(settings.getFont(), "Start", 30);
    startText.setFillColor(sf::Color::Green);
    float centerX = window.getSize().x / 2.0f - startButton.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f - startButton.getSize().y / 2.0f;
    startText.setPosition({centerX, centerY});
    window.draw(startText);

    window.display();
}

void Game::loadWordsFromFile(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Could not open words file");
    }

    while (std::getline(file, line)) {
        if (!line.empty()) {
            wordList.push_back(line);
        }
    }
}

void Game::displayWords() {
    float speedFactor = 0.2f + 0.001f * (std::sqrt(level / 1000));

    float yPosition = 50;
    for (int i = 0; i < 5; ++i) {
        std::string randomWord = wordList[rand() % wordList.size()];
        words.emplace_back(randomWord, sf::Vector2f(0, yPosition), settings.getFont(), speedFactor);
        yPosition += 50;
    }
}

void Game::checkAllWordsGuessed() {
    if (words.empty()) {
        allWordsGuessed = true;
    }
}

void Game::checkWord() {
    auto it = std::find_if(words.begin(), words.end(), [this](const Word &word) {
        return word.getText().getString() == typedText;
    });

    if (it != words.end()) {
        score += 10;
        words.erase(it);
    }

    checkAllWordsGuessed();

    typedText.clear();
}

void Game::loadNextWords() {
    if (allWordsGuessed) {
        allWordsGuessed = false;

        float yPosition = 50;
        for (int i = 0; i < 10; ++i) {
            std::string randomWord = wordList[rand() % wordList.size()];
            words.emplace_back(randomWord, sf::Vector2f(0, yPosition), settings.getFont(), level * 0.25f);
            yPosition += 50;
        }
    }
}

void Game::gameOver() {
    window.close();
    fmt::print("Game Over! Final Score: {}\n", score);
}
