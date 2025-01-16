#include "../include/Game.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <ctime>
#include "fmt/core.h"
#include <algorithm>
#include <SFML/System/Clock.hpp>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Monkey Typer", sf::Style::None),
      score(0), level(1), roundCounter(0), allWordsGuessed(false), gameStarted(false), backgroundSpeed(100.0f),
      backgroundX(0.0f), cursorVisible(true), notTypedWords(0), wordsLoaded(false) {
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

    float textureWidth = static_cast<float>(backgroundTexture.getSize().x);
    float firstTexturePosition = -backgroundX;
    float secondTexturePosition = firstTexturePosition + textureWidth;

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition({firstTexturePosition, 0});
    window.draw(backgroundSprite);

    if (secondTexturePosition < window.getSize().x) {
        backgroundSprite.setPosition({secondTexturePosition, 0});
        window.draw(backgroundSprite);
    }

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

    sf::Text missedWordsText(settings.getFont(), fmt::format("Missed: {}", notTypedWords), 35);
    missedWordsText.setPosition({1000.f, static_cast<float>(window.getSize().y) - 70.f});
    missedWordsText.setFillColor(sf::Color::White);
    window.draw(missedWordsText);

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
    if (backgroundX >= (backgroundTexture.getSize().x)) {
        backgroundX = 0.0f;
    }

    std::erase_if(words, [this](Word &word) {
        if (word.isOffScreen(window.getSize().x)) {
            countMissedWords();
            return true;
        } else if (word.isMatchExpired(0.5f)) {
            return true;
        }
        return false;
    });

    if (score >= level * 180) {
        roundCounter++;
        level++;
        backgroundX = 0.0f;
    }

    for (auto &word: words) {
        word.move();
    }
    if (!words.empty() && words.back().getText().getPosition().x > 150) {
        if (!wordsLoaded) {
            loadNextWords();
            wordsLoaded = true;
        }
    } else {
        wordsLoaded = false;
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
        score += 10;
        sf::Text greenText = it->getText();
        it->markAsMatched();
    } else {
        fmt::print("Incorrect word typed: {}\n", typedText);
    }
    // checkAllWordsGuessed();
    typedText.clear();
}

void Game::loadNextWords() {
    if (wordList.empty()) {
        fmt::print("Word list is empty. Reloading words...\n");
        loadWordsFromFile("assets/words.txt");
        if (wordList.empty()) {
            fmt::print("No words available to load. Exiting...\n");
            return;
        }
    }

    float yPosition = rand() % 200;
    for (int i = 0; i < 11; ++i) {
        if (wordList.empty()) {
            fmt::print("Word list became empty during word generation.\n");
            break;
        }
        int randomIndex = rand() % wordList.size();
        std::string randomWord = wordList[randomIndex];
        words.emplace_back(randomWord, sf::Vector2f(0, yPosition), settings.getFont(), level * 0.95f);
        yPosition += 40 + rand() % 60;
    }

    wordsLoaded = false;
}

void Game::exitGame() {
    window.close();
}

void Game::countMissedWords() {
    notTypedWords++;
}
