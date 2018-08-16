#include "SerialDevice.h"

#include <QDebug>

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
//    qDebug() << written << " bytes written to serial.";
    m_outData.remove(0, written);
}

QSerialPortInfo SerialDevice::portInfo() const
{
    const QString portName = m_serial.portName();
    for(const QSerialPortInfo& info : QSerialPortInfo::availablePorts())
    {
        if(info.portName() == portName){
            return info;
        }
    }
    return QSerialPortInfo();
}

QList<QSerialPortInfo> SerialDevice::availableDevices()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo> valid;
    for(QSerialPortInfo port : ports)
    {
        if(port.vendorIdentifier() == 0x03EB && port.productIdentifier() == 0x2044)
        {
            valid << port;
        }
    }
    return valid;
}

SerialDevice *SerialDevice::instance()
{
    static SerialDevice* inst = nullptr;
    if(inst == nullptr){
        inst = new SerialDevice;
    }
    return inst;
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
    const QByteArray all = m_serial.readAll();
//    qDebug() << all.size() << " bytes received from serial.";
    emit dataReceived(all);
}
