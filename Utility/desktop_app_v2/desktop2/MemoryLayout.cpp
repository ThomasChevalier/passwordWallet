#include "MemoryLayout.h"

#include <QDebug>

#include <QFile>
#include <QXmlStreamReader>

MemoryLayout::MemoryLayout()
{
    loadFromFile("../../../Code/memorg.xml");
}

bool MemoryLayout::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd()){
        if(xml.name() == "root")
        {

        }
        xml.readNext();
    }
    return true;
}

MemoryLayout &MemoryLayout::get()
{
    static MemoryLayout instance;
    return instance;
}

int MemoryLayout::operator[](const QString &field)
{
    return at(field);
}

int MemoryLayout::at(const QString &field)
{
    auto it = m_values.find(field);
    if(it == m_values.end()){
        return -1;
    }
    else{
        return it->first;
    }
}

int MemoryLayout::size(const QString &field)
{
    auto it = m_values.find(field);
    if(it == m_values.end()){
        return -1;
    }
    else{
        return it->second;
    }
}
