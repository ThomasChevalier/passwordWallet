#ifndef SERIALTERMINAL_H
#define SERIALTERMINAL_H

#include <QWidget>
#include <QStringListModel>

#include <QSerialPortInfo>

#include "SerialCommand.h"

namespace Ui {
class SerialTerminal;
}

class SerialTerminal : public QWidget
{
    Q_OBJECT

public:
    explicit SerialTerminal(QWidget *parent = 0);
    ~SerialTerminal();

private slots:
    void on_portSelected(const QModelIndex &index);

    void on_buttonScan_clicked();
    void on_buttonConnect_clicked();
    void on_buttonDisconnect_clicked();

    void on_buttonSend_clicked();

    void on_command(const SerialCommand& command);
    void on_disconnected();

private:
    QSerialPortInfo getPortByName(const QString& name);
    void display(QString text);

    QByteArray hexStringToByte(const QString& str);

    void scanPorts();

    Ui::SerialTerminal *ui;
    QStringListModel m_portsModel;
};

#endif // SERIALTERMINAL_H
