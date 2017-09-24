#include "SerialDevice.h"

SerialDevice::SerialDevice(QObject *parent) : QObject(parent)
{
    connect(&m_serial, &QSerialPort::bytesWritten, this, &SerialDevice::on_bytesWritten);
    connect(&m_serial, &QSerialPort::readyRead, this, &SerialDevice::on_readyRead);
    connect(&m_serial, &QSerialPort::errorOccurred, this, &SerialDevice::errorOccurred);
}

SerialDevice::~SerialDevice()
{

}

bool SerialDevice::connectSerial(QSerialPortInfo port)
{
    disconnectSerial();

    m_serial.setPort(port);
    const bool result = m_serial.open(QSerialPort::ReadWrite);
    return result;
}

bool SerialDevice::isConnected() const
{
    return m_serial.isOpen();
}

void SerialDevice::disconnectSerial()
{
    if(isConnected()){
        m_serial.close();
    }
}

void SerialDevice::send(const QByteArray& data)
{
    // If there is already data then we must wait until there are sent.
    if(!m_outData.isEmpty()){
        while(!m_outData.isEmpty()){
            if(!m_serial.waitForBytesWritten(1000)){
                return;
            }
        }
    }

    m_outData = data;

    // Try to write data and remove from the "outData"
    // the data that have been sent
    const qint64 written = m_serial.write(m_outData);
    m_outData.remove(0, written);
}

void SerialDevice::on_bytesWritten(qint64 bytes)
{
    if(!m_outData.isEmpty()){
        send(m_outData);
    }
    else{
        emit dataSent();
    }
}

void SerialDevice::on_readyRead()
{
    emit dataReceived(m_serial.readAll());
}
