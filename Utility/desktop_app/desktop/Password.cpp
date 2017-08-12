#include "Password.h"

#include "Aes.h"
#include "RandomGenerator.h"

static quint8 PWD_OFFSET_PREV_PWD_USE = 0;
static quint8 PWD_SIZE_PREV_PWD_USE = 1;

static quint8 PWD_OFFSET_NEXT_PWD_USE = PWD_OFFSET_PREV_PWD_USE + PWD_SIZE_PREV_PWD_USE; /* 1 */
static quint8 PWD_SIZE_NEXT_PWD_USE = 1;

static quint8 PWD_OFFSET_PREV_PWD_ALPHA = PWD_OFFSET_NEXT_PWD_USE + PWD_SIZE_NEXT_PWD_USE; /* 2 */
static quint8 PWD_SIZE_PREV_PWD_ALPHA = 1;

static quint8 PWD_OFFSET_NEXT_PWD_ALPHA = PWD_OFFSET_PREV_PWD_ALPHA + PWD_SIZE_PREV_PWD_ALPHA; /* 3 */
static quint8 PWD_SIZE_NEXT_PWD_ALPHA = 1;

static quint8 PWD_OFFSET_PWD_COUNT = PWD_OFFSET_NEXT_PWD_ALPHA + PWD_SIZE_NEXT_PWD_ALPHA; /* 4 */
static quint8 PWD_SIZE_PWD_COUNT = 2;

static quint8 PWD_OFFSET_PWD_NAME = PWD_OFFSET_PWD_COUNT + PWD_SIZE_PWD_COUNT; /* 6 */
static quint8 PWD_SIZE_PWD_NAME = 32;

static quint8 PWD_OFFSET_PWD_IV = PWD_OFFSET_PWD_NAME + PWD_SIZE_PWD_NAME; /* 38 */
static quint8 PWD_SIZE_PWD_IV = 16;

static quint8 PWD_OFFSET_PWD_DATA = PWD_OFFSET_PWD_IV + PWD_SIZE_PWD_IV; /* 54 */
static quint8 PWD_SIZE_PWD_DATA = 32;

static quint8 PWD_OFFSET_USR_IV = PWD_OFFSET_PWD_DATA + PWD_SIZE_PWD_DATA; /* 86 */
static quint8 PWD_SIZE_USR_IV = 16;

static quint8 PWD_OFFSET_USR_NAME = PWD_OFFSET_USR_IV + PWD_SIZE_USR_IV; /* 102 */
static quint8 PWD_SIZE_USR_NAME = 64;

static quint8 SIZE_OF_PWD_BLOCK = PWD_OFFSET_USR_NAME + PWD_SIZE_USR_NAME; /* 166 */

Password::Password(quint8 id, QByteArray data):
    m_data(data), m_id(id)
{
    if(m_data.size()!=SIZE_OF_PWD_BLOCK)
    {
        m_data = QByteArray(SIZE_OF_PWD_BLOCK, 0);
    }
}

quint8 Password::prevPwdUse() const
{
    return m_data[PWD_OFFSET_PREV_PWD_USE];
}

quint8 Password::nextPwdUse() const
{
    return m_data[PWD_OFFSET_NEXT_PWD_USE];
}

quint8 Password::prevPwdAlpha() const
{
    return m_data[PWD_OFFSET_PREV_PWD_ALPHA];
}

quint8 Password::nextPwdAlpha() const
{
    return m_data[PWD_OFFSET_NEXT_PWD_ALPHA];
}

quint16 Password::count() const
{
    // Avr is little endian
    return (static_cast<quint16>(m_data[PWD_OFFSET_PWD_COUNT]) + (m_data[PWD_OFFSET_PWD_COUNT+1]<<8));
}

QString Password::name() const
{
    return QString::fromLocal8Bit(m_data.mid(PWD_OFFSET_PWD_NAME, 32));
}

QString Password::password(QByteArray key) const
{
   QByteArray iv = m_data.mid(PWD_OFFSET_PWD_IV, PWD_SIZE_PWD_IV);
   QByteArray data = m_data.mid(PWD_OFFSET_PWD_DATA, PWD_SIZE_PWD_DATA);
   uint8_t output[PWD_SIZE_PWD_DATA];

   AES128_CBC_decrypt_buffer(output, reinterpret_cast<uint8_t*>(data.data()),
                             PWD_SIZE_PWD_DATA,
                             reinterpret_cast<const uint8_t*>(key.constData()),
                             reinterpret_cast<const uint8_t*>(iv.constData()));
   // Remove padding
   uint8_t i = 0;
   uint8_t j = 0;
   for(; i < PWD_SIZE_PWD_DATA; ++i)
   {
       if(output[i] < 32)
           j=1;
       if(j)
           output[i] = 0;
   }
   return QString::fromLocal8Bit(reinterpret_cast<const char*>(output));
}

