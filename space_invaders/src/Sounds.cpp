#include "Sounds.h"
#include <iostream>

void Sounds::createSound(const std::string& name, const std::string& soundFile, float volume)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(soundFile))
        std::cerr << "Unable to load " << soundFile << std::endl;
    sf::Sound temp(buffer);
    sounds[name] = sf::Sound(buffer);
    sounds[name].setVolume(volume);
}
