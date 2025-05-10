#include "Sound.h"
#include "constants.h"

Sound::Sound()
{
    addSound("Invaders", InvadersSoundFile);
    addSound("Bullet", BulletSoundFile);
    addSound("Explosion", ExplosionSoundFile, 80.f);
    addSound("Saucer", SaucerSoundFile,10.f);
    addSound("Bomb", BombSoundFile, 10.f);
    addSound("BombExplosion", BombExplosionSoundFile);
    addSound("Applause", ApplauseSoundFile);
}

Sound::~Sound()
{
    for (std::map<std::string, sf::Sound*>::iterator it = sound.begin(); it != sound.end(); ++it)
    {
        delete it->second;
        it->second = nullptr;
    }
}

void Sound::addSound(const std::string& name, const std::string& soundfile, float volume)
{
    sf::SoundBuffer* buffer = new sf::SoundBuffer(soundfile);
    sound[name] = new sf::Sound(*buffer);
    sound[name]->setVolume(volume);
}

void Sound::start(const std::string& name)
{
    sf::SoundSource::Status status = sound[name]->getStatus();
    if (status != sf::SoundSource::Status::Playing) sound[name]->play();
}

void Sound::stop(const std::string& name)
{
     sf::SoundSource::Status status = sound[name]->getStatus();
    if (status != sf::SoundSource::Status::Stopped) sound[name]->stop();
}
