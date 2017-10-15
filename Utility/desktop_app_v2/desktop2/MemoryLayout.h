#ifndef MEMORYLAYOUT_H
#define MEMORYLAYOUT_H

#include <QString>
#include <QMap>
#include <QPair>

class MemoryLayout
{
public:
    MemoryLayout();

    bool loadFromFile(const QString& fileName);

    static MemoryLayout &get();

    void setVersion();

    int operator[](const QString& field);

    int at(const QString& field);
    int size(const QString& field);

private:
    // Associate name of the field with <position, size>
    QMap<QString, QPair<unsigned, unsigned> > m_values;
};

#endif // MEMORYLAYOUT_H
