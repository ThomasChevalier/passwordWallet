#ifndef ABSTRACTOPTION_H
#define ABSTRACTOPTION_H

#include <QByteArray>

class AbstractOption
{
public:
    AbstractOption();

    virtual int size() = 0;
    virtual QByteArray toByteArray() = 0;
    virtual bool readFrom(QByteArray& deviceData) = 0;
    virtual bool isValid() const = 0;
};

#endif // ABSTRACTOPTION_H
