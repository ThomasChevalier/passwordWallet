#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <QDebug>

#include "SerialDevice.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_serial = new SerialDevice(this);

    connect(m_serial, &SerialDevice::disconnected, this, &MainWindow::on_disconnected);
    connect(m_serial, &SerialDevice::paramReceived, this, &MainWindow::on_paramReceived);
    connect(m_serial, &SerialDevice::sendProgress, this, &MainWindow::on_sendProgress);
    connect(m_serial, &SerialDevice::framReceived, this, &MainWindow::on_framReceived);
    connect(m_serial, &SerialDevice::keyReceived, this, &MainWindow::on_keyReceived);
    connect(m_serial, &SerialDevice::framReceiveProgress, this, &MainWindow::on_framReceiveProgress);

    // On startup auto connect
    ConnectionDialog diag(*m_serial, m_data);
    diag.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_disconnected()
{

}

void MainWindow::on_sendFinished()
{

}

void MainWindow::on_sendProgress(qint64 written, qint64 total)
{

}

void MainWindow::on_framReceived(const QByteArray &fram)
{

}

void MainWindow::on_keyReceived(const QByteArray &key)
{

}

void MainWindow::on_paramReceived(const QByteArray &param)
{

}

void MainWindow::on_framReceiveProgress(qint64 received)
{

}
