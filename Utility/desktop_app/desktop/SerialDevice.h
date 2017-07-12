#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>

struct DataSend
{
    DataSend();

    qint64 written;
    qint64 total;
    bool sending;
};

class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(QObject *parent = 0);
    ~SerialDevice();

    bool connectSerial(QSerialPortInfo port);
    bool isConnected();
    void disconnectSerial();
    bool busy();

    void initCommunication();
    void endCommunication();
    void requestFramDump(quint32 framSize);
    void requestKey();
    void requestParam();
    void setFram(const QByteArray& fram);
    void setKey(const QByteArray& key);

signals:
    void disconnected();
    void sendFinished();
    void sendProgress(qint64 written, qint64 total);
    void framReceived(const QByteArray& fram);
    void keyReceived(const QByteArray& key);
    void paramReceived(const QByteArray& param);
    void framReceiveProgress(qint64 received);

private slots:
    void slot_ready_read();
    void slot_bytes_written(qint64 bytes);
    void slot_error_occured(QSerialPort::SerialPortError error);

private:
    void sendData(const QByteArray& data);
    bool WIP() const;

    QSerialPort m_serial;

    bool m_waitingFram;
    bool m_waitingKey;
    bool m_waitingParam;

    QByteArray m_fram;
    quint32 m_framSize;
    QByteArray m_key;
    QByteArray m_param;

    DataSend m_currentData;
};

#endif // SERIALDEVICE_H
