#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QFile>

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
    void slot_ready_read();
    void on_buttonConnect_clicked();
    void on_buttonSend_clicked();

private:
    bool connectToSerial(QSerialPortInfo port);
    Ui::MainWindow *ui;
    QSerialPort m_serial;
    QFile m_savingFile;
};

#endif // MAINWINDOW_H
