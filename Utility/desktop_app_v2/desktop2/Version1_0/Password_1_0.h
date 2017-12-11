#ifndef PASSWORD_1_0_H
#define PASSWORD_1_0_H

#include "AbstractPassword.h"

#define p_addr_sz (1)

class Password_1_0 : public AbstractPassword
{
public:
    Password_1_0();

    static int size();
    bool readFrom(QByteArray& deviceData) override;
    QByteArray toByteArray() override;

};

#endif // PASSWORD_1_0_H
