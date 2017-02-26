#include "SerialDevice.h"

#include <QSerialPortInfo>
#include <QDebug>

SerialDevice::SerialDevice(QObject *parent) : QObject(parent),
    m_waitingFram(false), m_waitingKey(false)
{

}

bool SerialDevice::connectTo(QSerialPortInfo port)
{
    m_serial.setPort(port);
    bool result = m_serial.open(QSerialPort::ReadWrite);
    connect(&m_serial, &QSerialPort::readyRead, this, &SerialDevice::slot_ready_read);
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
    if(m_waitingKey)
    {
        qDebug() << "Cannot request fram while receiving key data";
        return;
    }

    sendByte(0x01);
    m_waitingFram = true;
}

void SerialDevice::requestKey()
{
    if(m_waitingFram)
    {
        qDebug() << "Cannot request key while receiving fram data";
        return;
    }

    sendByte(0x03);
    m_waitingKey = true;
}

void SerialDevice::setFram(const QByteArray &fram)
{
    sendByte(0x02);
    if(m_serial.isOpen())
    {
        m_serial.write(fram);
    }
}

void SerialDevice::setKey(const QByteArray &key)
{
    sendByte(0x04);
    if(m_serial.isOpen())
    {
        m_serial.write(key);
    }
}

void SerialDevice::disconnectSerial()
{
    m_serial.waitForBytesWritten();
    m_serial.close();
    m_waitingFram = false;
    m_waitingKey = false;
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

void SerialDevice::sendByte(char byte)
{
    if(m_serial.isOpen())
    {
        m_serial.write(&byte, 1);
    }
}
