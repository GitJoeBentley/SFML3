#ifndef SOUNDS_H
#define SOUNDS_H
#include <map>
#include <string>
#include <SFML/Audio.hpp>


class Sounds
{
    public:
        Sounds() {}
        void createSound(const std::string& name, const std::string& soundfile, float volume);
        void play(const std::string& name);
        void stop(const std::string& name);

    private:
        std::map<std::string, sf::Sound> sounds;
};

#endif // SOUNDS_H
