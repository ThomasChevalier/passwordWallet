#include "SerialDevice.h"

#include <QSerialPortInfo>
#include <QDebug>

SerialDevice::SerialDevice(QObject *parent) : QObject(parent),
    m_waitingFram(false), m_waitingKey(false), m_sendingFram(false), m_sendingKey(false)
{

}

bool SerialDevice::connectTo(QSerialPortInfo port)
{
    m_serial.setPort(port);
    bool result = m_serial.open(QSerialPort::ReadWrite);
    connect(&m_serial, &QSerialPort::readyRead, this, &SerialDevice::slot_ready_read, Qt::UniqueConnection);
    connect(&m_serial, &QSerialPort::bytesWritten, this, &SerialDevice::slot_bytes_written, Qt::UniqueConnection);
    return result;
}

bool SerialDevice::isConnected()
{
    return m_serial.isOpen();
}

void SerialDevice::initCommunication()
{
    sendByte(0xA5);
}

void SerialDevice::endCommunication()
{
    sendByte(0x5A);
}

void SerialDevice::requestFramDump()
{
    if(WIP())
    {
        qDebug() << "Cannot request key while performing other action.";
        return;
    }

    sendByte(0x01);
    m_waitingFram = true;
}

void SerialDevice::requestKey()
{
    if(WIP())
    {
        qDebug() << "Cannot request key while performing other action.";
        return;
    }

    sendByte(0x03);
    m_waitingKey = true;
}

void SerialDevice::setFram(const QByteArray &fram)
{
    if(WIP())
    {
        qDebug() << "Cannot request key while performing other action.";
        return;
    }

    sendByte(0x02);
    m_serial.waitForBytesWritten();
    if(m_serial.isOpen())
    {
        m_serial.write(fram);
        m_serial.waitForBytesWritten();
    }

    m_sendingFram = true;
    m_bytesWritten = 0;
}

void SerialDevice::setKey(const QByteArray &key)
{
    if(WIP())
    {
        qDebug() << "Cannot request key while performing other action.";
        return;
    }

    sendByte(0x04);
    if(m_serial.isOpen())
    {
        m_serial.write(key);
    }

    m_sendingKey = true;
    m_bytesWritten = 0;
}

void SerialDevice::disconnectSerial()
{
    m_serial.waitForBytesWritten();
    m_serial.close();
    m_waitingFram = false;
    m_waitingKey = false;
    m_sendingFram = false;
    m_sendingKey = false;
}

void SerialDevice::slot_ready_read()
{
    if(m_waitingFram)
    {
        m_fram.append(m_serial.readAll());
        if(m_fram.size() == 8192)
        {
            emit framReceived(m_fram);
            m_waitingFram = false;
        }
    }
    else if(m_waitingKey)
    {
        m_key.append(m_serial.readAll());
        if(m_key.size() == 16)
        {
            emit keyReceived(m_key);
            m_waitingKey = false;
        }
    }
    else
    {
        qDebug() << "Received data whereas nothing was expected";
    }
}

void SerialDevice::slot_bytes_written(qint64 bytes)
{
    if(m_sendingFram)
    {
        m_bytesWritten += bytes;
        if(m_bytesWritten >= 8192)
        {
            m_sendingFram = false;
            qDebug() << "Sending fram completed.";
        }
    }
    else if(m_sendingKey)
    {
        m_bytesWritten += bytes;
        if(m_bytesWritten >= 16)
        {
            m_sendingKey = false;
            qDebug() << "Sending key completed.";
        }
    }

}

void SerialDevice::sendByte(char byte)
{
    if(m_serial.isOpen())
    {
        m_serial.write(&byte, 1);
    }
}

bool SerialDevice::WIP() const
{
    return m_sendingFram || m_sendingKey || m_waitingFram || m_waitingKey;
}
