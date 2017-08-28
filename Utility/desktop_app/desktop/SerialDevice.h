#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>
#include <QStack>
#include <QEventLoop>

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
    void end();
    bool busy();

    QSerialPort::SerialPortError error() const;
    QString portName() const;

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

struct SerialCommand
{
    enum Type
    {
        RequestId,
        EndCommunication,
        InitCommunication,

        None
    };
    SerialCommand(Type type_ = None, const QByteArray& data_ = QByteArray()):
        type(type_), data(data_)
    {}
    Type type;
    QByteArray data;

    unsigned bytesExcepted() const;
};

class CommandWaiter : public QObject
{
    Q_OBJECT
public:
    CommandWaiter (SerialCommand::Type condition);

    void wait();

public slots:
    void scanSignal(SerialCommand::Type type);

signals:
    void finished();

private:
    SerialCommand::Type m_condition;
    QEventLoop m_pause;
    bool m_triggered;
};

class SerialDevice2 : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice2(QObject *parent = 0);
    ~SerialDevice2();

    bool connectSerial(QSerialPortInfo port);
    bool isConnected();
    void disconnectSerial();

    void pushCommand(SerialCommand::Type type, const QByteArray& data = QByteArray());
    void pushCommand(SerialCommand command);

    void pushAndWait(SerialCommand::Type type, const QByteArray& data = QByteArray());
    void pushAndWait(SerialCommand command);

signals:
    void commandSent(SerialCommand::Type type);
    void commandReceive(SerialCommand command);

    void disconnected();

private slots:
    void slot_ready_read();
    void slot_bytes_written(qint64 bytes);
    void slot_error_occured(QSerialPort::SerialPortError error);

private:
    void updateSerial();

    QSerialPort m_serial;
    QStack<SerialCommand> m_commandStack;
    SerialCommand m_currentCommand;
};

#endif // SERIALDEVICE_H
