#define _CRT_SECURE_NO_WARNINGS              // for MS Visual Studio

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;

#include "GameBoard.h"
#include "Constants.h"
#include "Words.h"
#include "Key.h"
#include "Statistics.h"
#include "Types.h"

char getKey(const auto* keyPressed);

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    unsigned targetRow = 0;
    unsigned targetCol = 0;
    unsigned hintCount;
    string message;
    string guess;
    char ltr;
    float deltaX;
    float deltaY;
    //sf::Event event;
    sf::Vector2i mousePos;
    bool statsDisplayReturn;
    GameStatus gameStatus = Started;

    Statistics stats;
    GameBoard game;
    PlayerStatus playerStatus;

    // Music
    sf::Music music;
    if (!music.openFromFile(Fifth)) cerr << "Unable of open music file " << Fifth << endl;
    music.setVolume(15);

    // If this is the first game played, display "How to play"
    if (stats.getPlayed() == 0)
    {
        game.howtoplay();
        // This is a hack
        game.draw("");
        game.display();
    }

    // If less than one day since last game, set player status to Practice
    if (stats.getNumberOfDaysSinceLastGame() == 0)
    {
        playerStatus = Practice;
        gameStatus = GameOver;
        hintCount = 0;
        game.displayLastActiveGame(stats.getTileContents());
        // this is a hack to reduce flickering
        for (auto i = 0; i < 10; i++)
        {
            game.draw("");
            game.display();
        }

        game.get_background().setFillColor(sf::Color::White);
        stats.display(game.get_window(), gameStatus,playerStatus, game);
        message = "Press F1 to play a practice game";
    }
    else  // This is a new "actual" game
    {
        music.play();
        playerStatus = Active;
        stats.clearTileContents();
        stats.getNumberOfGuesses() = 0;
    }

    ////////////////////////// Game Loop /////////////////////////
    while (game.get_window().isOpen())
    {
        ltr = 0;   // "Clear" the letter entry

        // Event Loop to monitor mouse and keyboard
        while (const std::optional event = game.get_window().pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                game.close();
                break;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if (music.getStatus() == sf::SoundSource::Status::Playing) music.stop();
                mousePos = sf::Mouse::getPosition(game.get_window());

                // check for How To Play Button
                deltaX = fabs(mousePos.x-game.gethowtoplayButtonPosition().x);
                deltaY = fabs(mousePos.y-game.gethowtoplayButtonPosition().y);
                if (deltaX < 13.0f and deltaY < 13.0f)
                {
                    game.howtoplay();
                    break;
                }

                // check for Stats Button
                deltaX = fabs(mousePos.x-game.getStatsButtonPosition().x);
                deltaY = fabs(mousePos.y-game.getStatsButtonPosition().y);
                if (deltaX < 12.0f and deltaY < 12.0f and stats.getPlayed() > 0)
                {
                    stats.display(game.get_window(), gameStatus,playerStatus, game);
                    break;
                }

                // Get the "key" from the "virtual keyboard"
                ltr = game.getKeyClicked(sf::Vector2f(mousePos.x, mousePos.y));
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                //case sf::Event::KeyPressed:
                if (music.getStatus() == sf::SoundSource::Status::Playing) music.stop();

                // Remove this backdoor
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Equal))
                {
                    cout << "The word is " << game.getWords().whatsTheWord() << endl;
                    break;
                }

                // Did the player ask for a hint?
                // Only 3 hints are available for practice games
                if (keyPressed->scancode == sf::Keyboard::Scancode::Period && playerStatus == Practice && targetRow < 6 && hintCount < 3)
                {
                    hintCount++;
                    game.hint(targetRow);
                    break;
                }

                // Did the use ask for a practice game?
                if (keyPressed->scancode == sf::Keyboard::Scancode::F1 and gameStatus == GameOver)
                {
                    gameStatus = NewGame;
                    playerStatus = Practice;
                    ltr = 0;
                    message = "";
                    continue;
                }

                // Get the key from the keyboard
                ltr = getKey(keyPressed);
            }
            // Process keyboard/mouse input
            switch (ltr)
            {
            case 0:
                break;
            case 'b':                                    // Backspace
                if (targetCol)
                {
                    targetCol--;
                    game.tile(targetRow,targetCol).setLetter(' ');
                }
                break;
            case 'e':                                    // Enter key
                game.processEnter(playerStatus, gameStatus, stats, targetRow, targetCol);
                break;
            default:
                if (ltr && targetCol < 5)
                {
                    game.playSound(letter);
                    game.tile(targetRow,targetCol).setLetter(ltr);
                    targetCol++;
                }
            }

            game.draw(message);

            if (gameStatus == Win or gameStatus == Loss)
            {
                game.display();
                if (playerStatus == Active)
                {
                    statsDisplayReturn = stats.display(game.get_window(), gameStatus,playerStatus, game);

                    // If it is an "actual" game, write out the statistics file
                    if (playerStatus == Active)
                    {
                        stats.writeStatsFile();
                    }

                    if (statsDisplayReturn == false)
                        message = "Press F1 to play a practice game";
                    else
                        message = "";
                    game.draw(message);
                }
                else   // It's a practice game
                {
                    gameStatus = GameOver;
                    message = "Press F1 to play another practice game";
                    game.draw(message);
                }
            }

            // Player asks for a new "practice" game?
            if (gameStatus == NewGame)
            {
                playerStatus = Practice;
                gameStatus = Started;
                music.play();
                targetRow = 0;
                targetCol = 0;
                hintCount = 0;
                game.startPracticeGame();
            }

            game.display();
            ltr = 0;
        }
    }
}

