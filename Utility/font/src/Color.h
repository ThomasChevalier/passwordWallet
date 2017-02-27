#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

class Color
{
public:
    enum  ColorValue{
        Red,
        Green,
        Blue,
        White,
        Black,
    };

    Color(unsigned char rValue, unsigned char gValue, unsigned char bValue):
        m_r(rValue), m_g(gValue), m_b(bValue)
    {}

    Color(ColorValue value):
	m_r(0), m_g(0), m_b(0)
    {
        setPredefined(value);
    }

    unsigned char r() const { return m_r; }
    unsigned char g() const { return m_g; }
    unsigned char b() const { return m_b; }

    void setR( unsigned char value ) { m_r = value; }
    void setG( unsigned char value ) { m_g = value; }
    void setB( unsigned char value ) { m_b = value; }

    void setPredefined( ColorValue value)
    {
        switch(value)
        {
        case Red:
            m_r = 255; m_g = 0; m_b = 0;
            break;
        case Green:
            m_r = 0; m_g = 255; m_b = 0;
            break;
        case Blue:
            m_r = 0; m_g = 0; m_b = 255;
            break;
        case White:
            m_r = 255; m_g = 255; m_b = 255;
            break;
        case Black:
            m_r = 0; m_g = 0; m_b = 0;
            break;
        default:
            break;
        }
    }

    bool equal(const Color& c)
    {
        return m_r==c.m_r && m_g==c.m_g && m_b==c.m_b;
    }

private:
    unsigned char m_r;
    unsigned char m_g;
    unsigned char m_b;
};

#endif // COLOR_H_INCLUDED
