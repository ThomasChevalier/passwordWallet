#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "connectiondialog.h"
#include "DeviceData.h"

class QSerialPortInfo;
class SerialDevice;

class PasswordTabView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_disconnected();
    void on_sendFinished();
    void on_sendProgress(qint64 written, qint64 total);
    void on_framReceived(const QByteArray& fram);
    void on_keyReceived(const QByteArray& key);
    void on_paramReceived(const QByteArray& param);
    void on_framReceiveProgress(qint64 received);

    void connectDevice();

private:

    Ui::MainWindow *ui;
    PasswordTabView *m_pwdView;

    SerialDevice *m_serial;
    DeviceData m_data;
};

#endif // MAINWINDOW_H
