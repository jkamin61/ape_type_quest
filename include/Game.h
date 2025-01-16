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
    void setupStartButton();
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

    sf::RenderWindow window;
    sf::RectangleShape startButton;
    sf::RectangleShape exitButton;
    Settings settings;
    std::vector<std::string> wordList;
    std::vector<Word> words;
    sf::Texture backgroundTexture;
    sf::Clock cursorClock;
    sf::Clock gameClock;
    std::string typedText;

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
