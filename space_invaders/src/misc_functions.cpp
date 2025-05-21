#include <iostream>
#include <fstream>
#include <sstream>
#include "Proto.h"
#include "Constants.h"
#include "Gun.h"
#include "Saucer.h"
#include "Sound.h"

using std::endl;
using std::cout;
using std::cerr;


Control start(sf::RenderWindow& window, const HighScores& highScores, Invaders& invaders,
              const sf::Texture& saucerTexture, sf::Texture& bombTexture, sf::Texture& gunTexture)
{
    std::string text, buffer;
    Control control = Control::Keyboard;

    // Music
    sf::Music music;
    if (!music.openFromFile(OpeningMusicFile))
        cerr << "Unable to open " << OpeningMusicFile << endl;
    music.setVolume(40);
    music.play();

    // Text
    sf::Font font;
    if (!font.openFromFile(CourierNewFontFile))
    {
        std::cout << "Can't find font" << std::endl;
    }

    std::ifstream fin(StartFile);
    if (!fin)
    {
        std::cout << "Cannot open welcome file." << std::endl;
        return control;
    }

    while (getline(fin, buffer))
    {
        text += buffer += '\n';
    }

    sf::Text instructions(font, text, 20);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition(sf::Vector2f(90.0f,20.0f));

    // Create High Scores text
    sf::Text highScoresText(font, "", 20);
    highScoresText.setFillColor(sf::Color::Yellow);
    highScoresText.setPosition(sf::Vector2f(480.0f,120.0f));

    // Write High Scores
    std::ostringstream sout;
    sout << highScores << std::endl;
    highScoresText.setString(sout.str());

    // Create and position a gun
    Gun gun(gunTexture);
    gun.moveToPosition(sf::Vector2f(56.0f,191.0f));

    // Position 5 invaders
    for (auto i = 0; i < 5; i++)
    {
        invaders.getInvader(i,0).setPosition(sf::Vector2f(55.0f,241.0f + i * 45.0f));
    }

    // create and position a saucer
    Saucer* saucerPtr = new Saucer(saucerTexture);
    saucerPtr->setPosition(sf::Vector2f(50.0f,471.0f));

    // create and position 3 bombs
    Bomb* bomb[3];

    for (auto i = 0; i < 3; i++)
    {
        bomb[i] = new Bomb(bombTexture, i);
        bomb[i]->setPosition(sf::Vector2f(40.0f+ i*16.0f, 543.0f));
    }

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Enter)) return Control::Keyboard;
            if (sf::Joystick::isButtonPressed(0,0)) return Control::Joystick;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) return Control::Mouse;
        }

        window.clear();
        window.draw(instructions);
        window.draw(highScoresText);

        window.draw(gun.getGun());

        for (auto i = 0; i < 5; i++)
        {
            window.draw(invaders.getInvader(i,0));
        }
        window.draw(*saucerPtr);
        for (auto i = 0; i < 3; i++)
        {
            window.draw(*(bomb[i]));
        }

        window.display();
    }
    music.stop();
    delete saucerPtr;
    for (int i = 0; i < 3; i++) delete bomb[i];
    return control;
}

