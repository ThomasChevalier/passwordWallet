#include "DeviceData.h"


FRAM_Id::FRAM_Id():
    i2c(){}

DeviceData::DeviceData():
    m_storeScreenBufferInFram(false)
{

}

FRAM_Id DeviceData::deviceFramId() const
{
    return m_framId;
}

QByteArray DeviceData::deviceKey() const
{
    return m_key;
}

QByteArray DeviceData::deviceFram() const
{
    return m_fram;
}

quint32 DeviceData::getDeviceFramSize() const
{
    Fram_Connection con = getFramConnection();
    quint8 density = 0;
    quint32 byteSize[] = {0, 2*1024, 0,  8*1024, 16*1024, 32*1024, 64*1024, 128*1024, 256*1024, 512*1024};
    if(con == SPI)
    {
        density = m_framId.spi.product_idL & 0x1F;
    }
    else if(con == I2C)
    {
        density = m_framId.i2c.product_id >> 8;
    }
    return byteSize[density];
}

bool DeviceData::getStoreScreenBufferInFram() const
{
    return m_storeScreenBufferInFram;
}

DeviceData::Fram_Connection DeviceData::getFramConnection() const
{
    if(m_framId.spi.manufacturer_id == 0x04 && m_framId.spi.continuation_code == 0x7F)
    {
        return SPI;
    }
    else if(m_framId.i2c.manufacturer_id == 0x00A)
    {
        return I2C;
    }
    else
    {
        return Unknow;
    }
}

void DeviceData::setDeviceFram(const QByteArray &data)
{
    m_fram = data;
}

void DeviceData::setDeviceKey(const QByteArray &key)
{
    m_key = key;
}

void DeviceData::setParameter(const QByteArray &parameter)
{
    m_framId.spi.manufacturer_id = parameter[0];
    m_framId.spi.continuation_code = parameter[1];
    m_framId.spi.product_idL = parameter[2];
    m_framId.spi.product_idH = parameter[3];

    m_storeScreenBufferInFram = parameter[4] != 0;
}

quint16 DeviceData::vendorIdentifier()
{
    return 0x03EB;
}

quint16 DeviceData::productIdentifier()
{
    return 0x2044;
}
