#include "SerialInterface_1_0.h"

#include <stdexcept>

SerialInterface_1_0::SerialInterface_1_0():
    SerialAbstractInterface()
{

}

void SerialInterface_1_0::setPassword(int pwdIndex, std::unique_ptr<AbstractPassword> pwd)
{
    Password_1_0 *pwd1_0 = dynamic_cast<Password_1_0*>(pwd.release());
    if(pwd1_0 == nullptr){
        throw std::invalid_argument("Unable to cast given pointer to Password_1_0*");
    }

}

std::unique_ptr<AbstractPassword> SerialInterface_1_0::getPassword(int pwdIndex) const
{

}

void SerialInterface_1_0::setOption(std::unique_ptr<AbstractOption> opt)
{

}

std::unique_ptr<AbstractOption> SerialInterface_1_0::getOption() const
{

}

void SerialInterface_1_0::setMemory(QByteArray mem)
{

}

QByteArray SerialInterface_1_0::getMemory() const
{

}

void SerialInterface_1_0::setKey(QByteArray key)
{

}

QByteArray SerialInterface_1_0::getKey() const
{

}
