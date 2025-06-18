#include <fstream>
#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>
#include "Message.h"
#include "ButtonBox.h"
#include "proto.h"
#include "Animation.h"

void displayGameDescription(size_t gameNumber, sf::RenderWindow& window, sf::Font& font)
{
    sf::Vector2u RenderWindowSize = window.getSize();

    const std::string digit = "0123456789D";
    const std::string filename = ResourcePath + "gamedescription" + digit[gameNumber] + ".txt";
    std::string text, buffer;

    std::ifstream fin(filename.c_str());
    if (!fin)
    {
        std::cout << "Cannot open game description file" << std::endl;
        return;
    }

    while (getline(fin, buffer))
    {
        text += buffer += '\n';
    }

    Message description(text, font, 20, sf::Vector2f(RenderWindowSize.x/2.0f, RenderWindowSize.y/ 2.0f));
    //sf::Mouse mouse;
    sf::Vector2f mousePos;
    sf::Vector2i mousePosI;
    sf::FloatRect buttRect = description.getContinueButtonPosition();
    while (window.isOpen())
    {
        mousePosI =  sf::Mouse::getPosition(window);
        mousePos.x = static_cast<float>(mousePosI.x);
        mousePos.y = static_cast<float>(mousePosI.y);
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if (buttRect.contains(mousePos))
                {
                    return;
                }
            }
        }
        description.drawMessage(window);
        window.display();
    }
}


bool hasJoystick()
{
    sf::Joystick::update();
    return sf::Joystick::isConnected(0);
}

// Determine what key is pressed
char getKey(const auto* keyPressed)
{
    if (keyPressed->scancode == sf::Keyboard::Scancode::A)
        return  'a';
    if (keyPressed->scancode == sf::Keyboard::Scancode::B)
        return  'b';
    if (keyPressed->scancode == sf::Keyboard::Scancode::C)
        return  'c';
    if (keyPressed->scancode == sf::Keyboard::Scancode::D)
        return  'd';
    if (keyPressed->scancode == sf::Keyboard::Scancode::E)
        return  'e';
    if (keyPressed->scancode == sf::Keyboard::Scancode::F)
        return  'f';
    if (keyPressed->scancode == sf::Keyboard::Scancode::G)
        return  'g';
    if (keyPressed->scancode == sf::Keyboard::Scancode::H)
        return  'h';
    if (keyPressed->scancode == sf::Keyboard::Scancode::I)
        return  'i';
    if (keyPressed->scancode == sf::Keyboard::Scancode::J)
        return  'j';
    if (keyPressed->scancode == sf::Keyboard::Scancode::K)
        return  'k';
    if (keyPressed->scancode == sf::Keyboard::Scancode::L)
        return  'l';
    if (keyPressed->scancode == sf::Keyboard::Scancode::M)
        return  'm';
    if (keyPressed->scancode == sf::Keyboard::Scancode::N)
        return  'n';
    if (keyPressed->scancode == sf::Keyboard::Scancode::O)
        return  'o';
    if (keyPressed->scancode == sf::Keyboard::Scancode::P)
        return  'p';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
        return  'q';
    if (keyPressed->scancode == sf::Keyboard::Scancode::R)
        return  'r';
    if (keyPressed->scancode == sf::Keyboard::Scancode::S)
        return  's';
    if (keyPressed->scancode == sf::Keyboard::Scancode::T)
        return  't';
    if (keyPressed->scancode == sf::Keyboard::Scancode::U)
        return  'u';
    if (keyPressed->scancode == sf::Keyboard::Scancode::V)
        return  'v';
    if (keyPressed->scancode == sf::Keyboard::Scancode::W)
        return  'w';
    if (keyPressed->scancode == sf::Keyboard::Scancode::X)
        return  'x';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Y)
        return  'y';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Z)
        return  'z';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
        return  ' ';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
        return  '\n';
    if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace)
        return  '\b';
    return ' ';
}

int getColorIndex(sf::Color color)
{
    for (int i = 0; i < 7; i++)
        if (color == RainbowColor[i]) return i;
    return -1;
}

