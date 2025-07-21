#include "Sound.h"
#include "constants.h"
#include <iostream>

Sound::Sound()
{
    std::string filenames[5] = {"Tic","Yay","Boo","StartWord","OutOfTime"};
    for (auto i = 0; i < 5; i++)
    {
        std::string filename = ResourcePath + filenames[i] + ".wav";
        if (!soundBuffer[i].loadFromFile(filename))
            std::cerr << "Unable to open file: " << filename << std::endl;
        else sound[i] = new sf::Sound(soundBuffer[i]);
    }

}

Sound::~Sound()
{
    for (auto i = 0; i < 5; i++)
    {
        delete sound[i];
        sound[i] = nullptr;
    }
}
