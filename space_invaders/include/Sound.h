#ifndef SOUND_H
#define SOUND_H

#include "constants.h"
#include <map>
#include <string>
#include <SFML/Audio.hpp>


class Sound
{
    public:
        Sound();
        ~Sound();
        void start(const std::string& name);
        void stop(const std::string& name);

    private:
        std::map<std::string, sf::Sound*> sound;
        void addSound(const std::string& name, const std::string& soundfile, float volume = 50);
};

#endif // SOUND_H
