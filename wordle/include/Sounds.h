#ifndef SOUNDS_H
#define SOUNDS_H
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Types.h"

class Sounds
{
public:

    Sounds();
    virtual ~Sounds() = default;
    void playSound(SoundType type);
    void playWinLoseSound(unsigned numberOfGuesses);
    static const std::string winSoundFiles[7];

private:
    sf::SoundBuffer letterSoundBuffer;
    sf::SoundBuffer enterSoundBuffer;
    sf::SoundBuffer dohSoundBuffer;
    sf::SoundBuffer winLoseSoundBuffer;
    //sf::SoundBuffer loseSoundBuffer;
    sf::Sound letterSound;
    sf::Sound enterSound;
    sf::Sound dohSound;
    //sf::Sound winLoseSound;
   // sf::Sound loseSound;
};

#endif // SOUNDS_H
