#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H
#include <SFML/Audio.hpp>

extern const std::string ResourcesPath;

class SoundEffect
{
public:
    enum SoundType {PaddleHitBall,PaddleHitWall,BallHitWall,BallHitTile,EndOfGame,PaddleMissBall};
    SoundEffect();
    ~SoundEffect();

    sf::Sound& operator[](SoundType type)
    {
        return *(sound[type]);
    }

private:
    sf::SoundBuffer soundBuffer[9];
    sf::Sound* sound[9];
};

#endif // SOUNDEFFECT_H
