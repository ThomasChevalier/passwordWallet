#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>


#include "PasswordTabView.h"
#include "OptionTabView.h"
#include "RestoreTabView.h"

#include "SerialDevice.h.old"
#include "KeyDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_serial = new SerialDevice(this);

    connect(m_serial, &SerialDevice::disconnected, this, &MainWindow::on_disconnected);
    connect(m_serial, &SerialDevice::paramReceived, this, &MainWindow::on_paramReceived);
    connect(m_serial, &SerialDevice::sendProgress, this, &MainWindow::on_sendProgress);
    connect(m_serial, &SerialDevice::sendFinished, this, &MainWindow::on_sendFinished);
    connect(m_serial, &SerialDevice::framReceived, this, &MainWindow::on_framReceived);
    connect(m_serial, &SerialDevice::keyReceived, this, &MainWindow::on_keyReceived);
    connect(m_serial, &SerialDevice::framReceiveProgress, this, &MainWindow::on_framReceiveProgress);

    m_pwdView = new PasswordTabView(m_data,this);
    ui->tabPwd->layout()->addWidget(m_pwdView);

    m_optView = new OptionTabView(m_data, this);
    ui->tabOption->layout()->addWidget(m_optView);

    m_restoreView = new RestoreTabView(m_data, *m_serial, this);
    ui->tabRestore->layout()->addWidget(m_restoreView);

    // On startup auto connect
    QTimer::singleShot(0, this, SLOT(connectDevice()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_disconnected()
{

    ui->actionDisconnect->setEnabled(false);
    ui->actionConnect->setEnabled(true);
}

void MainWindow::on_sendFinished()
{
    qDebug() << "Send finished";
}

void MainWindow::on_sendProgress(qint64 written, qint64 total)
{
    qDebug() << written << " / " << total;
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

void MainWindow::connectDevice()
{
//    QFile file("/home/thomas/Documents/param.hex");
//    if(!file.open(QFile::ReadOnly))
//    {
//        return;
//    }
//    m_data.setParameter(file.readAll());
//    file.close();
//    file.setFileName("/home/thomas/Documents/fram.hex");
//    if(!file.open(QFile::ReadOnly))
//    {
//        return;
//    }
//    m_data.setMemory(file.readAll());
    ConnectionDialog diag(*m_serial, m_data);
    diag.exec();
    if(!diag.isConnectionComplete())
    {
        ui->actionDisconnect->setEnabled(false);
        ui->actionConnect->setEnabled(true);
        return;
    }
    ui->actionDisconnect->setEnabled(true);
    ui->actionConnect->setEnabled(false);

    // If the key is empty
    if(m_data.key() == QByteArray(16,0))
    {
        QMessageBox::StandardButton but = QMessageBox::question(this,
                                                                tr("Clef de chiffrement"),
                                                                tr("Il semble que vous n'avez pas dévérouillé votre périphérique."
                                                                   "Voulez vous entrer la clef de chiffrement manuellement ?"));
        if(but == QMessageBox::Yes)
        {
            KeyDialog keyDiag;
            if(keyDiag.exec() == QDialog::Accepted)
            {
                m_data.setKey(keyDiag.key());
            }
        }
    }
}

void MainWindow::on_buttonSync_clicked()
{
    m_serial->setFram(m_data.memory());
}
