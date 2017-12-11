#include "Option_1_0.h"

#include "Password_1_0.h"

Option_1_0::Option_1_0():
    m_valid(false),
    m_optFlags(0),
    m_firstPwdUse(0),
    m_firstPwdAlpha(0),
    m_numPwd(0),
    m_entropyPool(),
    m_memoryMap(),
    m_backupSection()
{

}

int Option_1_0::size()
{
    return 1+3*p_addr_sz+256+25+2+Password_1_0::size();
}

QByteArray Option_1_0::toByteArray()
{
    QByteArray options;
    options.append(m_optFlags);
    options.append(m_firstPwdUse);
    options.append(m_firstPwdAlpha);
    options.append(m_numPwd);
    options.append(m_entropyPool);
    options.append(m_memoryMap);
    options.append(m_backupSection);
    return options;
}

static char pick(QByteArray& array)
{
    char c = array.at(0);
    array.remove(0, 1);
    return c;
}
bool Option_1_0::readFrom(QByteArray& deviceData)
{
    if(deviceData.size() != size()){
        return false;
    }
    m_valid = false;

    m_optFlags = pick(deviceData);

#if p_addr_sz == 1
    m_firstPwdUse   = pick(deviceData);
    m_firstPwdAlpha = pick(deviceData);
    m_numPwd       = pick(deviceData);
#elif p_addr_sz == 2
    m_firstPwdUse   = pick(deviceData) | (pick(deviceData) << 8);
    m_firstPwdAlpha = pick(deviceData) | (pick(deviceData) << 8);
    m_numPwd        = pick(deviceData) | (pick(deviceData) << 8);
#else
#error Unsupported size for passwords addresses
#endif

    m_entropyPool = deviceData.left(256);
    m_memoryMap = deviceData.left(25);
    m_backupSection = deviceData.left(2+Password_1_0::size());

    m_valid = true;
    return true;
}

bool Option_1_0::isValid() const
{
    return m_valid;
}
