#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

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

private:
    void connectToSerial(QSerialPortInfo port);
    Ui::MainWindow *ui;
    QSerialPort m_serial;
};

#endif // MAINWINDOW_H
