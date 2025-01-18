#include "../include/Game.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <ctime>
#include "fmt/core.h"
#include <algorithm>
#include <SFML/System/Clock.hpp>

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Monkey Typer", sf::Style::None),
      easyText(settings.getFont(), "Easy", 30),
      mediumText(settings.getFont(), "Medium", 30), hardText(settings.getFont(), "Hard", 30),
      backgroundSpeed(100.0f), backgroundX(0.0f), score(0), level(1), allWordsGuessed(false),
      gameStarted(false), cursorVisible(true), notTypedWords(0), wordsLoaded(false), speedFactor(0.35f) {
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
    setupStartMenuButtons();
    gameClock.restart();
}

void Game::setupStartMenuButtons() {
    float centerX = window.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f;

    startButton.setSize(sf::Vector2f(340, 60));
    startButton.setFillColor(sf::Color::Black);
    startButton.setPosition({centerX - 180, centerY - 200});

    chooseFontButton.setSize(sf::Vector2f(340, 60));
    chooseFontButton.setFillColor(sf::Color::Black);
    chooseFontButton.setPosition({centerX - 180, centerY - 100});

    chooseDifficultyButton.setSize(sf::Vector2f(340, 60));
    chooseDifficultyButton.setFillColor(sf::Color::Black);
    chooseDifficultyButton.setPosition({centerX - 180, centerY});

    uploadWordsButton.setSize(sf::Vector2f(340, 60));
    uploadWordsButton.setFillColor(sf::Color::Black);
    uploadWordsButton.setPosition({centerX - 180, centerY + 100});

    exitMenuButton.setSize(sf::Vector2f(340, 60));
    exitMenuButton.setFillColor(sf::Color::Black);
    exitMenuButton.setPosition({static_cast<float>(window.getSize().x) - 100.f, 10.f});

    gameStarted = false;
}

void Game::renderStartScreen() {
    window.clear();

    window.draw(startButton);
    window.draw(chooseFontButton);
    window.draw(chooseDifficultyButton);
    window.draw(uploadWordsButton);

    sf::Text startText(settings.getFont(), "Start", 50);
    startText.setFillColor(sf::Color::White);

    sf::Text chooseFontText(settings.getFont(), "Choose font", 50);
    chooseFontText.setFillColor(sf::Color::White);

    sf::Text chooseDifficultyText(settings.getFont(), "Choose difficulty", 50);
    chooseDifficultyText.setFillColor(sf::Color::White);

    sf::Text uploadWordsText(settings.getFont(), "Upload words", 50);
    uploadWordsText.setFillColor(sf::Color::White);

    sf::Text exitText(settings.getFont(), "Exit", 35);
    exitText.setFillColor(sf::Color::White);

    float centerX = window.getSize().x / 2.0f - startButton.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f - startButton.getSize().y / 2.0f;

    startText.setPosition({centerX, centerY - 180});
    chooseFontText.setPosition({centerX, centerY - 80});
    chooseDifficultyText.setPosition({centerX, centerY + 20});
    uploadWordsText.setPosition({centerX, centerY + 120});
    exitText.setPosition({static_cast<float>(window.getSize().x) - 100.f, 10.f});

    if (startText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        startText.setFillColor(sf::Color::Yellow);
    }

    if (chooseFontText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        chooseFontText.setFillColor(sf::Color::Yellow);
    }

    if (chooseDifficultyText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        chooseDifficultyText.setFillColor(sf::Color::Yellow);
    }

    if (uploadWordsText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        uploadWordsText.setFillColor(sf::Color::Yellow);
    }

    if (exitText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        exitText.setFillColor(sf::Color::Yellow);
    }

    window.draw(startText);
    window.draw(chooseFontText);
    window.draw(chooseDifficultyText);
    window.draw(uploadWordsText);
    window.draw(exitText);

    window.display();
}

void Game::renderFontSelectionScreen() {
    window.clear();

    sf::Text title(settings.getFont(), "Select a Font", 50);
    title.setFillColor(sf::Color::White);
    title.setPosition({window.getSize().x / 2.0f - title.getGlobalBounds().size.y / 2.0f, 20});
    window.draw(title);

    float yPosition = 100.0f;
    for (size_t i = 0; i < availableFonts.size(); ++i) {
        sf::Text fontText(settings.getFont(), availableFonts[i], 30);
        fontText.setFillColor(sf::Color::White);
        fontText.setPosition({50.0f, yPosition});

        if (fontText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            fontText.setFillColor(sf::Color::Yellow);
        }

        window.draw(fontText);
        yPosition += 50.0f;
    }

    window.display();
}

void Game::loadAvailableFonts(const std::string &directory) {
    availableFonts.clear();
    for (const auto &entry: std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".ttf") {
            availableFonts.push_back(entry.path().string());
        }
    }

    if (availableFonts.empty()) {
        throw std::runtime_error("No fonts found in the directory: " + directory);
    }
}

