#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "constants.h"
#include "proto.h"
#include "TextBox.h"
#include "NumberCircle.h"
#include "Numbers.h"
#include "Problem.h"
#include "Sound.h"
#include "HighScores.h"
using namespace std;

sf::Vector2f Vector2u_to_Vector2f(sf::Vector2u object);

int main()
{
    srand(time(0));
    bool gameOver = false;
    bool problemComplete = false;
    bool again = true;

    int numGuesses = 0;
    //int numProblems = 10;
    int numProblems = 3;
    int score = 0;
    int problemNumber = 0;
    int indexOfAnswer;
    int gameNumber;

    // Create the main window
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Honey's Arithmetic Game");

    sf::Font arial(ArialFont);
    sf::Font courier(CourierFont);

    //GAME CLOCK & TIMER
    sf::Clock clock;
    int timer;
    int countDown;

    Sound sound;

    HighScores highScores;

    sf::Text title(arial,"Honey's Arithmetic Game");
    title.setCharacterSize(64u);
    sf::FloatRect textRect = title.getLocalBounds();
    title.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    title.setPosition(sf::Vector2f(desktopMode.size.x / 2.f, desktopMode.size.y * .04f));

    Problem* problem = nullptr;
    TextBox* problemTB = nullptr;
    std::vector<TextBox*> values;
    Numbers* numbers = nullptr;

    sf::RectangleShape bottomLine(sf::Vector2f(1.0f * window.getSize().x, window.getSize().y * 0.05f));
    bottomLine.setPosition(sf::Vector2f(0.0f, window.getSize().y * 0.875f));
    bottomLine.setFillColor(sf::Color::Blue);

    sf::Text scoreText(arial, "Score 0", 28u);
    scoreText.setPosition(sf::Vector2f(window.getSize().x * 0.1f, window.getSize().y * 0.882f));
    scoreText.setFillColor(sf::Color::White);

    sf::Text timeText(arial, "Time", 28u);
    timeText.setPosition(sf::Vector2f(window.getSize().x * 0.395f, window.getSize().y * 0.882f));
    timeText.setFillColor(sf::Color::White);

    float timeBarWidth = 400.0f;
    float timeBarHeight = 20.f;
    sf::RectangleShape timeBar(sf::Vector2f(timeBarWidth, timeBarHeight));
    timeBar.setOutlineThickness(2.0f);
    timeBar.setOutlineColor(sf::Color::Red);
    timeBar.setFillColor(sf::Color::White);
    timeBar.setPosition(sf::Vector2f(window.getSize().x/2.3f, window.getSize().y * 0.891f));

    sf::Text problemText(arial, "Problem #1", 28u);
    problemText.setPosition(sf::Vector2f(window.getSize().x * 0.8f, window.getSize().y * 0.88f));
    problemText.setFillColor(sf::Color::White);

    sf::Vector2f mousePos;
    sf::Vector2i mousePosI;

    gameNumber = start(window, highScores, arial, sound);
    while (again)
    {
        gameOver = false;
        problemNumber = 0;
        score = 0;
        scoreText.setString("Score " + to_string(score));
        // Start the game loop
        while (!gameOver)
        {
            while (problemNumber < numProblems)
            {
                problem = new Problem(gameNumber);
                problemTB = new TextBox(window, static_cast<string>(*problem), 64u, sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
                values.clear();
                numbers = new Numbers(window, *problem, *problemTB);
                sound[Sound::StartWord].play();
                problemNumber++;
                problemText.setString("Problem #" + to_string(problemNumber));
                problemComplete = false;
                numGuesses = 0;
                countDown = 10;

                while (!problemComplete)
                {

                    mousePosI = sf::Mouse::getPosition(window);
                    mousePos.x = static_cast<float>(mousePosI.x);
                    mousePos.y = static_cast<float>(mousePosI.y);

                    timer = static_cast<int> (clock.getElapsedTime().asSeconds());
                    if (timer > 0)
                    {
                        countDown--;
                        clock.restart();
                        timeBar.setSize(sf::Vector2f(timeBarWidth*countDown/10.0f, timeBarHeight));
                    }
                    if (countDown < 0)
                    {
                        sound[Sound::OutOfTime].play();
                        timeBar.setSize(sf::Vector2f(timeBarWidth*countDown/10.0f, timeBarHeight));
                        window.draw(timeBar);
                        window.display();
                        indexOfAnswer = numbers->getIndex(*problem);
                        numbers->numberCircle(indexOfAnswer)->emphasize();
                        score--;
                        scoreText.setString("Score " + to_string(score));
                        problemComplete = true;
                        break;
                    }

                    for (size_t i = 0; i < NumNumbers; i++)
                    {
                        if (distanceBetweenTwoPositions(numbers->getCirclePosition(i), mousePos) < CircleRadius)
                        {
                            if (!numbers->numberCircle(i)->isHighlighted())
                            {
                                sound[Sound::Tic].play();
                                numbers->numberCircle(i)->highlight();
                            }
                        }
                        else
                        {
                            if (numbers->numberCircle(i)->isHighlighted())
                            {
                                numbers->numberCircle(i)->normal();
                            }
                        }
                    }

                    // Process events
                    while (const std::optional event = window.pollEvent())
                    {
                        // Close window: exit
                        if (event->is<sf::Event::Closed>())
                        {
                            problemComplete = true;
                            gameOver = true;
                            break;
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q))
                        {
                            problemComplete = true;
                            gameOver = true;
                            break;
                        }

                        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                        {
                            for (size_t i = 0; i < NumNumbers; i++)
                            {
                                if (distanceBetweenTwoPositions(numbers->getCircle(i).getPosition(), mousePos) < CircleRadius)
                                {
                                    numGuesses++;
                                    auto temp = numbers->numberCircle(i)->getStr();
                                    if (std::stoi(temp) == static_cast<int>(*problem))
                                    {
                                        score += 5;
                                        scoreText.setString("Score " + to_string(score));
                                        sound[Sound::Yay].play();
                                        numbers->numberCircle(i)->flash();
                                        problemComplete = true;

                                    }
                                    else
                                    {
                                        sound[Sound::Boo].play();

                                        numbers->numberCircle(i)->flash(sf::Color::Yellow);
                                        if (numGuesses == 3)
                                        {
                                            problemComplete = true;
                                            indexOfAnswer = numbers->getIndex(*problem);
                                            numbers->numberCircle(indexOfAnswer)->emphasize();
                                        }
                                        score--;

                                    }
                                    scoreText.setString("Score " + to_string(score));
                                    if (problemComplete)
                                    {
                                        break;
                                    }
                                }
                            }
                        }

                        if (problemComplete)
                        {
                            sf::sleep(sf::Time(sf::seconds(1.0f)));

                            break;
                        }
                    }
                    if (problemNumber > numProblems)
                    {
                        scoreText.setString("Score " + to_string(score));
                        gameOver = true;
                        sf::sleep(sf::Time(sf::seconds(1.0f)));
                    }

                    // Clear screen
                    window.clear();

                    // Update the window
                    window.draw(title);
                    problemTB->draw();
                    numbers->draw();
                    window.draw(bottomLine);
                    window.draw(timeBar);
                    window.draw(scoreText);
                    window.draw(timeText);
                    window.draw(problemText);
                    window.display();
                }
            }                // while (problemNumber <= numProblems)
            if (problemNumber == numProblems)
            {
                gameOver = true;
                scoreText.setString("Score " + to_string(score));
                window.clear();

                // Update the window
                window.draw(title);
                problemTB->draw();
                    numbers->draw();
                window.draw(bottomLine);
                window.draw(timeBar);
                window.draw(timeText);
                window.draw(problemText);
                window.draw(scoreText);
                window.display();
                sf::sleep(sf::Time(sf::seconds(2.0f)));
            }
            if (problem)
            {
                delete problem;
                problem = nullptr;
            }
            if (problemTB)
            {
                delete problemTB;
                problemTB = nullptr;
            }
            if (numbers)
            {
                delete numbers;
                numbers = nullptr;
            }
        }
        bool highScoresEligible = highScores.eligible(score);
        std::string statement;
        if (problemNumber == numProblems)
        {
            if (highScoresEligible)
            {
                if (score > highScores.getHightestScore())
                    statement += "          Congratulations!!!\nYou have the highest score!!!";
                else statement += "        Congratulations!!!\n You made the leader board";
                highScores.updateHighScores(Score(score,time(0)));
                highScores.WriteHighScoresFile();

            }
            else statement = "Game Over";
        }
        again = playAgain(window, arial, statement, highScores, courier);
    }
    window.close();
    cerr << "That's all folks!!!" << endl;
    return 0;
}

sf::Vector2f Vector2u_to_Vector2f(sf::Vector2u object)
{
    return sf::Vector2f(static_cast<float>(object.x), static_cast<float>(object.y));
}
