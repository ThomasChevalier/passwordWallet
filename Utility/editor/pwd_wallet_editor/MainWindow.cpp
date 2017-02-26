#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SerialSetupDialog.h"

#include <QFileDialog>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textArea->hide();

    QString fileName = QFileDialog::getSaveFileName(this, "Fichier de dump", "/home/");
    if(!fileName.isEmpty())
    {
        m_savingFile.setFileName(fileName);
        if(!m_savingFile.open(QFile::WriteOnly))
        {
            qDebug() << "Error opening " << fileName << " in write only mode.";
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_ready_read()
{
    if(m_savingFile.isOpen())
    {
        m_savingFile.write(m_serial.readAll());
    }
    else
    {
        ui->textArea->appendPlainText(QString::fromLocal8Bit(m_serial.readAll().toHex()));
    }

}

void MainWindow::on_buttonConnect_clicked()
{
    SerialSetupDialog diag;
    int result = diag.exec();
    if(result == QDialog::Accepted && !diag.port().isNull())
    {
        if(connectToSerial(diag.port()))
        {
            ui->textArea->show();
            ui->labelConnect->hide();
            ui->buttonConnect->hide();
            return;
        }
    }
    ui->labelConnect->show();
    ui->buttonConnect->show();
    ui->textArea->hide();
}

void MainWindow::on_buttonSend_clicked()
{
    if(m_serial.isOpen())
    {
        m_serial.write(QString("Test123").toLocal8Bit());
    }
}

bool MainWindow::connectToSerial(QSerialPortInfo port)
{
    m_serial.setPort(port);

    if(m_serial.open(QSerialPort::ReadWrite))
    {
        qDebug() << "Connected on " << port.portName() << " : PID = " << port.productIdentifier() << " VID = " << port.vendorIdentifier();
        connect(&m_serial, &QSerialPort::readyRead, this, &MainWindow::slot_ready_read);
        return true;
    }
    else
    {
        qDebug() << "Connect error : " << m_serial.errorString();
        qDebug() <<"(" << port.portName() << " : PID = " << port.productIdentifier() << " VID = " << port.vendorIdentifier() << ")";
        return false;
    }

}
