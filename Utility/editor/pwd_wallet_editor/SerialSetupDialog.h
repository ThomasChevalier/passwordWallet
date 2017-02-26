#ifndef SERIALSETUPDIALOG_H
#define SERIALSETUPDIALOG_H

#include <QDialog>

#include <QList>
#include <QSerialPortInfo>

namespace Ui {
class SerialSetupDialog;
}

class SerialSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialSetupDialog(QWidget *parent = 0);
    ~SerialSetupDialog();

    QSerialPortInfo port() const;

public slots:
    void on_buttonRefresh_clicked();
    void on_comboSerialAvailable_currentIndexChanged(int index);

private:
    Ui::SerialSetupDialog *ui;
    QList<QSerialPortInfo> m_ports;
};

#endif // SERIALSETUPDIALOG_H
