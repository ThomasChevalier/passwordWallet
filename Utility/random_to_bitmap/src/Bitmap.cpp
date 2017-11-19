#include "Bitmap.h"

Bitmap::Bitmap():
    m_data(), m_height(0), m_width(0), m_fileName(), m_isOpen(false),
     m_fileHeader(), m_imageHeader()
{}

Bitmap::Bitmap(const std::string &fileName):
     m_data(), m_height(0), m_width(0), m_fileName(fileName), m_isOpen(false),
      m_fileHeader(), m_imageHeader()
{
    open();
}

Bitmap::Bitmap(unsigned int width, unsigned int height):
    m_data(), m_height(height), m_width(width), m_fileName(), m_isOpen(false),
     m_fileHeader(), m_imageHeader()
{
    m_data = std::make_shared< Matrix<Pixel> >(m_width, m_height, Pixel());
}

Bitmap::~Bitmap()
{}

void Bitmap::open()
{
    if(m_isOpen)
        close();
    m_isOpen = true;

    if(m_fileName.empty()){
        std::cerr << "No file name specified." << std::endl;
        return;
    }
    std::ifstream imageFile(m_fileName, std::ios::binary);
    if(!imageFile){
            std::cerr << "Unable to open the file \"" << m_fileName << "\" in read mode." << std::endl;
            return;
    }
    if(imageFile)
        readFileHeader(imageFile);
    if(imageFile)
        readImageHeader(imageFile);
    if(imageFile)
        readPixelData(imageFile);
    imageFile.close();
}

void Bitmap::open(const std::string &fileName)
{
    m_fileName = fileName;
    open();
}

void Bitmap::close()
{
    if(m_data.use_count() != 1)
        throw std::runtime_error("Trop de pointeur en jeu :oo");
    m_data.reset();
    m_fileHeader.reset();
    m_imageHeader.reset();
    m_height = 0;
    m_width = 0;
    m_fileName = "";
    m_isOpen = false;

}

void Bitmap::save()
{
    if(m_fileName.empty()){
        std::cerr << "No file name specified." << std::endl;
        return;
    }
    std::ofstream imageFile(m_fileName);
    if(!imageFile){
        std::cerr << "Unable to open the file \"" << m_fileName << "\" in write mode." << std::endl;
        return;
    }
    setHeader();
    writeFileHeader(imageFile);
    writeImageHeader(imageFile);
    writePixelData(imageFile);
    imageFile.close();

}
Pixel& Bitmap::getPixel(unsigned int x, unsigned int y) const
{
    return m_data->at(x, y);
}