void pollEvent(sf::RenderWindow& window, sf::Clock& clock, Game* game, sf::Vector2f& joystick)
{
    /////////////// Manage the move movement ///////////////
    sf::Vector2i currentMPos;
    static sf::Vector2i lastMPos = sf::Vector2i(0,0);
    static int minMouseX = static_cast<int>(GameBorderWidth + game->getPaddle()->getSize().x / 2);
    static int maxMouseX = static_cast<int>(GameWindowSize.x + GameBorderWidth - game->getPaddle()->getSize().x / 2);

    if (game->getDevice() == Game::Control::Mouse)
    {
        currentMPos = sf::Mouse::getPosition(window);
        if (currentMPos != lastMPos)
        {
            if (currentMPos.x < minMouseX)
            {
                sf::Mouse::setPosition(sf::Vector2i(minMouseX,sf::Mouse::getPosition(window).y), window);
                currentMPos.x = minMouseX;
            }
            if (currentMPos.x > maxMouseX)
            {
                sf::Mouse::setPosition(sf::Vector2i(maxMouseX,sf::Mouse::getPosition(window).y), window);
                currentMPos.x = maxMouseX;
            }
            game->getPaddle()->setPosition(sf::Vector2f(static_cast<float>(currentMPos.x), PaddleStartPosition.y));
            lastMPos = currentMPos;
        }
        return;
    }

    /////////////// Manage keyboard or joystick ///////////////
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
            return;
        }
        if (game->getDevice() == Game::Control::Mouse) return;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q) || sf::Joystick::isButtonPressed(0,0))
        {
            game->setStatus(Game::GameStatus::Quit);
            return;
        }

        if (game->getStatus() == Game::GameStatus::NotStarted)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) game->setDevice(Game::Control::Keyboard);
            if (sf::Joystick::isButtonPressed(0,7)) game->setDevice(Game::Control::Joystick);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) game->setDevice(Game::Control::Mouse);
            if (game->getDevice() != Game::Control::None)
            {
                window.setMouseCursorVisible(false);
                game->setStatus(Game::GameStatus::Active);
                clock.restart();
                game->decrementNumBalls();
            }
            continue;
        }

        if (game->getDevice() == Game::Control::Joystick)
        {
            sf::Joystick::update();
            joystick = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y));
        }

        // Handle the pressing and releasing of the arrow keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || joystick.x < -15.f)
        {
            game->getPaddle()->move(Paddle::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || joystick.x > 15.f)
        {
            game->getPaddle()->move(Paddle::Right);
        }
        else
        {
            game->getPaddle()->stop();
        }
    }
}

std::string getName(sf::RenderWindow& window, sf::Font& font, Game* game)
{
    std::string name;
    char input = ' ';
    std::string text = "Enter Your name for the High Scores Leaderboard ===> ";
    sf::Text namePrompt(font, text, 20);
    namePrompt.setPosition(sf::Vector2f(window.getSize().x/10.0f, 0.75f * window.getSize().y));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                break;
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                input = getKey(keyPressed);
                if ((input >= 'a' && input <= 'z')||(input >= 'A' && input <= 'Z')||input == ' ')
                {
                    name += input;
                }
                if (input == '\b') // backspace
                {
                    name = "";
                    text = "Enter Your name for the High Scores Leaderboard ===> ";
                }
            }
        }
        window.clear();
        namePrompt.setString((text+name+'_').c_str());
        game->drawGameObjects();
        window.draw(namePrompt);
        window.display();
        if (isspace(name[0]))
            name = name.substr(1);  // remove leading space from name
        name[0] = toupper(name[0]);

        if (input == '\n')
        {
            window.clear();
            return name;
        }
    }
    return name;
}

bool buttonIsClicked(sf::RectangleShape& button, sf::Vector2f mousePosition)
{
    sf::FloatRect rectangle = button.getGlobalBounds();
    return rectangle.contains(mousePosition);
}

void drawCenteredText(sf::Text& text, sf::RenderWindow& window)
{
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    window.draw(text);
}

