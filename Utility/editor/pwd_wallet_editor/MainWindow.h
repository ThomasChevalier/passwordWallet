#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QFile>

#include "SerialDevice.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slot_fram_received(const QByteArray& fram);
    void slot_key_received(const QByteArray& key);

    void on_buttonConnect_clicked();
    void on_buttonDisconnect_clicked();

    void on_buttonSetFram_clicked();
    void on_buttonGetFram_clicked();
    void on_buttonSetKey_clicked();
    void on_buttonGetKey_clicked();

    void on_actionOpenBackup_triggered();

private:

    QByteArray openFramFile();

    bool connectToSerial(QSerialPortInfo port);
    Ui::MainWindow *ui;
    SerialDevice m_device;
    QByteArray m_fram;
};

#endif // MAINWINDOW_H
