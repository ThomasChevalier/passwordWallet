#include "SerialInterface.h"

#include <QDebug>

SerialInterface::SerialInterface(QObject *parent):
    QObject(parent),
    m_device(this),
    m_commandStack(),
    m_sending(false),
    m_timeoutTimer()
{
    connect(&m_timeoutTimer, &QTimer::timeout, this, &SerialInterface::on_timeout);
    m_timeoutTimer.setInterval(DEFAULT_TIMEOUT);
    m_timeoutTimer.setSingleShot(true);

    connect(&m_device, &SerialDevice::dataSent, this, &SerialInterface::on_dataSent);
    connect(&m_device, &SerialDevice::dataReceived, this, &SerialInterface::on_dataReceived);
    connect(&m_device, &SerialDevice::errorOccurred, this, &SerialInterface::on_error);

    connect(this, &SerialInterface::serialDisconnected, this, &SerialInterface::on_serial_disconnected);
}

SerialInterface::~SerialInterface()
{
    disconnectSerial();
}

void SerialInterface::connectSerial(QSerialPortInfo port)
{
    if(!m_device.connectSerial(port))
    {
        emit connectionError();
        return;
    }
    pushCommand(SerialCommand(SerialCommand::InitCommunication));
}

bool SerialInterface::isConnected() const
{
    return m_device.isConnected();
}

void SerialInterface::disconnectSerial()
{
    if(isConnected())
    {
        pushCommand(SerialCommand(SerialCommand::EndCommunication));
    }
}

void SerialInterface::pushCommand(const SerialCommand &command)
{
     m_commandStack.push(command);
     if(!m_sending){
         sendFirstCommand();
     }
}

int SerialInterface::timeout() const
{
    return m_timeoutTimer.interval();
}

void SerialInterface::setTimeout(int timeout)
{
    m_timeoutTimer.setInterval(timeout);
}

void SerialInterface::on_timeout()
{
    m_sending = false;
    emit serialDisconnected();
}

void SerialInterface::on_dataSent()
{
    m_commandStack.pop();
    if(!m_commandStack.isEmpty()){
        sendFirstCommand();
    }
    else{
        m_sending = false;
    }
}

void SerialInterface::on_dataReceived(QByteArray data)
{
    static SerialCommand inCommand;
    while(!data.isEmpty())
    {
        // If the command is complete then notify
        if(inCommand.readFrom(data)){
            emit commandReceived(inCommand);
        }
    }
}

void SerialInterface::on_error(QSerialPort::SerialPortError error)
{
    if(isConnected() && error == QSerialPort::ResourceError){
        emit serialDisconnected();
        return;
    }
    qDebug() << "Error : " << static_cast<int>(error);
}

void SerialInterface::on_serial_disconnected()
{
    m_device.disconnectSerial();
    m_commandStack.clear();
    m_sending = false;
    m_timeoutTimer.stop();
}

void SerialInterface::sendFirstCommand()
{
    m_device.send(m_commandStack.first().toByteArray());
    m_sending = true;
    m_timeoutTimer.start();
}
