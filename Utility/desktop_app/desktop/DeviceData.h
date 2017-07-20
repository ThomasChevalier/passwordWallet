#ifndef DEVICEDATA_H
#define DEVICEDATA_H

#include <QtGlobal>
#include <QByteArray>
#include <QList>

#include "Password.h"

struct FRAM_I2C_Id
{
    FRAM_I2C_Id(): manufacturer_id(0), product_id(0) {}

    quint16 manufacturer_id;
    quint16 product_id;
};

struct FRAM_SPI_Id
{
    FRAM_SPI_Id(): continuation_code(0), manufacturer_id(0),
        product_idL(0), product_idH(0) {}

    quint8 continuation_code;	// Continuation code : should be 0x7F (2)
    quint8 manufacturer_id;	// Manufacturer ID : should be 0x04 for Fujistu (1)
    quint8 product_idL;		// First byte of product ID : the bit 0-4 are the density code, bit 5-7 are for proprietary use (3)
    // Density code : 0x01 is 16 kbit / 0x03 is 64 kbit / 0x04 is 128 kbit / 0x05 is 256kbit / 0x06 is 512kbit / 0x07 is 1Mbit / 0x08 is 2Mbit / 0x09 is 4 Mbit
    quint8 product_idH;		// Second byte of product ID : proprietary use only (4)
};

struct FRAM_Id
{
    FRAM_Id();
    union
    {
        FRAM_I2C_Id i2c;
        FRAM_SPI_Id spi;
    };
};

class DeviceData
{
public:

    enum Fram_Connection
    {
        SPI,
        I2C,
        Unknow
    };

    enum SortingAlgo
    {
        None,
        Use,
        Alpha
    };

    enum Orientation
    {
        Normal,
        Inverse
    };


    DeviceData();

    FRAM_Id    framId()  const;
    QByteArray key()     const;
    QByteArray memory()  const;
    quint32 memorySize() const;

    bool encryption() const;
    SortingAlgo sorting() const;
    Orientation orientation() const;
    bool initialized() const;
    bool qwerty() const;
    quint8 firstPwdUse() const;
    quint8 firstPwdAlpha() const;
    quint8 passwordNum() const;
    QByteArray memoryMap() const;

    QList<Password> allPassword() const;
    Password password(quint8 id) const;
    quint8 maxNumPwd() const;

    void addPassword(QString name, QString pwd, QString usrName) const;

    void setMemory(const QByteArray& data);
    void setKey(const QByteArray& key);
    void setParameter(const QByteArray& parameter);

    static quint16 vendorIdentifier();
    static quint16 productIdentifier();


private:

    quint16 firstPwdOffset() const;

    bool storeScreenBufferInFram() const;
    Fram_Connection getFramConnection() const;

    QByteArray m_key;
    QByteArray m_memory;
    FRAM_Id m_framId;
    bool m_storeScreenBufferInFram;
};

#endif // DEVICEDATA_H
