#ifndef PIXEL_H
#define PIXEL_H

#include <ostream>
#include "Color.h"

class Pixel
{
    public:
        Pixel():
            m_color(Color::White)
        {}

        Pixel(Color colorValue):
            m_color(colorValue)
        {}

        Pixel(unsigned char rValue, unsigned char gValue, unsigned char bValue):
            m_color(rValue, gValue, bValue)
        {}

        Color color() const { return m_color; }

        void setColor(Color colorValue) { m_color = colorValue; }
        void setColor(unsigned char rValue, unsigned char gValue, unsigned char bValue) { m_color = Color(rValue, gValue, bValue); }

        void display(std::ostream& flux) const
        {
            flux << "rgb(" << static_cast<int>(m_color.r()) << "; " << static_cast<int>(m_color.g()) << "; " << static_cast<int>(m_color.b()) << ") ";
        }

    protected:
        Color m_color;
};

std::ostream& operator<<( std::ostream &flux, Pixel const& pixel);


#endif // PIXEL_H