void Bitmap::setPixel(unsigned int x, unsigned int y, Pixel p)
{
    m_data->at(x, y) = p;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////    READ FUNCTION   ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Bitmap::readFileHeader(std::ifstream& file)
{
    //std::cout << "Start reading the file header ..." << std::endl;
    file.seekg(0);

    m_fileHeader.signature[0] = readData<char>(file, 1);
    m_fileHeader.signature[1] = readData<char>(file, 1);
    if(m_fileHeader.signature[0] != 'B' || m_fileHeader.signature[1] != 'M')
        throw std::runtime_error("The format is not supported.");
    m_fileHeader.fileSize = readData<int>(file, 4);
    m_fileHeader.rsv = readData<int>(file, 4);
    m_fileHeader.offsetImg = readData<int>(file, 4);

   // std::cout << "End of reading the file header." << std::endl;
}

void Bitmap::readImageHeader(std::ifstream& file)
{
    //std::cout << "Start reading the image header ..." << std::endl;
    m_imageHeader.sizeHeader  = readData<int>(file, 4);
    m_imageHeader.width = readData<int>(file, 4);
    m_imageHeader.height = readData<int>(file, 4);
    m_imageHeader.nbPlane = readData<short int>(file, 2);
    m_imageHeader.bpp = readData<short int>(file, 2);
    if(m_imageHeader.bpp!=24)
		throw std::runtime_error("Only the 24-bit format is supported by the software.");

    m_imageHeader.compression = readData<int>(file, 4);
   // if(m_imageHeader.compression != 0)
		//return */throw std::runtime_error("The compression is not supported by the software.");

    m_imageHeader.sizeImg = readData<int>(file, 4);
    m_imageHeader.hRes = readData<int>(file, 4);
    m_imageHeader.vRes = readData<int>(file, 4);
    m_imageHeader.cPalette = readData<int>(file, 4);
    m_imageHeader.cIPalette = readData<int>(file, 4);
   // if (m_imageHeader.cPalette!=0 || m_imageHeader.cIPalette!=0)
	//	throw std::runtime_error("The palette is not supported.\nOnly the 24-bit format is supported by the software.");

   // std::cout << "End of reading the image header." << std::endl;

    m_height = m_imageHeader.height;
    m_width = m_imageHeader.width;
    if(m_height >  1e6 && m_width > 1e6)
        throw std::runtime_error("Image too big (width or height > 1000000 pixels)");
    m_data = std::make_shared< Matrix<Pixel> >(m_width, m_height, Pixel());
}

void Bitmap::readPixelData(std::ifstream& file)
{
   //std::cout << "Start reading pixels ..." << std::endl;

    int pitch = getPitch();
    file.seekg(m_fileHeader.offsetImg);

    for(unsigned int height(0); height < m_height; ++height)
    {
        for(unsigned int width(0); width < m_width; ++width)
        {
            Pixel p;

            unsigned char blue;
            blue = readData<unsigned char>(file, 1);

            unsigned char green;
            green = readData<unsigned char>(file, 1);

            unsigned char red;
            red = readData<unsigned char>(file, 1);

            p.setColor(red, green, blue);
            setPixel(width, m_height - height - 1, p);
        }

        for(int i(0); i < pitch; ++i)
            readData<unsigned char>(file, 1);
    }
    //std::cout << "End pixel read." << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////   WRITE FUNCTION   ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Bitmap::writeFileHeader(std::ofstream& file) const
{
    writeData(file, m_fileHeader.signature[0]);
    writeData(file, m_fileHeader.signature[1]);
    writeData(file, m_fileHeader.fileSize);
    writeData(file, m_fileHeader.rsv);
    writeData(file, m_fileHeader.offsetImg);
}

void Bitmap::writeImageHeader(std::ofstream& file) const
{
    writeData(file, m_imageHeader.sizeHeader);
    writeData(file, m_imageHeader.width);
    writeData(file, m_imageHeader.height);
    writeData(file, m_imageHeader.nbPlane);
    writeData(file, m_imageHeader.bpp);
    writeData(file, m_imageHeader.compression);
    writeData(file, m_imageHeader.sizeImg);
    writeData(file, m_imageHeader.hRes);
    writeData(file, m_imageHeader.vRes);
    writeData(file, m_imageHeader.cPalette);
    writeData(file, m_imageHeader.cIPalette);
}

void Bitmap::writePixelData(std::ofstream& file) const
{
    Pixel p;
    unsigned char bgr[3];

    int pitch = getPitch();

    for(unsigned int height(0); height < m_height; ++height)
    {
        for(unsigned int width(0); width < m_width; ++width)
        {
            p = getPixel(width, m_height - height - 1);
            bgr[0] = p.color().b();
            bgr[1] = p.color().g();
            bgr[2] = p.color().r();
            for(int i(0); i < 3; i++)
                file.put(bgr[i]);
        }
        for(int i(0); i < pitch; ++i)
            file.put(0);
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////   DATA SET FUNCTION   /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Bitmap::setHeader()
{
    const unsigned short int fileHeaderSize = sizeof(m_fileHeader);
    const unsigned short int imgHeaderSize = sizeof(m_imageHeader);
    const unsigned short int headerSize = fileHeaderSize + imgHeaderSize;

    m_fileHeader.signature[0] = 'B';
    m_fileHeader.signature[1] = 'M';
    m_fileHeader.rsv = 0;
    m_fileHeader.offsetImg = headerSize;

    m_imageHeader.sizeHeader = imgHeaderSize;
    m_imageHeader.width = m_data->getRow();
    m_imageHeader.height = m_data->getColumn();
    m_imageHeader.nbPlane = 1;
    m_imageHeader.bpp = 24;
    m_imageHeader.compression = 0;
    m_imageHeader.sizeImg = m_imageHeader.width * m_imageHeader.height * sizeof(Pixel) + m_imageHeader.height * getPitch();

    m_fileHeader.fileSize = m_fileHeader.offsetImg + m_imageHeader.sizeImg;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////   OTHERS   ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int Bitmap::getPitch() const
{
    int tabPitch[4] {0, 3, 2, 1};
    int pitch = tabPitch[ (3*m_width)%4 ];
    return pitch;
}