std::string getWelcomeText()
{
    std::ifstream fin(StartFile);
    std::string text, buffer;
    if (!fin)
    {
        std::cout << "Cannot open welcome file." << std::endl;
        return "";
    }

    while (getline(fin, buffer))
    {
        text += buffer += '\n';
    }

    //text += "\n          Please type your name and press Enter ===> ";

    fin.close();
    return text;
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

void displayGameObjects(sf::RenderWindow& window, sf::RectangleShape& background, sf::Text& text, Gun* guns,
                        Invaders& invaders, Explosion& explosion, Bomb* bombPtr, Saucer* saucerPtr,
                        std::list<Bullet*>& bulletsInFlight, sf::Text& gameOverText, Shield* shields)
{
    window.clear();
    window.draw(background);
    window.draw(text);
    invaders.draw(window);
    if (guns[0].isVisible())
        window.draw(guns[0].getGun());
    if (guns[1].isVisible())
        window.draw(guns[1].getGun());
    if (guns[2].isVisible())
        window.draw(guns[2].getGun());
    if (explosion.isExploding())
        window.draw(explosion.getExplosion());
    if (bombPtr)
        window.draw(*bombPtr);
    if (saucerPtr)
        window.draw(*saucerPtr);
    // draw bullets
    for (auto it = bulletsInFlight.begin(); it!= bulletsInFlight.end(); ++it)
        window.draw(**it);
    window.draw(gameOverText);
    window.draw(shields[0].getShield());
    window.draw(shields[1].getShield());
    window.draw(shields[2].getShield());
    window.display();
}

/////////// Handle keyboard, mouse, and joystick ////////////////////////
void pollEvent(sf::RenderWindow& window, const Control& control, Sound& sound, bool& pauseFlag, bool& gameOver, Gun*& guns, std::list<Bullet*>& bulletsInFlight, Score& score)
{
    bool fire;
    if (control == Control::Mouse)
    {
        sf::Vector2i currentMPos;
        static sf::Vector2i lastMPos = sf::Vector2i(0,0);
        static int minMouseX = static_cast<int>(ShieldSize.x / 2);
        static int maxMouseX = static_cast<int>(MainWindowWidth - ShieldSize.x / 2);
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
            guns[0].setPosition(sf::Vector2f(static_cast<float>(currentMPos.x), 0.96f * MainWindowHeight));
            lastMPos = currentMPos;
        }
    }

    while (const std::optional event = window.pollEvent())
    {
        fire = false;
        if (sf::Joystick::isButtonPressed(0, 2) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
        {
            fire = true;
            break;
        }

        float joystickDirection = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);

        if (joystickDirection < -25.f)
        {
            guns[0].move(Gun::Left, GunSpeed / GunSpeed);
            break;
        }
        if (joystickDirection > 25.f)
        {
            guns[0].move(Gun::Right, GunSpeed / GunSpeed);
            break;
        }

        // Close window: exit
        if (event->is<sf::Event::Closed>()) window.close();
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
            {
                sound.stop("Saucer");
                pauseFlag = !pauseFlag;
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
                gameOver = true;
            // Move the gun left or right
            else if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
            {
                guns[0].move(Gun::Right);
                break;
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
            {
                guns[0].move(Gun::Left);
                break;
            }
        }
    }

    // Limit the number of bullets that can be fired at one time
    if (fire && bulletsInFlight.size() < 1)
    {
        bulletsInFlight.push_back(guns[0].shoot());
        score += -1;                                   // subtract one point for each bullet
        sound.start("Bullet");
    }
}

void manageBullets(std::list<Bullet*>& bulletsInFlight, Shield* shields, Invaders& invaders, Sound& sound, Explosion& explosion,
                   Score& score, Bomb*& bombPtr, bool& gameOver, Saucer*& saucerPtr, sf::RenderWindow& window)
{
    //bool bulletHitsShield;
    int invaderHit;
    sf::Vector2f shieldPos;
    sf::Vector2f bulletPos;
    for (auto it = bulletsInFlight.begin(); it!= bulletsInFlight.end(); ++it)
    {
        // Bullet moved off the top of the window
        if (!(*it)->move())
        {
            delete (*it);
            *it = nullptr;
            bulletsInFlight.erase(it);
            return;
        }
        /// Bullet hits shield?
        for (int i = 0; i < 3; ++i)
        {
            if (shields[i].isAlignedWithBullet(**it))
            {
                shieldPos = shields[i].getShield().getPosition();
                bulletPos = (*it)->getPosition();
                sf::Vector2f shieldPositionToExamine = sf::Vector2f(32 + (bulletPos.x-shieldPos.x),22.5f + (bulletPos.y - shieldPos.y));
                if (shields[i].hitByBullet(shieldPositionToExamine))
                {
                    // remove bullet
                    delete (*it);
                    *it = nullptr;
                    bulletsInFlight.erase(it);
                    return;
                }
            }
        }

        /// Bullet hits invader?
        invaderHit = invaders.invaderHitByBullet(**it);
        if (invaderHit)
        {
            score += invaderHit;
            sound.start("Explosion");
            bulletsInFlight.erase(it);

            if (invaders.getCountVisible() == 0)
                gameOver = true;
            // add a random invader, maybe
            else if (!gameOver && invaders.getCountVisible() < 10 && rand() % 2  == 0)
            {
                invaders.addRandomInvader();
            }
            else {};
            return;
        }

        /// Bullet hits bomb?
        if (bombPtr && bombPtr->hitByBullet(**it))
        {
            score += 20;  // 20 points for hitting a bomb
            sound.stop("Bomb");
            sound.start("BombExplosion");
            explosion.startExplosion(bombPtr->getPosition());

            // remove bullet
            delete (*it);
            *it = nullptr;
            bulletsInFlight.erase(it);

            // remove the bomb
            delete bombPtr;
            bombPtr = nullptr;
            return;
        }
        /// Bullet hits saucer?
        else if (saucerPtr && saucerPtr->hitByBullet(**it))
        {
            score += 10;  // 10 points for hitting a saucer
            sound.stop("Saucer");
            sound.start("BombExplosion");
            explosion.startExplosion(saucerPtr->getPosition());

            // remove bullet
            delete (*it);
            *it = nullptr;
            bulletsInFlight.erase(it);

            // remove the saucer
            delete saucerPtr;
            saucerPtr = nullptr;
            return;
        }
        else
        {
            window.draw(**it);
        }
    }
}

void processGameEnd(sf::RenderWindow& window, sf::RectangleShape& background, sf::Text& text, sf::Font& font,
                    Gun* guns, Invaders& invaders, Shield* shields, const Score& score, HighScores& highScores)
{
    std::string statement;
    bool highScoresEligible = highScores.eligible(score);

    if (highScoresEligible)
    {
        if (score > highScores.getHightestScore())
            statement += "\n         Congratulations!!!\nYou have the highest score!!!\n";
        else statement += "\n        Congratulations!!!\nYou made the leader board\n";
    }
    else statement = "          Game Over\n\nPress Enter to Continue";

    std::string name;
    char input = ' ';
    std::string prompt = "Enter your name for the High Scores Leaderboard ===> ";
    sf::Text namePrompt(font, statement, 24 );
    namePrompt.setPosition(sf::Vector2f(window.getSize().x/10.0f, 0.75f * window.getSize().y));

    sf::Text gameOverText(font, statement, 48);
    gameOverText.setFillColor(sf::Color(0xff0000ff));
    sf::FloatRect textRect = gameOverText.getLocalBounds();

    gameOverText.setOrigin(sf::Vector2f(textRect.size.x / 2.0f, textRect.size.y / 2.0f));
    gameOverText.setPosition(sf::Vector2f(MainWindowWidth/2.0f, MainWindowHeight/2.0f));

    sf::RectangleShape textBackground(textRect.size);
    textBackground.setOrigin(sf::Vector2f(textRect.size.x / 2.0f, textRect.size.y / 2.0f));
    textBackground.setPosition(gameOverText.getPosition());
    textBackground.setFillColor(sf::Color(0x010101ab));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
                {
                    if (highScoresEligible)
                    {
                        highScores.updateHighScores(Score(name,score,time(0)));
                        highScores.WriteHighScoresFile();

                    }
                    return;
                }
                input = getKey(keyPressed);
                if ((input >= 'a' && input <= 'z')||(input >= 'A' && input <= 'Z')||input == ' ')
                {
                    name += input;
                }
                if (input == '\b') // backspace
                {
                    name = "";
                    // ?
                }
                break;
            }
        }
        window.clear();
        namePrompt.setString((prompt + name + '_'));
        window.draw(background);
        window.draw(text);
        invaders.draw(window);
        if (guns[0].isVisible())
            window.draw(guns[0].getGun());
        if (guns[1].isVisible())
            window.draw(guns[1].getGun());
        if (guns[2].isVisible())
            window.draw(guns[2].getGun());
        window.draw(shields[0].getShield());
        window.draw(shields[1].getShield());
        window.draw(shields[2].getShield());
        window.draw(textBackground);
        window.draw(gameOverText);
        if (highScoresEligible) window.draw(namePrompt);
        window.display();
        if (isspace(name[0]))
            name = name.substr(1);  // remove leading space from name
        name[0] = toupper(name[0]);
    }
}
