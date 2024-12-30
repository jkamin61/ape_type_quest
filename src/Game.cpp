#include "../include/Game.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <ctime>
#include "fmt/core.h"
#include <algorithm>
#include <SFML/System/Clock.hpp>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Monkey Typer", sf::Style::None),
      score(0), level(1), roundCounter(0), allWordsGuessed(false), gameStarted(false), backgroundSpeed(100.0f), backgroundX(0.0f), cursorVisible(true) {
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(0)));

    if (!settings.loadFont("assets/fonts/breatheiii.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    if (!backgroundTexture.loadFromFile("assets/background.jpg")) {
        throw std::runtime_error("Failed to load background image!");
    }

    loadWordsFromFile("assets/words.txt");
    checkWord();
    setupStartButton();
}

void Game::render() {
    window.clear();

    sf::Sprite backgroundSprite(backgroundTexture);

    backgroundSprite.setPosition({-backgroundX, 0});
    window.draw(backgroundSprite);

    backgroundSprite.setPosition({backgroundTexture.getSize().x + backgroundX, 0});
    window.draw(backgroundSprite);

    window.draw(backgroundSprite);

    for (const auto &word: words) {
        window.draw(word.getText());
    }

    sf::Text typed(settings.getFont(), typedText, 35);
    typed.setPosition({15.f, static_cast<float>(window.getSize().y) - 70.f});
    typed.setFillColor(sf::Color::White);
    window.draw(typed);

    if (cursorVisible) {
        sf::Text cursor(settings.getFont(), "|", 35);
        cursor.setPosition({5.f, static_cast<float>(window.getSize().y) - 70.f});
        cursor.setFillColor(sf::Color::White);
        window.draw(cursor);
    }

    sf::Text scoreText(settings.getFont(), fmt::format("Score: {}", score), 35);
    scoreText.setPosition({400.f, static_cast<float>(window.getSize().y) - 70.f});
    scoreText.setFillColor(sf::Color::White);
    window.draw(scoreText);

    sf::Text levelText(settings.getFont(), fmt::format("Level: {}", level), 35);
    levelText.setPosition({600.f, static_cast<float>(window.getSize().y) - 70.f});
    levelText.setFillColor(sf::Color::White);
    window.draw(levelText);

    sf::Text roundText(settings.getFont(), fmt::format("Round: {}", roundCounter), 35);
    roundText.setPosition({800.f, static_cast<float>(window.getSize().y) - 70.f});
    roundText.setFillColor(sf::Color::White);
    window.draw(roundText);

    sf::Text exitText(settings.getFont(), "Exit", 35);
    exitText.setPosition({static_cast<float>(window.getSize().x) - 100.f, 10.f});
    exitText.setFillColor(sf::Color::White);
    window.draw(exitText);

    exitButton.setSize(sf::Vector2f(100, 50));
    exitButton.setFillColor(sf::Color::Transparent);
    exitButton.setPosition({static_cast<float>(window.getSize().x) - 100.f, 10.f});
    window.draw(exitButton);

    window.display();
}

void Game::update() {

    float deltaTime = 1.0f / 60.0f;

    backgroundX += backgroundSpeed * deltaTime;
    if (backgroundX >= (backgroundTexture.getSize().x / 2)) {
        backgroundX = 0.0f;
    }

    std::erase_if(words,
                  [this](const Word &word) {
                      if (word.isOffScreen(window.getSize().x)) {
                          gameOver();
                          return true;
                      }
                      return false;
                  });

    if (score >= level * 200) {
        roundCounter++;
        level++;
        backgroundX = 0.0f;
    }

    loadNextWords();

    for (auto &word: words) {
        word.move();
    }

    if (cursorClock.getElapsedTime().asSeconds() >= 0.5f) {
        cursorVisible = !cursorVisible;
        cursorClock.restart();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto *textEvent = event->getIf<sf::Event::TextEntered>()) {
            if (textEvent->unicode == '\b') {
                if (!typedText.empty()) {
                    typedText.pop_back();
                }
            } else if (textEvent->unicode == '\r' || textEvent->unicode == '\n') {
                checkWord();
            } else if (textEvent->unicode < 128) {
                typedText += static_cast<char>(textEvent->unicode);
            }
        } else if (event->is<sf::Event::MouseButtonPressed>()) {
            if (!gameStarted && startButton.getGlobalBounds().contains(
                    window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                startGame();
                    } else if (exitButton.getGlobalBounds().contains(
                    window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                        exitGame();
                    }
        }
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

void Game::renderStartScreen() {
    window.clear();

    window.draw(startButton);

    sf::Text startText(settings.getFont(), "Start", 50);
    startText.setFillColor(sf::Color::White);
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
        displayScoringPing();
        score += 10;
        words.erase(it);
    }

    checkAllWordsGuessed();

    typedText.clear();
}

void Game:: displayScoringPing() {

}

void Game::loadNextWords() {
    if (allWordsGuessed) {
        allWordsGuessed = false;
        float yPosition = 50;
        for (int i = 0; i < 18; ++i) {
            std::string randomWord = wordList[rand() % wordList.size()];
            words.emplace_back(randomWord, sf::Vector2f(static_cast<float>(rand() % 100), yPosition), settings.getFont(), level * 0.25f);
            yPosition += 50;
        }
    }
}

void Game::exitGame() {
    window.close();
}

void Game::gameOver() {
    window.close();
    fmt::print("Game Over! Final Score: {}\n", score);
}
