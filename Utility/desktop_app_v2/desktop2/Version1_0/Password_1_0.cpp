#include "Password_1_0.h"

#include <QtGlobal>

Password_1_0::Password_1_0():
    AbstractPassword()
{

}

int Password_1_0::size()
{
    return 4*p_addr_sz+2+32+16+32+16+64;
}

static char pick(QByteArray& array)
{
    char c = array.at(0);
    array.remove(0, 1);
    return c;
}

bool Password_1_0::readFrom(QByteArray &deviceData)
{
    if(deviceData.size() < size())
    {
        return false;
    }
#if p_addr_sz == 1
    m_prevUse   = pick(deviceData);
    m_nextUse   = pick(deviceData);
    m_prevAlpha = pick(deviceData);
    m_nextAlpha = pick(deviceData);
#elif p_addr_sz == 2
    m_prevUse   = pick(deviceData) | (pick(deviceData)<<8);
    m_nextUse   = pick(deviceData) | (pick(deviceData)<<8);
    m_prevAlpha = pick(deviceData) | (pick(deviceData)<<8);
    m_nextAlpha = pick(deviceData) | (pick(deviceData)<<8);
#else
#error Unsupported size for passwords addresses
#endif

    m_counter = pick(deviceData) | (pick(deviceData) << 8); // Not sure
    m_name = QString::fromLatin1(deviceData.left(32));
    deviceData.remove(0, 32);
    m_ivPwd = deviceData.left(16);
    deviceData.remove(0, 16);
    m_pwd = deviceData.left(32);
    deviceData.remove(0, 32);
    m_ivUsrName = deviceData.left(16);
    deviceData.remove(0, 16);
    m_userName = deviceData.left(64);
    deviceData.remove(0, 64);

    return true;
}

QByteArray Password_1_0::toByteArray()
{
    QByteArray pwdChunk;

    // Addresses
#if p_addr_sz == 1
     pwdChunk.append(m_prevUse & 0xFF);
     pwdChunk.append(m_nextUse & 0xFF);
     pwdChunk.append(m_prevAlpha & 0xFF);
     pwdChunk.append(m_nextAlpha & 0xFF);
#elif p_addr_sz == 2
    pwdChunk.append(m_prevUse & 0xFF);
    pwdChunk.append((m_prevUse>>8) & 0xFF);
    pwdChunk.append(m_nextUse & 0xFF);
    pwdChunk.append((m_prevUse>>8) & 0xFF);
    pwdChunk.append(m_prevAlpha & 0xFF);
    pwdChunk.append((m_prevAlpha>>8) & 0xFF);
    pwdChunk.append(m_nextAlpha & 0xFF);
    pwdChunk.append((m_nextAlpha>>8) & 0xFF);
#else
#error Unsupported size for passwords' addresses
#endif

    // Counter
    pwdChunk.append(m_counter & 0xFF);
    pwdChunk.append((m_counter>>8) & 0xFF);

    // Name
    QString validName = m_name;
    validName.resize(32, 0);
    pwdChunk.append(validName.toLatin1());

    // Iv pwd
    Q_ASSERT_X(m_ivPwd.size() == 16, "Password_1_0::toByteArray()", "Initial vector for password data is not 16 bytes");
    pwdChunk.append(m_ivPwd);

    Q_ASSERT_X(m_pwd.size() == 32, "Password_1_0::toByteArray()", "Password data is not 32 bytes");
    pwdChunk.append(m_pwd);

    // Iv usr
    Q_ASSERT_X(m_ivUsrName.size() == 16, "Password_1_0::toByteArray()", "Initial vector for username data is not 16 bytes");
    pwdChunk.append(m_ivUsrName);

    Q_ASSERT_X(m_userName.size() == 64, "Password_1_0::toByteArray()", "User name data is not 64 bytes");
    pwdChunk.append(m_userName);

    Q_ASSERT_X(pwdChunk.size() == size(), "Password_1_0::toByteArray()", "pwdChunk size and expected size mismatch");
    return pwdChunk;
}
