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

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    on_buttonDisconnect_clicked();
    delete ui;
}// 20b4aa14de3d744d0a82021d20d74bb8

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
    QSerialPortInfo port = diag.port();
    if(result == QDialog::Accepted && !port.isNull())
    {
        if(m_device.connectTo(port))
        {
            m_device.initCommunication();
            connect(&m_device, &SerialDevice::framReceived, this, &MainWindow::slot_fram_received, Qt::UniqueConnection);
            connect(&m_device, &SerialDevice::keyReceived, this, &MainWindow::slot_key_received, Qt::UniqueConnection);
            ui->textArea->show();
            ui->widgetAction->show();
            ui->buttonDisconnect->show();
            ui->labelConnect->hide();
            ui->buttonConnect->hide();
            ui->statusBar->showMessage("Connected to " + port.portName() + " (VID =" + QString::number(port.vendorIdentifier()) + " PID =" + QString::number(port.productIdentifier()) +")", 2000);
            return;
        }
        else
        {
            ui->statusBar->showMessage("Unable to connect to device");
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
    ui->textArea->clear();
}

void MainWindow::on_buttonSetFram_clicked()
{
    QByteArray fram = openFramFile();
    if(!fram.isEmpty())
    {
        m_device.setFram(openFramFile());
    }
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
//cd317bd92b9cd8a80dc27f0ccbfc4ef1
void MainWindow::on_buttonGetKey_clicked()
{
    m_device.requestKey();
}

void MainWindow::on_actionOpenBackup_triggered()
{
    m_fram = openFramFile();
}

QByteArray MainWindow::openFramFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Fram file");
    if(fileName.isEmpty())
    {
        return QByteArray();
    }
    QFile framFile(fileName);
    if(!framFile.open(QFile::ReadOnly))
    {
        qDebug() << "Impossible d'ouvrir \"" << fileName << "\" en lecture seulement.";
        return QByteArray();
    }
    if(framFile.size() != 8192)
    {
        qDebug() << "Fichier invalide (taille incompatible).";
        return QByteArray();
    }
    return framFile.readAll();
}