GameSelection playAgain(sf::RenderWindow& window, sf::Font& font, sf::Text* title)
{
    GameSelection choice[3] = {GameSelection::Same, GameSelection::New, GameSelection::Exit};

    sf::Vector2f mousePos;
    sf::Vector2i mousePosI, saveMousePos;

    sf::SoundBuffer ticBuffer;
    if (!ticBuffer.loadFromFile(TicSoundFile))
        cerr<< "Unable to load file " << TicSoundFile << endl;
    sf::Sound ticSound(ticBuffer);
    ticSound.setVolume(100.0f);

    std::vector<std::string> buttonLabels = {"Same Game", "New Game","Exit"};
    size_t numButtons = buttonLabels.size();
    ButtonBox buttonBox(window, buttonLabels, font, 24 );
    sf::FloatRect* butRect = new sf::FloatRect[numButtons];
    for (size_t i = 0; i < numButtons; i++)
        butRect[i] = buttonBox.getButton(i)->getGlobalBounds();

    while (window.isOpen())
    {
        mousePosI = sf::Mouse::getPosition(window);
        mousePos.x = static_cast<float>(mousePosI.x);
        mousePos.y = static_cast<float>(mousePosI.y);
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return GameSelection::Exit;
            }

            for (size_t i = 0; i < numButtons; i++)
            {
                if (butRect[i].contains(mousePos))
                {
                    buttonBox.getButton(i)->highlight();
                    ticSound.play();

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        delete [] butRect;
                        butRect = nullptr;
                        return choice[i];
                    }
                }
                else buttonBox.getButton(i)->normal();
            }
        }
        window.clear();
        window.draw(*title);

        buttonBox.drawButtonBox();
        window.display();
    }

    return GameSelection::Exit;
}

int start(sf::RenderWindow& window, sf::Font& font, sf::Text* title)
{
    sf::Vector2f mousePos;
    sf::Vector2i mousePosI, saveMousePos;
    sf::SoundBuffer ticBuffer(TicSoundFile);
    sf::Sound ticSound(ticBuffer);
    ticSound.setVolume(100.0f);
    sf::Clock clock;
    sf::Time dt;

    size_t numButtons = GameName.size();
    ButtonBox buttonBox(window, GameName, font, 20 );
    sf::FloatRect* butRect = new sf::FloatRect[numButtons];
    for (size_t i = 0; i < numButtons; i++)
        butRect[i] = buttonBox.getButton(i)->getGlobalBounds();

    sf::Vector2f buttonBoxUpperLeft = buttonBox.getUpperLeftCorner();
    Animation** animation = new Animation*[10];
    for (int i = 0; i < 5; i++)
    {
        animation[i] = new Animation(sf::Vector2f(buttonBoxUpperLeft.x - 360.f, buttonBoxUpperLeft.y + i * 142.0f));
        animation[i + 5] = new Animation(sf::Vector2f(buttonBoxUpperLeft.x + buttonBox.getBoxSize().x + 40.f, buttonBoxUpperLeft.y + i * 142.0f));
    }

    while (window.isOpen())
    {
        dt = clock.restart();
        mousePosI = sf::Mouse::getPosition(window);
        mousePos.x = static_cast<float>(mousePosI.x);
        mousePos.y = static_cast<float>(mousePosI.y);
        while (const std::optional event = window.pollEvent())
        {
            for (size_t i = 0; i < numButtons; i++)
            {
                if (butRect[i].contains(mousePos))
                {
                    buttonBox.getButton(i)->highlight();
                    ticSound.play();
                }
                else buttonBox.getButton(i)->normal();
            }

            if (event->is<sf::Event::Closed>())
            {
                window.close();
                exit(0);
            }

            if (event->getIf<sf::Event::MouseButtonPressed>())
            {
                for (size_t i = 0; i < numButtons; i++)
                {
                    if (butRect[i].contains(mousePos))
                    {
                        buttonBox.getButton(i)->highlight();
                        ticSound.play();
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                        {

                            if (i == numButtons - 1)
                            {
                                saveMousePos = sf::Mouse::getPosition(window);
                                displayGameDescription(i, window, font);
                                sf::Mouse::setPosition(saveMousePos);
                                break;
                            }
                            // Clean up before leaving
                            delete [] butRect;
                            butRect = nullptr;
                            for (int i = 0; i < 10; i++)
                            {
                                delete animation[i];
                                animation[i] = nullptr;
                            }
                            delete [] animation;
                            animation = nullptr;

                            return i;
                        }
                        else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                        {
                            saveMousePos = sf::Mouse::getPosition();
                            displayGameDescription(i, window, font);
                            sf::Mouse::setPosition(saveMousePos);
                            break;
                        }
                    }
                }
            }

        }
        // process animations
        for (int i = 0; i < 10; i++)
        {
            animation[i]->update(dt);
            animation[i]->hitATile();
        }
        window.clear();

        window.draw(*title);
        buttonBox.drawButtonBox();
        for (int i = 0; i < 10; i++) animation[i]->drawAnimation(window);
        window.display();
    }

    return -1;
}
