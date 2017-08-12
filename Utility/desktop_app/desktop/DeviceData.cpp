#include "DeviceData.h"

static quint16  OFFSET_OPTIONS_FLAG = 0;
static quint16  SIZE_OPTION_FLAG = 1;
static quint16  OPTIONS_FLAG_OFFSET_SORTING_METHOD_L =0;
static quint16  OPTIONS_FLAG_OFFSET_SORTING_METHOD_H =1;
static quint16  OPTIONS_FLAG_OFFSET_ORIENTATION =2;
static quint16  OPTIONS_FLAG_OFFSET_INITIALIZED =3;
static quint16  OPTIONS_FLAG_OFFSET_QWERTY =4;

static quint16  OFFSET_FIRST_PWD_USE =OFFSET_OPTIONS_FLAG + SIZE_OPTION_FLAG; /* 1 */
static quint16  SIZE_FIRST_PWD_USE =1;

static quint16  OFFSET_FIRST_PWD_ALPHA =OFFSET_FIRST_PWD_USE + SIZE_FIRST_PWD_USE; /* 2 */
static quint16  SIZE_FIRST_PWD_ALPHA =1;

static quint16  OFFSET_NUM_PWD =OFFSET_FIRST_PWD_ALPHA + SIZE_FIRST_PWD_ALPHA; /* 3 */
static quint16  SIZE_NUM_PWD =1;

FRAM_Id::FRAM_Id():
    i2c(){}

DeviceData::DeviceData():
    m_storeScreenBufferInFram(false)
{

}

FRAM_Id DeviceData::framId() const
{
    return m_framId;
}

QByteArray DeviceData::key() const
{
    return m_key;
}

QByteArray DeviceData::memory() const
{
    return m_memory;
}

quint32 DeviceData::memorySize() const
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

DeviceData::SortingAlgo DeviceData::sorting() const
{
    switch ((m_memory[OFFSET_OPTIONS_FLAG] & ((1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_L)|(1<<OPTIONS_FLAG_OFFSET_SORTING_METHOD_H))) >> OPTIONS_FLAG_OFFSET_SORTING_METHOD_L) {
    case 0: return None;  break;
    case 1: return Use;   break;
    case 2: return Alpha; break;
    default: return None; break;
    }
}

DeviceData::Orientation DeviceData::orientation() const
{
    if(m_memory[OFFSET_OPTIONS_FLAG] & (1 << OPTIONS_FLAG_OFFSET_ORIENTATION))
    {
        return Inverse;
    }
    else
    {
        return Normal;
    }
}

bool DeviceData::initialized() const
{
    return m_memory[OFFSET_OPTIONS_FLAG] & (1 << OPTIONS_FLAG_OFFSET_INITIALIZED);
}

bool DeviceData::qwerty() const
{
    return m_memory[OFFSET_OPTIONS_FLAG] & (1 << OPTIONS_FLAG_OFFSET_QWERTY);
}

quint8 DeviceData::firstPwdUse() const
{
    return m_memory[OFFSET_FIRST_PWD_USE];
}

quint8 DeviceData::firstPwdAlpha() const
{
    return m_memory[OFFSET_FIRST_PWD_ALPHA];
}

quint8 DeviceData::passwordNum() const
{
    return m_memory[OFFSET_NUM_PWD];
}

QByteArray DeviceData::memoryMap() const
{
    quint16 offset = 260 + (storeScreenBufferInFram() ? 1024 : 0);
    quint16 size = 0;
    switch (memorySize()) {
    case 8192:  size=6;  break;
    case 32768: size=25; break;
    default:    size=0;  break;
    }
    return m_memory.mid(offset, size);
}

QList<Password> DeviceData::allPassword() const
{
    QList<Password> all;
    QByteArray map = memoryMap();
    for(int i = 0; i < map.size(); ++i)
    {
        quint8 byte = map[i];
        for(int j = 0; j < 8; ++j)
        {
            if(byte & (1 << j))
            {
                all << password(i*8+j);
            }
        }
    }
    return all;
}

Password DeviceData::password(quint8 id) const
{
    if(id >= maxNumPwd())
    {
        return Password(0, QByteArray());
    }
    else
    {
        return Password(id, m_memory.mid(firstPwdOffset()+id*166, 166));
    }
}

quint8 DeviceData::maxNumPwd() const
{
    return (memorySize() - firstPwdOffset()) / 166;
}

void DeviceData::addPassword(QString name, QString pwd, QString usrName) const
{

}

void DeviceData::setMemory(const QByteArray &data)
{
    m_memory = data;
}

void DeviceData::setKey(const QByteArray &key)
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

quint16 DeviceData::firstPwdOffset() const
{
    quint16 firstPwd = OFFSET_NUM_PWD+SIZE_NUM_PWD;
    firstPwd+=256; // Entropy pool (256 bytes)
    if(storeScreenBufferInFram())
    {
        firstPwd+=1024;
    }

    switch (memorySize()) {
    case 8192:  firstPwd+=6;  break;
    case 32768: firstPwd+=25; break;
    default: break;
    }
    return firstPwd;
}

// PRIVATE
bool DeviceData::storeScreenBufferInFram() const
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
