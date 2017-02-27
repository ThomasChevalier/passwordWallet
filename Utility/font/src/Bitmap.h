#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include <deque>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cassert>
#include <memory>

#include "Pixel.h"
#include "FileHeader.h"
#include "ImageHeader.h"
#include "Matrix.h"

static_assert(sizeof(char) == 1 && sizeof(int) == 4 && sizeof(short) == 2, "Erreur, taille imcomptatible");

template<typename T>
T readData(std::ifstream& file, unsigned int bytes)
{
    if(sizeof(char) * bytes != sizeof(T))
        throw std::runtime_error("Cann't read data because sizeof(char) * nbrBytes != sizeof(T)");
    char *buffer = new char[bytes];
    file.read(buffer, bytes);
    T result = *(reinterpret_cast<T*>(buffer));
    delete[] buffer;
    return result;
}

template<typename T>
void writeData(std::ofstream& file, T data)
{
    file.write(reinterpret_cast<char*>(&data), sizeof(data));
}

class Bitmap
{
public:
    Bitmap();
    Bitmap(const std::string &fileName);
    Bitmap(unsigned int width, unsigned int height);

    ~Bitmap();

    void open();
    void open(const std::string &fileName);

    void close();

    void save();

    Pixel& getPixel(unsigned int x, unsigned int y) const;
    void setPixel(unsigned int x, unsigned int y, Pixel p);
    void setFileName(const std::string& fileName)
    {
        m_fileName = fileName;
    }

    unsigned int getWidth() const
    {
        return m_width;
    }
    unsigned int getHeight() const
    {
        return m_height;
    }
    bool isOpen() const
    {
        return m_isOpen;
    }

protected:

    // Read function
    void readFileHeader(std::ifstream& file);
    void readImageHeader(std::ifstream& file);
    void readPixelData(std::ifstream& file);

    // Write function
    void writeFileHeader(std::ofstream& file) const;
    void writeImageHeader(std::ofstream& file) const;
    void writePixelData(std::ofstream& file) const;

    // Data set function
    void setHeader();

    // Others
    int getPitch() const;

    // Image data

    std::shared_ptr< Matrix<Pixel> > m_data;
    unsigned int m_height;
    unsigned int m_width;
    std::string m_fileName;

    // Info
    bool m_isOpen;

    // Header
    FileHeader m_fileHeader;
    ImageHeader m_imageHeader;
};

#endif // BITMAP_H
