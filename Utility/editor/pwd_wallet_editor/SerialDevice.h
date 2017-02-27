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
    void sendFinished();
    void framReceived(const QByteArray& fram);
    void keyReceived(const QByteArray& key);

private slots:
    void slot_ready_read();
    void slot_bytes_written(qint64 bytes);

private:
    void sendByte(char byte);
    bool WIP() const;

    QSerialPort m_serial;

    bool m_waitingFram;
    bool m_waitingKey;
    bool m_sendingFram;
    bool m_sendingKey;
    qint64 m_bytesWritten;


    QByteArray m_fram;
    QByteArray m_key;
};

#endif // SERIALDEVICE_H
