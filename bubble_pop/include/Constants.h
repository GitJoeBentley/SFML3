
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>
#include <string>

const sf::Vector2u MainWindowSize{1024, 640};
const std::string ResourceDirectory = "./resources/";
const std::string WelcomeFile = ResourceDirectory + "welcome.txt";
const std::string BubbleFile = ResourceDirectory + "bubble.png";
const std::string BubbleSoundFile = ResourceDirectory + "WormPop.wav";
const std::string BigBubbleSoundFile = ResourceDirectory + "mypop.wav";
const std::string EndOfGameSoundFile = ResourceDirectory + "EndOfGame.wav";
const std::string ArialFont = ResourceDirectory + "arial.ttf";
const std::string CourierFont = ResourceDirectory + "CourierNew.ttf";
const std::string BubbleSong = ResourceDirectory + "Lawrence_welk_bubbles_in_the_wine.ogg";



#endif /* CONSTANTS_H */
