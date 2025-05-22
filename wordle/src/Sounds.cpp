#include "Sounds.h"
#include <string>
#include <iostream>
using namespace std;

const std::string Sounds::winSoundFiles[7] =
{
    "lose","yeehaw","Cheering","Clapping","crowdhomerunapplause","crowdapplause","ywwowoo"
};

Sounds::Sounds() :
    letterSoundBuffer(LetterSoundFile),
    enterSoundBuffer(EnterSoundFile),
    dohSoundBuffer(DohSoundFile),
    letterSound(letterSoundBuffer),
    enterSound(enterSoundBuffer),
    dohSound(dohSoundBuffer)
{
}

void Sounds::playWinLoseSound(unsigned numberOfGuesses)
{
    string soundFile = ResourcePath + winSoundFiles[numberOfGuesses] + ".wav";
    if (!winLoseSoundBuffer.loadFromFile(soundFile)) std::cerr << "Unable to open sound file, " << soundFile << endl;
    sf::Sound* winLoseSound = new sf::Sound(winLoseSoundBuffer);
    winLoseSound->setVolume(40.0f);
    winLoseSound->play();
    sf::sleep(sf::Time(sf::seconds(3.0)));
    delete winLoseSound;
}

void Sounds::playSound(SoundType type)
{
    switch (type)
    {
    case letter:
        letterSound.play();
        break;
    case enter:
        enterSound.play();
        break;
    case doh:
        dohSound.play();
        break;
    default:
        ;
    }
}
