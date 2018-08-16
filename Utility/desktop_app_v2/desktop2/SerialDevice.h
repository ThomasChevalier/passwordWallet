#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QList>
#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "SerialCommand.h"

class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(QObject *parent = nullptr);
    ~SerialDevice();

    bool connectSerial(QSerialPortInfo port);
    bool isConnected() const;
    void disconnectSerial();

    void send(const QByteArray &data);

    QSerialPortInfo portInfo() const;

    static QList<QSerialPortInfo> availableDevices();

    static SerialDevice *instance();

signals:
    void dataSent();
    void dataReceived(QByteArray data);
    void errorOccurred(QSerialPort::SerialPortError error);

private slots:
    void on_bytesWritten(qint64 bytes);
    void on_readyRead();

private:

    QSerialPort m_serial;
    QByteArray m_outData;
};

#endif // SERIALDEVICE_H
