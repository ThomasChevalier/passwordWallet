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
    ui->widgetAction->hide();
    ui->buttonDisconnect->hide();
}

MainWindow::~MainWindow()
{
    m_device.endCommunication();
    delete ui;
}

void MainWindow::slot_fram_received(const QByteArray &fram)
{
    QString fileName = QFileDialog::getSaveFileName(this, "Fichier dump fram", "/home/");
    if(!fileName.isEmpty())
    {
        QFile savingFile(fileName);
        if(!savingFile.open(QFile::WriteOnly))
        {
            qDebug() << "Error opening " << fileName << " in write only mode.";
        }
        savingFile.write(fram);
    }
}

void MainWindow::slot_key_received(const QByteArray& key)
{
    ui->textArea->appendPlainText(QString::fromLocal8Bit(key.toHex()));
}

void MainWindow::on_buttonConnect_clicked()
{
    SerialSetupDialog diag;
    int result = diag.exec();
    if(result == QDialog::Accepted && !diag.port().isNull())
    {
        if(m_device.connectTo(diag.port()))
        {
            m_device.initCommunication();
            connect(&m_device, &SerialDevice::framReceived, this, &MainWindow::slot_fram_received);
            connect(&m_device, &SerialDevice::keyReceived, this, &MainWindow::slot_key_received);
            ui->textArea->show();
            ui->widgetAction->show();
            ui->buttonDisconnect->show();
            ui->labelConnect->hide();
            ui->buttonConnect->hide();
            ui->statusBar->showMessage(QString("Connected to %1 (VID = %2 PID = %3)").arg(diag.port().portName(), diag.port().vendorIdentifier(), diag.port().productIdentifier()));
            return;
        }
    }
    ui->labelConnect->show();
    ui->buttonConnect->show();
    ui->textArea->hide();
    ui->widgetAction->hide();
    ui->buttonDisconnect->hide();
}

void MainWindow::on_buttonDisconnect_clicked()
{
    m_device.endCommunication();
    m_device.disconnectSerial();
    ui->labelConnect->show();
    ui->buttonConnect->show();
    ui->textArea->hide();
    ui->widgetAction->hide();
    ui->buttonDisconnect->hide();
}

void MainWindow::on_buttonSetFram_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Fram file");
    if(fileName.isEmpty())
    {
        return;
    }
    QFile framFile(fileName);
    if(!framFile.open(QFile::ReadOnly))
    {
        qDebug() << "Impossible d'ouvrir \"" << fileName << "\" en lecture seulement.";
        return;
    }
    if(framFile.size() != 8192)
    {
        qDebug() << "Fichier invalide (taille incompatible).";
        return;
    }
    m_device.setFram(framFile.readAll());
}

void MainWindow::on_buttonGetFram_clicked()
{
    m_device.requestFramDump();
}

void MainWindow::on_buttonSetKey_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Key file");
    if(fileName.isEmpty())
    {
        return;
    }
    QFile keyFile(fileName);
    if(!keyFile.open(QFile::ReadOnly))
    {
        qDebug() << "Impossible d'ouvrir \"" << fileName << "\" en lecture seulement.";
        return;
    }
    if(keyFile.size() != 16)
    {
        qDebug() << "Fichier invalide (taille incompatible).";
        return;
    }
    m_device.setKey(keyFile.readAll());
}

void MainWindow::on_buttonGetKey_clicked()
{
    m_device.requestKey();
}
