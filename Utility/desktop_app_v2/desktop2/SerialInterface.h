#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QStack>
#include <QTimer>
#include <QObject>
#include <QByteArray>
#include <QEventLoop>
#include <QSerialPortInfo>

#include "SerialDevice.h"
#include "SerialCommand.h"

#define DEFAULT_TIMEOUT (100000000)

class SerialInterface : public QObject
{
    Q_OBJECT
public:
    SerialInterface(QObject *parent = nullptr);
    ~SerialInterface();

    bool connectSerial(QSerialPortInfo port);
    bool isConnected() const;
    void disconnectSerial();

    void pushCommand(const SerialCommand& command);

    int timeout() const;
    void setTimeout(int timeout);

    static SerialInterface &get();

    QSerialPortInfo portInfo() const;

signals:
    void connectionError();
    void serialConnected();
    void serialDisconnected();

    void commandReceived(SerialCommand command);

private slots:
    void on_timeout();
    void on_dataSent();
    void on_dataReceived(QByteArray data);
    void on_error(QSerialPort::SerialPortError error);

    void on_serial_disconnected();

private:

    void sendFirstCommand();

    SerialDevice m_device;
    QStack<SerialCommand> m_commandStack;
    bool m_sending;

    QTimer m_timeoutTimer;
};


#endif // SERIALINTERFACE_H