quint8 Password::id() const
{
    return m_id;
}

QByteArray Password::data() const
{
    return m_data;
}

QString Password::userName(QByteArray key) const
{
    QByteArray iv = m_data.mid(PWD_OFFSET_USR_IV, PWD_SIZE_USR_IV);
    QByteArray data = m_data.mid(PWD_OFFSET_USR_NAME, PWD_SIZE_USR_NAME);
    uint8_t output[PWD_SIZE_USR_NAME];
    AES128_CBC_decrypt_buffer(output, reinterpret_cast<uint8_t*>(data.data()),
                              PWD_SIZE_USR_NAME,
                              reinterpret_cast<const uint8_t*>(key.constData()),
                              reinterpret_cast<const uint8_t*>(iv.constData()));
    // Remove padding
    uint8_t i = 0;
    uint8_t j = 0;
    for(; i < PWD_SIZE_USR_NAME; ++i)
    {
        if(output[i] < 32)
            j=1;
        if(j)
            output[i] = 0;
    }
    return QString::fromLocal8Bit(reinterpret_cast<const char*>(output));
}

void Password::setCount(quint16 c)
{
    m_data[PWD_OFFSET_PWD_COUNT] = c >> 8;
    m_data[PWD_OFFSET_PWD_COUNT+1] = c & 0xFF;
}

bool Password::setName(const QString &name)
{
    if(name.size()>PWD_SIZE_PWD_NAME||name.isEmpty())
    {
        return false;
    }
    for(int i(0); i < PWD_SIZE_PWD_NAME; ++i)
    {
        if(i<name.size())
        {
            m_data[PWD_OFFSET_PWD_NAME+i] = name[i].toLatin1();
        }
        else
        {
            m_data[PWD_OFFSET_PWD_NAME+i] = 0;
        }
    }
    return true;
}

bool Password::setPassword(QString pwd, QByteArray key)
{
    if(pwd.size()>PWD_SIZE_PWD_DATA||pwd.isEmpty())
    {
        return false;
    }

    if(pwd.size()<PWD_SIZE_PWD_DATA)
    {
        const uint8_t randByteLessThan32 = RandomGenerator::get()->byte() >> 3;
        pwd.append(static_cast<char>(randByteLessThan32));
        for(uint8_t i = pwd.size(); i < PWD_SIZE_PWD_DATA; ++i)
        {
            pwd.append(static_cast<char>(RandomGenerator::get()->byte()));
        }
    }
    QByteArray iv(16, 0);
    for(uint8_t i = 0; i < 16; ++i)
    {
        iv[i] = RandomGenerator::get()->byte();
        m_data[PWD_OFFSET_PWD_IV+i]=iv[i];
    }

    uint8_t out[PWD_SIZE_PWD_DATA];
    AES128_CBC_encrypt_buffer(out, reinterpret_cast<uint8_t*>(pwd.toLocal8Bit().data()), PWD_SIZE_PWD_DATA,
                              reinterpret_cast<const uint8_t*>(key.constData()),
                              reinterpret_cast<const uint8_t*>(iv.constData()));

    for(uint8_t i = 0; i < PWD_SIZE_PWD_DATA; ++i)
    {
        m_data[PWD_OFFSET_PWD_DATA+i] = out[i];
    }

    return true;
}

bool Password::setUserName(QString usr, QByteArray key)
{
    if(usr.size()>PWD_SIZE_USR_NAME||usr.isEmpty())
    {
        return false;
    }

    if(usr.size()<PWD_SIZE_USR_NAME)
    {
        const uint8_t randByteLessThan32 = RandomGenerator::get()->byte() >> 3;
        usr.append(static_cast<char>(randByteLessThan32));
        for(uint8_t i = usr.size(); i < PWD_SIZE_USR_NAME; ++i)
        {
            usr.append(static_cast<char>(RandomGenerator::get()->byte()));
        }
    }
    QByteArray iv(16, 0);
    for(uint8_t i = 0; i < 16; ++i)
    {
        iv[i] = RandomGenerator::get()->byte();
        m_data[PWD_OFFSET_USR_IV+i]=iv[i];
    }

    uint8_t out[PWD_SIZE_USR_NAME];
    AES128_CBC_encrypt_buffer(out, reinterpret_cast<uint8_t*>(usr.toLocal8Bit().data()), PWD_SIZE_USR_NAME,
                              reinterpret_cast<const uint8_t*>(key.constData()),
                              reinterpret_cast<const uint8_t*>(iv.constData()));

    for(uint8_t i = 0; i < PWD_SIZE_USR_NAME; ++i)
    {
        m_data[PWD_OFFSET_USR_NAME+i] = out[i];
    }

    return true;
}
