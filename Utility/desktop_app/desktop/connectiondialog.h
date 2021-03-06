#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

#include <QTimer>
#include <QList>

#include <QSerialPort>

#include "DeviceData.h"
#include "SerialCommand.h"

class QLabel;

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(DeviceData& data, QWidget *parent = 0);
    ~ConnectionDialog();

    bool isConnectionComplete();

public slots:
    void addMessage(QString msg);

private slots:
    void on_disconnected();
    void on_data(const SerialCommand& command);

    void on_issueLinuxEnterUsrName_clicked();

    void on_buttonReconnect_clicked();

    void on_rejected();

private:

    enum State
    {
        Connecting,
        WaitingParam,
        WaitingKey,
        WaitingFram,
        Complete
    };

    void tryConnect();

    void explorePorts();
    QTimer* m_exploreTimer;

    void connectDevice(QSerialPortInfo& info);
    unsigned int m_connectionAttempt;

    void resolveConnectionIssue(QString userName = QString());
    void resolveDisconnectionIssue();
    void getParameter();

    Ui::ConnectionDialog *ui;

    State m_currentState;

    QList<QMetaObject::Connection> m_deviceConnections;
    DeviceData& m_data;
};

#endif // CONNECTIONDIALOG_H
