#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SerialSetupDialog.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textArea->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_ready_read()
{
    ui->textArea->appendPlainText(QString::fromLocal8Bit(m_serial.readAll()));
}

void MainWindow::on_buttonConnect_clicked()
{
    SerialSetupDialog diag;
    int result = diag.exec();
    if(result == QDialog::Accepted && !diag.port().isNull())
    {
        connectToSerial(diag.port());
        ui->textArea->show();
        ui->labelConnect->hide();
        ui->buttonConnect->hide();
    }
    else
    {
        ui->labelConnect->show();
        ui->buttonConnect->show();
        ui->textArea->hide();
    }
}

void MainWindow::connectToSerial(QSerialPortInfo port)
{
    m_serial.setPort(port);

    if(m_serial.open(QSerialPort::ReadOnly))
    {
        qDebug() << "Connected on " << port.portName() << " : PID = " << port.productIdentifier() << " VID = " << port.vendorIdentifier();
        connect(&m_serial, &QSerialPort::readyRead, this, &MainWindow::slot_ready_read);
    }
    else
    {
        qDebug() << "Connect error : " << m_serial.errorString();
        qDebug() <<"(" << port.portName() << " : PID = " << port.productIdentifier() << " VID = " << port.vendorIdentifier() << ")";
    }

}
