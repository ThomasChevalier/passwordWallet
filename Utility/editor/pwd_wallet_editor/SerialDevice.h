#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>

class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(QObject *parent = 0);

    bool connectTo(QSerialPortInfo port);
    bool isConnected();

    void initCommunication();
    void endCommunication();
    void requestFramDump();
    void requestKey();
    void setFram(const QByteArray& fram);
    void setKey(const QByteArray& key);

    void disconnectSerial();

signals:
    void framReceived(const QByteArray& fram);
    void keyReceived(const QByteArray& key);

private slots:
    void slot_ready_read();

private:
    void sendByte(char byte);

    QSerialPort m_serial;

    bool m_waitingFram;
    bool m_waitingKey;
    QByteArray m_fram;
    QByteArray m_key;
};

#endif // SERIALDEVICE_H
