#ifndef MEMORY_1_0_H
#define MEMORY_1_0_H

#include "AbstractMemory.h"

class Memory_1_0 : public AbstractMemory
{
public:
    Memory_1_0();

    QList<QPair<int, QByteArray>> diff(const AbstractMemory& old) override;

    int size() override;
    QByteArray toByteArray() override;
    bool readFrom(QByteArray& deviceData) override;
    bool isValid() const override;

    void addPassword(std::unique_ptr<AbstractPassword> pwd) override;
    QList<AbstractPassword *> allPasswords(int sortingMethod) override;
    void setPassword(int position, std::unique_ptr<AbstractPassword> pwd) override;

    void setOption(std::unique_ptr<AbstractOption> opt) override;
    std::unique_ptr<AbstractOption> getOption() const override;

    void setKey(QByteArray key) override;
    QByteArray getKey() const override;

};

#endif // MEMORY_1_0_H
