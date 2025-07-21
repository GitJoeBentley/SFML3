#include "proto.h"
#include "constants.h"
#include "ButtonBox.h"
#include <cmath>
#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


float distanceBetweenTwoPositions(sf::Vector2f p1, sf::Vector2f p2)
{
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

size_t start(sf::RenderWindow& window, HighScores& highScores, sf::Font& font, Sound& sound)
{
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    bool gotChoice = false;;
    size_t choice;
    sf::Vector2f mousePos;
    sf::Vector2i mousePosI, saveMousePos;
    sf::Font courier(CourierFont);

    sf::Text title(font,"Honey's Arithmetic Game");
    title.setCharacterSize(64u);
    sf::FloatRect textRect = title.getLocalBounds();
    title.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    title.setPosition(sf::Vector2f(windowWidth / 2.f, 0.09f * windowHeight));
    title.setFillColor(sf::Color::Magenta);

    std::vector<std::string> buttonLabels = {"Add", "Subtract", "Add and Subtract", "Multiply", "All types of Problems"};
    size_t numButtons = buttonLabels.size();
    ButtonBox buttonBox(window, buttonLabels, courier, 0.33f, 24);
    sf::FloatRect* butRect = new sf::FloatRect[numButtons];
    for (size_t i = 0; i < numButtons; i++)
        butRect[i] = buttonBox.getButton(i)->getGlobalBounds();

    sf::Texture texture[4];
    sf::RectangleShape* pic[4];
    float leftEdge =0.f;
    float rightEdge = windowWidth - 300.f;
    float topEdge = 0.f;
    float bottomEdge = windowHeight - 400.f;
    sf::Vector2f picPos[4] = { sf::Vector2f(leftEdge, topEdge),
                               sf::Vector2f(leftEdge, bottomEdge),
                               sf::Vector2f(rightEdge, topEdge),
                               sf::Vector2f(rightEdge, bottomEdge)
                             };
    for (int i = 0; i < 4; i++)
    {
        if (!texture[i].loadFromFile(Pic[i]))
        {
            cerr << "Unable to load texture image file: " << Pic[i] << endl;
            continue;
        }
        pic[i] = new sf::RectangleShape(PicSize);
        pic[i]->setTexture(texture + i);
        pic[i]->setPosition(picPos[i]);
    }

    // Create High Scores text
    sf::Text highScoresText(courier, "", 20);
    highScoresText.setStyle(sf::Text::Style::Bold);
    highScoresText.setFillColor(sf::Color::White);

    // Write High Scores
    std::ostringstream sout;
    sout << highScores << std::endl;
    highScoresText.setString(sout.str());
    textRect = highScoresText.getLocalBounds();
    highScoresText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    highScoresText.setPosition(sf::Vector2f(windowWidth / 2.f, .73f * windowHeight));

    sf::Music music;
    if (!music.openFromFile("c:/temp/honey.ogg"))
        cerr << "Unable to open music" << endl;
    music.setVolume(90);
    music.play();
    while (window.isOpen() && !gotChoice)
    {
        mousePosI = sf::Mouse::getPosition(window);
        mousePos.x = static_cast<float>(mousePosI.x);
        mousePos.y = static_cast<float>(mousePosI.y);
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                choice = numButtons;
                gotChoice = true;
                break;
            }

            for (size_t i = 0; i < numButtons; i++)
            {
                if (butRect[i].contains(mousePos))
                {
                    buttonBox.getButton(i)->highlight();
                    sound[Sound::Tic].play();

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        delete [] butRect;
                        butRect = nullptr;
                        choice = i;
                        gotChoice = true;
                        break;
                    }
                }
                else buttonBox.getButton(i)->normal();
            }
            if (gotChoice) break;
            for (int i = 0; i < 4; i++) window.draw(*(pic[i]));

            window.draw(title);
            window.draw(highScoresText);
            buttonBox.drawit();
            window.display();
        }
    }

    for (int i = 0; i < 4; i++) delete pic[i];

    return choice;
}

bool playAgain(sf::RenderWindow& window, sf::Font& font, const std::string& statement, HighScores& highScores, sf::Font& courier)
{
    sf::Vector2f mousePos;
    sf::Vector2i mousePosI, saveMousePos;

    std::vector<std::string> buttonLabels = {"Play Again", "Exit"};
    size_t numButtons = buttonLabels.size();
    ButtonBox buttonBox(window, buttonLabels, font, 0.43f, 24 );
    sf::FloatRect* butRect = new sf::FloatRect[numButtons];
    for (size_t i = 0; i < numButtons; i++)
        butRect[i] = buttonBox.getButton(i)->getGlobalBounds();

    sf::Text msg(font, statement, 64);
    sf::FloatRect textRect = msg.getLocalBounds();
    msg.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    msg.setPosition(sf::Vector2f(window.getSize().x / 2.f, 0.15f * window.getSize().y));
    msg.setFillColor(sf::Color::White);

    // Create High Scores text
    sf::Text highScoresText(courier, "", 20);
    highScoresText.setStyle(sf::Text::Style::Bold);
    highScoresText.setFillColor(sf::Color::White);

    // Write High Scores
    std::ostringstream sout;
    sout << highScores << std::endl;
    highScoresText.setString(sout.str());
    textRect = highScoresText.getLocalBounds();
    highScoresText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    highScoresText.setPosition(sf::Vector2f(window.getSize().x / 2.f, 0.77f * window.getSize().y));
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            mousePosI = sf::Mouse::getPosition(window);
            mousePos.x = static_cast<float>(mousePosI.x);
            mousePos.y = static_cast<float>(mousePosI.y);
            for (size_t i = 0; i < numButtons; i++)
            {
                if (butRect[i].contains(mousePos))
                {
                    buttonBox.getButton(i)->highlight();
                }
                else buttonBox.getButton(i)->normal();
            }
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                for (size_t i = 0; i < numButtons; i++)
                {
                    if (butRect[i].contains(mousePos))
                    {
                        delete [] butRect;
                        butRect = nullptr;
                        return i == 0 ? true : false;
                    }
                }
            }
            break;
        }
        window.clear();

        buttonBox.drawit();
        window.draw(highScoresText);
        window.draw(msg);
        window.display();
    }
    return false;
}
