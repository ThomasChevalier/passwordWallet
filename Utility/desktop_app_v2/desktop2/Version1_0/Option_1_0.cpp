#include "Option_1_0.h"

Option_1_0::Option_1_0():
    m_state(StateRead::Flags),
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
    return toByteArray().size();
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

bool Option_1_0::readFrom(QByteArray& deviceData)
{
    if(m_state == StateRead::Done){
        return true;
    }
    if(m_state == StateRead::Flags && deviceData.size() >= 1)
    {
        m_optFlags = deviceData.at(0);
        deviceData.remove(0, 1);
        m_state = StateRead::PwdUse;
    }
    if(m_state == StateRead::PwdUse && deviceData.size() >= 1)
    {
        m_firstPwdUse = deviceData.at(0);
        deviceData.remove(0, 1);
        m_state = StateRead::PwdAlpha;
    }
    if(m_state == StateRead::PwdAlpha && deviceData.size() >= 1)
    {
        m_firstPwdAlpha = deviceData.at(0);
        deviceData.remove(0,1);
        m_state = StateRead::NumPwd;
    }
    if(m_state == StateRead::NumPwd && deviceData.size() >= 1)
    {
        m_numPwd = deviceData.at(0);
        deviceData.remove(0, 1);
        m_state = StateRead::EntropyPool;
    }
    if(m_state == StateRead::EntropyPool && deviceData.size() >= 256)
    {
        m_entropyPool = deviceData.mid(0, 256);
        deviceData.remove(0, 256);
        m_state = StateRead::MemoryMap;
    }
    if(m_state == StateRead::MemoryMap && deviceData.size() >= 25)
    {
        m_memoryMap = deviceData.mid(0, 25);
        deviceData.remove(0, 25);
        m_state = StateRead::BackupSection;
    }
    if(m_state == StateRead::BackupSection && deviceData.size() >= 168)
    {
        m_backupSection = deviceData.mid(0, 168);
        deviceData.remove(0, 168);
        m_valid = true;
        m_state = StateRead::Done;
    }
    return isValid();
}

bool Option_1_0::isValid() const
{
    return m_state == StateRead::Done;
}
