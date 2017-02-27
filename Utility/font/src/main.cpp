#include <iostream>
#include <iomanip>
#include <sstream>
#include "Bitmap.h"
#include "Pixel.h"
#include "FileHeader.h"
#include "ImageHeader.h"

int main(int argc, char *argv[])
{
    if(argc != 2 || (argc == 2  && argv[1][0] == '-'
                                && argv[1][1] == '-'
                                && argv[1][2] == 'h'
                                && argv[1][3] == 'e'
                                && argv[1][4] == 'l'
                                && argv[1][5] == 'p'))
    {
        std::cout << "bitmap_to_font by Thomas Chevalier.\n(c) 2017 Thomas Chevalier - All rights reserved.\nthomasom [dot] chevalier [at] gmail [dot] com\nUsage : bitmap_to_font [fileName]\n\tfileName = The file name of the image of the font in the bmp format." << std::endl;
        return 0;
    }
    Bitmap img(argv[1]);
    std::ostringstream oss;

    const unsigned int width = img.getWidth();
    const unsigned int height = img.getHeight();
    char c = ' ';
    unsigned int byte_counter = 0;
    for(unsigned int x(0); x < width; ++x)
    {
        ++byte_counter;

        unsigned char byte = 0;
        for(unsigned int y(0); y < height; ++y)
        {
            Pixel p = img.getPixel(x ,y);
            if(p.color().equal(Color(Color::Black)))
            {
                byte |= (1<<y);
            }

        }
        if(byte <= 0x0F)
        {
            oss << "0x" << std::hex << "0" << std::uppercase << static_cast<int>(byte) << ", ";
        }
        else
        {
            oss << "0x" << std::hex << std::uppercase << static_cast<int>(byte) << ", ";
        }


        if(byte_counter == 5)
        {
            oss << "/* " << c << " */"<< std::endl;
            ++c;
            byte_counter = 0;
            x+=5;
        }

    }

    std::cout << oss.str();

    return 0;
}
