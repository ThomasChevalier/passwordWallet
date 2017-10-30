#ifndef SERIALINTERFACE_1_0_H
#define SERIALINTERFACE_1_0_H

#include <memory>

#include "SerialAbstractInterface.h"

#include "Password_1_0.h"

class SerialInterface_1_0 : public SerialAbstractInterface
{
public:
    SerialInterface_1_0();

    void setPassword(int pwdIndex, std::unique_ptr<AbstractPassword> pwd) override;
    std::unique_ptr<AbstractPassword> getPassword(int pwdIndex) const override;

    void setOption(std::unique_ptr<AbstractOption> opt) override;
    std::unique_ptr<AbstractOption> getOption() const override;

    void setMemory(QByteArray mem) override;
    QByteArray getMemory() const override;

    void setKey(QByteArray key) override;
    QByteArray getKey() const override;
};

#endif // SERIALINTERFACE_1_0_H
