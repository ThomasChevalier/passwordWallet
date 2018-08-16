#ifndef RESOLVECONNECTIONISSUESDIALOG_H
#define RESOLVECONNECTIONISSUESDIALOG_H

#include <QDialog>
#include <QSerialPortInfo>
#include <QSerialPort>

namespace Ui {
class ResolveConnectionIssuesDialog;
}

class ResolveConnectionIssuesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResolveConnectionIssuesDialog(QWidget *parent = 0);
    ~ResolveConnectionIssuesDialog();

    void setError(QSerialPort::SerialPortError error);

    void launch();
private:
    Ui::ResolveConnectionIssuesDialog *ui;

    void msg(const QString& text);

    void tryConnect();
    void noPeriphericFound();
    void permissionError();
    void resourceError();
    void severalInstances();

    QSerialPortInfo m_portInfo;
    QSerialPort m_serial;

    QString m_userName;
};

#endif // RESOLVECONNECTIONISSUESDIALOG_H
