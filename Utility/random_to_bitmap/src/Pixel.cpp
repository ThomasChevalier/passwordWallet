#include "Pixel.h"

std::ostream& operator<<( std::ostream &flux, Pixel const& pixel)
{
    pixel.display(flux);
    return flux;
}