void Game::renderDifficultySelectionScreen() {
    window.clear();
    float centerX = window.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f;

    sf::Text title(settings.getFont(), "Select a Difficulty", 50);
    title.setFillColor(sf::Color::White);
    title.setPosition({centerX - title.getGlobalBounds().size.y / 2.0f, 20});
    window.draw(title);
    easyText.setPosition({centerX - 50, centerY - 100});
    easyText.setFillColor(sf::Color::White);
    if (easyText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        easyText.setFillColor(sf::Color::Yellow);
    }
    window.draw(easyText);
    mediumText.setPosition({centerX - 50, centerY});
    mediumText.setFillColor(sf::Color::White);;
    if (mediumText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        mediumText.setFillColor(sf::Color::Yellow);
    }
    window.draw(mediumText);
    hardText.setPosition({centerX - 50, centerY + 100});
    hardText.setFillColor(sf::Color::White);;
    if (hardText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        hardText.setFillColor(sf::Color::Yellow);
    }
    window.draw(hardText);

    window.display();
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

    sf::Text missedWordsText(settings.getFont(), fmt::format("Missed: {}", notTypedWords), 35);
    missedWordsText.setPosition({800.f, static_cast<float>(window.getSize().y) - 70.f});
    missedWordsText.setFillColor(sf::Color::White);
    window.draw(missedWordsText);

    sf::Time elapsed = gameClock.getElapsedTime();
    sf::Text timeText(settings.getFont(), fmt::format("Time: {}", elapsed.asSeconds()), 35);
    timeText.setPosition({1000.f, static_cast<float>(window.getSize().y) - 70.f});
    timeText.setFillColor(sf::Color::White);
    window.draw(timeText);

    sf::Text exitText(settings.getFont(), "Exit", 35);
    exitText.setPosition({static_cast<float>(window.getSize().x) - 100.f, 10.f});
    exitText.setFillColor(sf::Color::White);
    window.draw(exitText);

    exitButton.setSize(sf::Vector2f(100, 50));
    exitButton.setFillColor(sf::Color::Transparent);
    exitButton.setPosition({static_cast<float>(window.getSize().x) - 100.f, 10.f});
    window.draw(exitButton);

    if (exitText.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        exitText.setFillColor(sf::Color::Yellow);
    }

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
        level++;
        backgroundX = 0.0f;
        words.clear();
        loadNextWords();
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
            } else if (exitMenuButton.getGlobalBounds().contains(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                exitGame();
            } else if (!gameStarted && chooseFontButton.getGlobalBounds().contains(
                           window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                loadAvailableFonts("assets/fonts");
                while (window.isOpen()) {
                    renderFontSelectionScreen();

                    while (window.pollEvent()) {
                        if (event->is<sf::Event::Closed>()) {
                            window.close();
                        } else if (event->is<sf::Event::MouseButtonPressed>()) {
                            const auto &mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                            sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                            if (mouseEvent->button == sf::Mouse::Button::Left) {
                                float yPosition = 100.0f;

                                for (size_t i = 0; i < availableFonts.size(); ++i) {
                                    sf::Text fontText(settings.getFont(), availableFonts[i], 30);
                                    fontText.setPosition({50.0f, yPosition});

                                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && fontText.
                                        getGlobalBounds().contains(mousePosition)) {
                                        settings.loadFont(availableFonts[i]);
                                        return;
                                    }

                                    yPosition += 50.0f;
                                }
                            }
                        }
                    }
                }
            } else if (!gameStarted && chooseDifficultyButton.getGlobalBounds().contains(
                           window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                while (window.isOpen()) {
                    renderDifficultySelectionScreen();
                    while (window.pollEvent()) {
                        if (event->is<sf::Event::Closed>()) {
                            window.close();
                        } else if (event->is<sf::Event::MouseButtonPressed>()) {
                            const auto &mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                            sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                            if (mouseEvent->button == sf::Mouse::Button::Left) {
                                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && easyText.getGlobalBounds().
                                    contains(mousePosition)) {
                                    speedFactor = 0.25f;
                                    return;
                                } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mediumText.
                                           getGlobalBounds().contains(mousePosition)) {
                                    speedFactor = 0.35f;
                                    return;
                                } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && hardText.
                                           getGlobalBounds().contains(mousePosition)) {
                                    speedFactor = 0.45f;
                                    return;
                                }
                            }
                        }
                    }
                }
            } else if (!gameStarted && uploadWordsButton.getGlobalBounds().contains(
                           window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                fmt::print("Upload words button clicked\n");
            }
        }
    }
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
        } else if (allWordsGuessed) {
            renderEndScreen();
        } else {
            renderStartScreen();
        }
    }
}


void Game::renderEndScreen() {
    window.clear();

    sf::Text endText(settings.getFont(), "You guessed all words! Congratulations!", 50);
    endText.setFillColor(sf::Color::White);
    endText.setPosition({window.getSize().x / 2.0f - 300.f, window.getSize().y / 2.0f});
    window.draw(endText);

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
    if (words.empty() && wordList.empty()) {
        allWordsGuessed = true;
        fmt::print("Congratulations! All words guessed.\n");
        window.close();
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
    }
    typedText.clear();

    checkAllWordsGuessed();
}

void Game::loadNextWords() {
    if (wordList.empty()) {
        fmt::print("Word list is empty. No more words available.\n");
        return;
    }

    float yPosition = rand() % 200;
    for (int i = 0; i < 4 + rand() % 6; ++i) {
        if (wordList.empty()) {
            fmt::print("Word list became empty during word generation.\n");
            break;
        }
        int randomIndex = rand() % wordList.size();
        std::string randomWord = wordList[randomIndex];
        words.emplace_back(randomWord, sf::Vector2f(0, yPosition), settings.getFont(), level * speedFactor);
        yPosition += 40 + rand() % 60;
    }

    wordsLoaded = false;
}

void Game::exitGame() {
    window.close();
}

void Game::goBackToMenu() {
    gameStarted = false;
    setupStartMenuButtons();
}

void Game::countMissedWords() {
    notTypedWords++;
}
