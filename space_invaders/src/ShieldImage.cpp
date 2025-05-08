#include "ShieldImage.h"
#include "constants.h"

ShieldImage::ShieldImage() : sf::Image(ShieldSize), shieldData(new uint64_t[ShieldSize.y])
{
    sf::Color color;
    unsigned pix;

    std::ifstream fin(ShieldDataFile, ios::in);

    if (!fin)
    {
        std::cerr << "Unable to open file " << ShieldDataFile << std::endl;
        std::exit(23);
    }
    for (unsigned row = 0; row < ShieldSize.y; row++)
    {
        for (unsigned col = 0; col < ShieldSize.x; col++)
        {
            fin >> pix;
            if (pix) color = sf::Color::Black;
            else color = sf::Color::Green;
            setPixel(sf::Vector2u(col,row), color);
        }
    }
    fin.close();
}

