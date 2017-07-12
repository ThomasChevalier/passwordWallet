#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <QDebug>

#include "SerialDevice.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_connecting(false)
{
    ui->setupUi(this);

    m_exploreTimer = new QTimer(this);
    m_serial = new SerialDevice(this);

    connect(m_serial, &SerialDevice::disconnected, this, &MainWindow::on_disconnected);
    connect(m_serial, &SerialDevice::paramReceived, this, &MainWindow::on_paramReceived);
    connect(m_serial, &SerialDevice::sendProgress, this, &MainWindow::on_sendProgress);
    connect(m_serial, &SerialDevice::framReceived, this, &MainWindow::on_framReceived);
    connect(m_serial, &SerialDevice::keyReceived, this, &MainWindow::on_keyReceived);
    connect(m_serial, &SerialDevice::framReceiveProgress, this, &MainWindow::on_framReceiveProgress);

    // On startup auto connect
    tryConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_disconnected()
{
    if(m_connecting)
    {
        m_connectDial.addMessage(tr("Périphérique déconnecté."));
        m_connectDial.setStepResult(ConnectionDialog::Error);
        m_connectDial.goToStep(0);
        m_connectDial.setStepResult(ConnectionDialog::Wait);
        tryConnect();
    }
}

void MainWindow::on_sendFinished()
{

}

void MainWindow::on_sendProgress(qint64 written, qint64 total)
{

}

void MainWindow::on_framReceived(const QByteArray &fram)
{
    m_data.setDeviceFram(fram);
    if(m_connecting)
    {
        m_connectDial.stepComplete(ConnectionDialog::Ok);
        m_connectDial.addMessage(tr("Mémoire reçue"));
        m_connectDial.allIsComplete();
    }
}

void MainWindow::on_keyReceived(const QByteArray &key)
{
    m_data.setDeviceKey(key);
    if(m_connecting)
    {
        m_connectDial.stepComplete(ConnectionDialog::Ok);
        m_connectDial.addMessage(tr("Clef de chiffrement reçue"));
        m_serial->requestFramDump(m_data.getDeviceFramSize());
    }
}

void MainWindow::on_paramReceived(const QByteArray &param)
{
    if(m_connecting)
    {
        m_connectDial.addMessage(tr("Paramètres reçues"));
        m_connectDial.stepComplete(ConnectionDialog::Ok);
        m_data.setParameter(param);
        m_connectDial.addMessage(tr("Taille de la mémoire : %1").arg(m_data.getDeviceFramSize()));

        m_serial->requestKey();
    }
}

void MainWindow::on_framReceiveProgress(qint64 received)
{
    if(m_connecting)
    {
        m_connectDial.setProgress(received, 0, m_data.getDeviceFramSize());
    }
}

void MainWindow::tryConnect()
{
    m_connecting = true;
    m_exploreTimer->stop();
    connect(m_exploreTimer, &QTimer::timeout, this, &MainWindow::explorePorts, Qt::UniqueConnection);
    m_connectDial.setModal(true);
    m_connectDial.show();
    m_exploreTimer->start(100);
}

void MainWindow::explorePorts()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo port : ports)
    {
        if(port.vendorIdentifier() == 0x03EB && port.productIdentifier() == 0x2044)
        {
            m_connectDial.addMessage(tr("Périphérique trouvé:VID=0x%1|PID=0x%2|Nom=%3").arg(port.vendorIdentifier(),0,16).arg(port.productIdentifier(), 0, 16).arg(port.portName()));
            connectDevice(port);
        }
    }
}

void MainWindow::connectDevice(QSerialPortInfo &info)
{
    if(m_serial->connectSerial(info))
    {
        m_connectDial.addMessage(tr("Connecté avec succés"));
        m_connectDial.stepComplete(ConnectionDialog::Ok);
        m_exploreTimer->stop();
        getParameter();
    }
    else
    {
        m_connectDial.addMessage(tr("Connexion refusée"));
        m_connectDial.setStepResult(ConnectionDialog::Error);
    }
}

void MainWindow::getParameter()
{
    // Wait for the inital message to be send
    while(m_serial->busy()){QCoreApplication::processEvents();}
    m_serial->requestParam();
}
