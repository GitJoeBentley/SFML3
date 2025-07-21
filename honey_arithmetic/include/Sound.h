#ifndef SOUND_H
#define SOUND_H
#include <SFML/Audio.hpp>

extern const std::string ResourcesPath;

class Sound
{
public:
    enum SoundType {Tic,Yay,Boo,StartWord,OutOfTime};
    Sound();
    ~Sound();

    sf::Sound& operator[](SoundType type)
    {
        return *(sound[type]);
    }

private:
    sf::SoundBuffer soundBuffer[5];
    sf::Sound* sound[5];
};

#endif // SOUND_H
