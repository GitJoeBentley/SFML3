#include "SoundEffect.h"
#include "constants.h"
#include <iostream>

SoundEffect::SoundEffect()
{
    std::string filenames[9] = {"PaddleHitBall","PaddleHitWall","BallHitWall","BallHitTile","EndOfGame",
    "PaddleMissBall0","PaddleMissBall1","PaddleMissBall2","PaddleMissBall3",};
    for (auto i = 0; i < 9; i++)
    {
        std::string filename = ResourcePath + filenames[i] + ".wav";
        if (!soundBuffer[i].loadFromFile(filename))
            std::cerr << "Unable to open file: " << filename << std::endl;
        else sound[i] = new sf::Sound(soundBuffer[i]);
    }

}

SoundEffect::~SoundEffect()
{
    for (auto i = 0; i < 9; i++)
    {
        delete sound[i];
        sound[i] = nullptr;
    }
}
