#include "SerialDevice.h"

#include <QSerialPortInfo>
#include <QDebug>

DataSend::DataSend():
    written(0), total(0), sending(false)
{

}


SerialDevice::SerialDevice(QObject *parent) : QObject(parent),
    m_waitingFram(false), m_waitingKey(false), m_waitingParam(false), m_framSize(0)
{
    connect(&m_serial, &QSerialPort::readyRead, this, &SerialDevice::slot_ready_read);
    connect(&m_serial, &QSerialPort::bytesWritten, this, &SerialDevice::slot_bytes_written);
    connect(&m_serial, &QSerialPort::errorOccurred, this, &SerialDevice::slot_error_occured);
}

SerialDevice::~SerialDevice()
{
    if(isConnected())
    {
        disconnectSerial();
    }
}

bool SerialDevice::connectSerial(QSerialPortInfo port)
{
    m_serial.setPort(port);
    bool result = m_serial.open(QSerialPort::ReadWrite);
    initCommunication();
    return result;
}

bool SerialDevice::isConnected()
{
    return m_serial.isOpen();
}

void SerialDevice::disconnectSerial()
{
    endCommunication();
    m_serial.waitForBytesWritten();
    m_serial.close();
    m_waitingFram = false;
    m_waitingKey = false;
    m_waitingParam = false;
    m_currentData.sending = false;
}

void SerialDevice::end()
{
    endCommunication();
}

bool SerialDevice::busy()
{
    return WIP();
}

QSerialPort::SerialPortError SerialDevice::error() const
{
    return m_serial.error();
}

QString SerialDevice::portName() const
{
    return m_serial.portName();
}

void SerialDevice::initCommunication()
{
    sendData(QByteArray(1, 0xA5));
}

void SerialDevice::endCommunication()
{
    sendData(QByteArray(1, 0x5A));
}

void SerialDevice::requestFramDump(quint32 framSize)
{
    if(WIP())
    {
        qDebug() << "Cannot request fram while performing other action.";
        //qDebug() << m_currentData.sending; qDebug() <<  m_waitingFram ;qDebug() <<  m_waitingKey; qDebug() <<  m_waitingParam;
        return;
    }
    m_framSize = framSize;
    m_fram.clear();
    sendData(QByteArray(1, 0x01));
    m_waitingFram = true;
}

void SerialDevice::requestKey()
{
    if(WIP())
    {
        qDebug() << "Cannot request key while performing other action.";
        return;
    }
    m_key.clear();
    sendData(QByteArray(1, 0x03));
    m_waitingKey = true;
}

void SerialDevice::requestParam()
{
    if(WIP())
    {
        qDebug() << "Cannot request parameter while performing other action.";
        return;
    }
    sendData(QByteArray(1, 0x05));
    m_waitingParam = true;
}

void SerialDevice::setFram(const QByteArray &fram)
{
    if(WIP())
    {
        qDebug() << "Cannot send fram while performing other action.";
        return;
    }

    QByteArray data;
    data.append(0x02).append(fram);
    sendData(data);
}

void SerialDevice::setKey(const QByteArray &key)
{
    if(WIP())
    {
        qDebug() << "Cannot send key while performing other action.";
        return;
    }

    QByteArray data;
    data.append(0x04).append(key);
    sendData(data);
}

void SerialDevice::slot_ready_read()
{
    if(m_waitingFram)
    {
        m_fram.append(m_serial.readAll());
        emit framReceiveProgress(m_fram.size());
        if(m_fram.size() >= m_framSize)
        {
            m_waitingFram = false;
            emit framReceived(m_fram);
        }
    }
    else if(m_waitingKey)
    {
        m_key.append(m_serial.readAll());
        if(m_key.size() >= 16)
        {
            m_waitingKey = false;
            emit keyReceived(m_key);
        }
    }
    else if(m_waitingParam)
    {
        m_param.append(m_serial.readAll());
        if(m_param.size() >= 5)
        {
            m_waitingParam = false;
            emit paramReceived(m_param);
        }
    }
}

void SerialDevice::slot_bytes_written(qint64 bytes)
{
    if(!m_currentData.sending)
    {
        qDebug() << "Hey ! What are you sending ?!";
    }
    else
    {
        m_currentData.written += bytes;
        if(m_currentData.written == m_currentData.total)
        {
            m_currentData.sending = false;
            emit sendFinished();
        }
        else if(m_currentData.written > m_currentData.total)
        {
            qDebug() << "Oups i send " << m_currentData.total - m_currentData.written << " illegal bytes :/";
        }
        else
        {
            emit sendProgress(m_currentData.written, m_currentData.total);
        }
    }
}

void SerialDevice::slot_error_occured(QSerialPort::SerialPortError error)
{
    if(!m_serial.isOpen() && error == QSerialPort::ResourceError)
    {
        return;
    }
    else if(error != QSerialPort::NoError)
    {
        qDebug() << "Error : " << m_serial.errorString() << "/" << static_cast<int>(m_serial.error());
    }

    switch (error) {
    case QSerialPort::ResourceError:
        m_serial.close();
        emit disconnected();
        break;
    default:
        break;
    }
}

void SerialDevice::sendData(const QByteArray &data)
{
    if(m_serial.isOpen())
    {
        // If something is already being send, wait for the transaction to finish
        if(m_currentData.sending)
        {
            if(!m_serial.waitForBytesWritten())
            {
                qDebug() << "Error : timed out - "<<m_serial.errorString();
            }
        }
        m_currentData.total = data.size();
        m_currentData.written = 0;
        m_currentData.sending = true;
        m_serial.write(data);
    }

}

bool SerialDevice::WIP() const
{
    return m_currentData.sending || m_waitingFram || m_waitingKey || m_waitingParam;
}
