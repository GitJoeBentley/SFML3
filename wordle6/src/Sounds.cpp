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
    letterSound(letterSoundBuffer),

    enterSoundBuffer(EnterSoundFile),
    enterSound(enterSoundBuffer),

    //dohSoundBuffer(DohSoundFile),
    dohSoundBuffer(WinSoundFile),
    dohSound(dohSoundBuffer),

    winSoundBuffer(WinSoundFile),
    winSound(winSoundBuffer),

    loseSoundBuffer(LoseSoundFile),
    loseSound(loseSoundBuffer)
{
}

void Sounds::playWinLoseSound(unsigned numberOfGuesses)
{
    //string soundFile = ResourcePath + winSoundFiles[numberOfGuesses] + ".wav";
    //if (!winLoseSoundBuffer.loadFromFile(soundFile)) std::cerr << "Unable to open sound file, " << soundFile << endl;
    //sf::Sound* winLoseSound = new sf::Sound(winLoseSoundBuffer);
    //winLoseSound->setVolume(40.0f);
    if (numberOfGuesses < 6) winSound.play();
    else loseSound.play();
    sf::sleep(sf::Time(sf::seconds(3.0)));
    //delete winLoseSound;
}

void Sounds::playSound(SoundType type)
{
    switch (type)
    {
    case letter:
        letterSound.setVolume(30.f);
        letterSound.play();
        break;
    case enter:
        enterSound.setVolume(30.f);
        enterSound.play();
        break;
    case doh:
        dohSound.play();
        break;
    default:
        ;
    }
}
