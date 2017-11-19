#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept>

template <typename Type>
class Matrix
{
public:
        Matrix(unsigned int X, unsigned int Y, Type defaultValue):
        m_row(Y), m_column(X), m_data(m_row * m_column, defaultValue)
    {

    }

    Type& at(unsigned int x, unsigned int y) // y -> row
    {
        if(x * y >= m_data.size() ||
           y >= m_row || x >= m_column){
            throw std::out_of_range("Index out of range.");
        }
        return m_data.at(x+y*m_column);
    }

    unsigned int getRow() const
    {
        return m_row;
    }

    unsigned int getColumn() const
    {
        return m_column;
    }

private:
    const unsigned int m_row;
    const unsigned int m_column;
    // A deque instead of a vector because the deque can store more data
    std::deque<Type> m_data;
};


#endif // MATRIX_H
