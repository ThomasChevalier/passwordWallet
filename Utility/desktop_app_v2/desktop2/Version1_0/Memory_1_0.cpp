#include "Memory_1_0.h"

Memory_1_0::Memory_1_0():
    AbstractMemory()
{

}

QList<QPair<int, QByteArray> > Memory_1_0::diff(const AbstractMemory &old)
{

}

int Memory_1_0::size()
{

}

QByteArray Memory_1_0::toByteArray()
{

}

bool Memory_1_0::readFrom(QByteArray &deviceData)
{

}

bool Memory_1_0::isValid() const
{

}

void Memory_1_0::addPassword(std::unique_ptr<AbstractPassword> pwd)
{

}

QList<AbstractPassword *> Memory_1_0::allPasswords(int sortingMethod)
{

}

void Memory_1_0::setPassword(int position, std::unique_ptr<AbstractPassword> pwd)
{

}

void Memory_1_0::setOption(std::unique_ptr<AbstractOption> opt)
{

}

std::unique_ptr<AbstractOption> Memory_1_0::getOption() const
{

}

void Memory_1_0::setKey(QByteArray key)
{

}

QByteArray Memory_1_0::getKey() const
{

}
