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
    void displayWords();
    void checkAllWordsGuessed();
    void checkWord();
    void loadNextWords();
    void gameOver();

    sf::RenderWindow window;
    sf::RectangleShape startButton;
    Settings settings;
    std::vector<std::string> wordList;
    std::vector<Word> words;
    std::string typedText;
    int score;
    int level;
    int roundCounter;
    bool allWordsGuessed;
    bool gameStarted;
};

#endif // GAME_H
