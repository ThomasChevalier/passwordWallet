#include "SerialCommand.h"

#include <limits>
#include <stdexcept>

#include <QDebug>

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
    static quint32 tempSize = 0;

    if(!deviceData.isEmpty() && m_state == WaitId){
        const quint8 rawId = deviceData.at(0);
        deviceData.remove(0, 1);
        if(rawId >= None){
            throw std::out_of_range(QObject::tr("Command id out of range : %1").arg(static_cast<int>(rawId)).toStdString());
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

        m_state = WaitSize3;
    }

    if(!deviceData.isEmpty() && m_state == WaitSize3)
    {
        tempSize |= deviceData.at(0) << 16;
        deviceData.remove(0, 1);
        m_data.clear();
        m_state = WaitData;
    }

    if(m_state == WaitData){
        if(deviceData.size() >= tempSize){
            m_data.append(deviceData.left(tempSize));
            deviceData.remove(0, tempSize);
            tempSize = 0;
        }
        else
        {
            m_data.append(deviceData);
            tempSize -= deviceData.size();
            deviceData.clear();
        }

        if(tempSize == 0){
            m_state = WaitId;
            m_valid = true;
            qDebug() << m_type;
            qDebug() << m_data.size();
            qDebug() << m_data;
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
    const unsigned header_size = 4;
    converted.resize(header_size + m_data.size());
    converted[0] = static_cast<quint8>(m_type);
    converted[1] = static_cast<quint8>(  m_data.size() & 0xFF);
    converted[2] = static_cast<quint8>( (m_data.size() >> 8) & 0xFF);
    converted[3] = static_cast<quint8>( (m_data.size() >> 16) & 0xFF);

    for(int i(0); i < m_data.size(); ++i){
        converted[header_size+i] = m_data[i];
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

QString SerialCommand::typeToString(SerialCommand::Type t)
{
    switch (t) {
    case Ok:
        return "Ok";
    case Err_Not_Init:
        return "Err_Not_Init";
    case InitCommunication:
        return "InitCommunication";
    case EndCommunication:
        return "EndCommunication";
    case Unavailable:
        return "Unavailable";
    case BadKey:
        return "BadKey";
    case Timeout:
        return "Timeout";
    case GetFram:
        return "GetFram";
    case GetKey:
        return "GetKey";
    case GetParam:
        return "GetParam";
    case SetFram:
        return "SetFram";
    case SetKey:
        return "SetKey";
    case Fram:
        return "Fram";
    case Key:
        return "Key";
    case Param:
        return "Param";
    case Version:
        return "Version";
    case None:
        return "None";
    default:
        return "Unknown";
    }
}