// Determine what key is pressed
char getKey(const auto* keyPressed)
{
    if (keyPressed->scancode == sf::Keyboard::Scancode::A)
        return  'A';
    if (keyPressed->scancode == sf::Keyboard::Scancode::B)
        return  'B';
    if (keyPressed->scancode == sf::Keyboard::Scancode::C)
        return  'C';
    if (keyPressed->scancode == sf::Keyboard::Scancode::D)
        return  'D';
    if (keyPressed->scancode == sf::Keyboard::Scancode::E)
        return  'E';
    if (keyPressed->scancode == sf::Keyboard::Scancode::F)
        return  'F';
    if (keyPressed->scancode == sf::Keyboard::Scancode::G)
        return  'G';
    if (keyPressed->scancode == sf::Keyboard::Scancode::H)
        return  'H';
    if (keyPressed->scancode == sf::Keyboard::Scancode::I)
        return  'I';
    if (keyPressed->scancode == sf::Keyboard::Scancode::J)
        return  'J';
    if (keyPressed->scancode == sf::Keyboard::Scancode::K)
        return  'K';
    if (keyPressed->scancode == sf::Keyboard::Scancode::L)
        return  'L';
    if (keyPressed->scancode == sf::Keyboard::Scancode::M)
        return  'M';
    if (keyPressed->scancode == sf::Keyboard::Scancode::N)
        return  'N';
    if (keyPressed->scancode == sf::Keyboard::Scancode::O)
        return  'O';
    if (keyPressed->scancode == sf::Keyboard::Scancode::P)
        return  'P';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
        return  'Q';
    if (keyPressed->scancode == sf::Keyboard::Scancode::R)
        return  'R';
    if (keyPressed->scancode == sf::Keyboard::Scancode::S)
        return  'S';
    if (keyPressed->scancode == sf::Keyboard::Scancode::T)
        return  'T';
    if (keyPressed->scancode == sf::Keyboard::Scancode::U)
        return  'U';
    if (keyPressed->scancode == sf::Keyboard::Scancode::V)
        return  'V';
    if (keyPressed->scancode == sf::Keyboard::Scancode::W)
        return  'W';
    if (keyPressed->scancode == sf::Keyboard::Scancode::X)
        return  'X';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Y)
        return  'Y';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Z)
        return  'Z';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
        return  ' ';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
        return  'e';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace)
        return  'b';
    return 0;
}
