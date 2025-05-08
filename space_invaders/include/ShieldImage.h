#ifndef SHIELDIMAGE_H
#define SHIELDIMAGE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace std;

class ShieldImage : public sf::Image
{
    public:
        ShieldImage();
        virtual ~ShieldImage() = default;
        uint64_t* getshieldData() { return shieldData; }

    private:
        uint64_t* shieldData;
};

#endif // SHIELDIMAGE_H
