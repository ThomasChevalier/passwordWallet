#ifndef IMAGEHEADER_H_INCLUDED
#define IMAGEHEADER_H_INCLUDED

#pragma pack(1)

class ImageHeader
{
public:
    ImageHeader()
    {
        reset();
    }
    void reset()
    {
        sizeHeader = width = height = 0;
        nbPlane = bpp = 0;
        compression = sizeImg = 0;
        hRes = vRes = 0;
        cPalette = cIPalette = 0;
    }
    int sizeHeader;         // The size of the image header
	int width;              // The width of the image
	int height;             // The height of the image
	short int nbPlane;      // Always at 1
	short int bpp;          // number of bits used for write the color
	int compression;        // The compression method
	int sizeImg;            // Size total of the image
	int hRes;               // The hozizontal resolution
	int vRes;               // The vertical resolution
	int cPalette;           // The number of colors in the "palette"
	int cIPalette;          // The number of important colors in the "palette"
};

#endif // IMAGEHEADER_H_INCLUDED
