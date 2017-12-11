#ifndef ABSTRACTMEMORY_H
#define ABSTRACTMEMORY_H

#include <memory>

#include <QList>
#include <QByteArray>

#include "AbstractPassword.h"
#include "AbstractOption.h"

class AbstractMemory
{
public:
    AbstractMemory();

    /**
     * @brief diff Calculates a list of bytes with the chunk to updates
     * If the update is not worth it returns the complete data at position 0
     * @param old The memory to compare to
     * @return Returns a list of chunk with the position of the first byte and the data
     */
    virtual QList<QPair<int, QByteArray>> diff(const AbstractMemory& old) = 0;

    virtual int size() = 0;
    virtual QByteArray toByteArray() = 0;
    virtual bool readFrom(QByteArray& deviceData) = 0;
    virtual bool isValid() const = 0;

    virtual void addPassword(std::unique_ptr<AbstractPassword> pwd);
    virtual QList<AbstractPassword*> allPasswords(int sortingMethod);
    virtual void setPassword(int position, std::unique_ptr<AbstractPassword> pwd);

    virtual void setOption(std::unique_ptr<AbstractOption> opt) = 0;
    virtual std::unique_ptr<AbstractOption> getOption() const = 0;

    virtual void setKey(QByteArray key) = 0;
    virtual QByteArray getKey() const = 0;

};

#endif // ABSTRACTMEMORY_H
