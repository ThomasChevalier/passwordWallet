#include "SerialCommand.h"

#include <limits>
#include <stdexcept>

SerialCommand::SerialCommand(Type type, const QByteArray& data):
    m_type(type), m_data(data), m_state(WaitId),
    m_valid(false)
{
    if(m_type != None && data.size() <= std::numeric_limits<quint16>::max())
    {
        m_valid = true;
    }
}

bool SerialCommand::readFrom(QByteArray &deviceData)
{
    static quint16 tempSize = 0;

    if(!deviceData.isEmpty() && m_state == WaitId){
        const quint8 rawId = deviceData.at(0);
        deviceData.remove(0, 1);
        if(rawId >= None){
            throw std::out_of_range("Command id out of range.");
        }
        m_type = static_cast<Type>(rawId);

        m_state = WaitSize1;
    }

    if(!deviceData.isEmpty() && m_state == WaitSize1)
    {
        tempSize = deviceData.at(0);
        deviceData.remove(0, 1);

        m_state = WaitSize2;
    }

    if(!deviceData.isEmpty() && m_state == WaitSize2)
    {
        tempSize |= deviceData.at(0) << 8;
        deviceData.remove(0, 1);
        m_data.clear();
        m_state = WaitData;
    }

    if(!deviceData.isEmpty() && m_state == WaitData){
        if(deviceData.size() >= tempSize){
            m_data.append(deviceData.left(tempSize));
            deviceData.remove(0, tempSize);
            tempSize = 0;
        }
        else
        {
            m_data.append(deviceData);
            deviceData.clear();
            tempSize -= deviceData.size();
        }

        if(tempSize == 0){
            m_state = WaitId;
            m_valid = true;
            return true;
        }
    }

    m_valid = false;
    return false;
}

bool SerialCommand::isValid() const
{
    return m_valid;
}

QByteArray SerialCommand::toByteArray() const
{
    if(m_data.size() > std::numeric_limits<quint16>::max())
    {
        throw std::length_error("Too much data in the command.");
    }

    QByteArray converted;
    converted.resize(1 + 2 + m_data.size());
    converted[0] = static_cast<quint8>(m_type);
    converted[1] = static_cast<quint8>(m_data.size() & 0xFF);
    converted[2] = static_cast<quint8>(m_data.size() >> 8);

    for(int i(0); i < m_data.size(); ++i){
        converted[2+i] = m_data[i];
    }

    return converted;
}

SerialCommand::Type SerialCommand::type() const
{
    return m_type;
}

QByteArray SerialCommand::data() const
{
    return m_data;
}
