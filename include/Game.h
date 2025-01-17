#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Settings.h"
#include "Word.h"

class Game {
public:
    Game();
    void run();

private:
    void setupStartMenuButtons();
    void startGame();
    void processEvents();
    void update();
    void render();
    void renderStartScreen();
    void loadWordsFromFile(const std::string& filename);
    void checkAllWordsGuessed();
    void checkWord();
    void loadNextWords();
    void countMissedWords();
    void exitGame();
    void renderEndScreen();
    void loadAvailableFonts(const std::string& directory);
    void renderFontSelectionScreen();

    sf::RenderWindow window;
    sf::RectangleShape startButton;
    sf::RectangleShape exitButton;
    sf::RectangleShape exitMenuButton;
    sf::RectangleShape chooseFontButton;
    sf::RectangleShape chooseDifficultyButton;
    sf::RectangleShape uploadWordsButton;
    Settings settings;
    std::vector<std::string> wordList;
    std::vector<Word> words;
    sf::Texture backgroundTexture;
    sf::Clock cursorClock;
    sf::Clock gameClock;
    std::string typedText;
    std::vector<std::string> availableFonts;

    float backgroundSpeed;
    float backgroundX;
    int score;
    int level;
    bool allWordsGuessed;
    bool gameStarted;
    bool cursorVisible;
    int notTypedWords;
    bool wordsLoaded;
};

#endif // GAME_H
