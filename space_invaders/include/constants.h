#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>

const unsigned MainWindowWidth = 1024;
const unsigned MainWindowHeight = 800;
const int numGuns = 3;
const float SwitchTime = .2f;
const float BulletRadius = 2.0f;
const float BulletSpeed = 0.1f;
const float BombSpeed = 0.0225f;
const float SaucerSpeed = 0.01f;
const float InvaderSpeed = 3.0f;
const float GunSpeed = 15.f;

const int BombFrequency = 32767;
const int SaucerFrequency = 32767;
const sf::Vector2f InvaderSize{32.0f,32.0f};
const sf::Vector2u ShieldSize(64u, 45u);
const unsigned ShieldHeight = 45u;;
const sf::Vector2f BombSize{12.0f,24.0f};

const float ShieldYPosition = 0.9f * MainWindowHeight;
const sf::Color GreenPixel(0,255,0,255);
const sf::Color TransparentPixel(0,0,0,255);

const std::string ResourcePath = ".\\resources\\";

// Image files
const std::string BackgroundFile = ResourcePath + "background.png";
const std::string GunImageFile = ResourcePath + "gun.png";
const std::string InvaderImageFile = ResourcePath + "invaders2.png";
const std::string BombImageFile = ResourcePath + "bombs.png";
const std::string SaucerImageFile = ResourcePath + "saucer.png";
const std::string ExplosionImageFile = ResourcePath + "explosion.png";
const std::string ShieldDataFile {ResourcePath + "shield_data.txt"};
const std::string HighScoresFile{ResourcePath + "highscores.bin"};

// Sound files
const std::string InvadersSoundFile = ResourcePath + "frogs.wav";
const std::string BulletSoundFile = ResourcePath + "bullet.wav";
const std::string ExplosionSoundFile = ResourcePath + "explosion.wav";
const std::string SaucerSoundFile = ResourcePath + "saucer.wav";
const std::string BombSoundFile = ResourcePath + "bomb.wav";
const std::string BombExplosionSoundFile = ResourcePath + "bomb_explosion.wav";
const std::string ApplauseSoundFile = ResourcePath + "applause.wav";
const std::string OpeningMusicFile = ResourcePath + "Also_Sprach_Zarathustra.ogg";

// Font files
const std::string ArielFontFile = ResourcePath + "arial.ttf";
const std::string CourierNewFontFile = ResourcePath + "courbd.ttf";

// Text Files
const std::string StartFile = ResourcePath + "start.txt";

#endif // CONSTANTS_H
