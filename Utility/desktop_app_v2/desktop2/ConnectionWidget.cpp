#include "ConnectionWidget.h"
#include "ui_ConnectionWidget.h"

#include <QSerialPortInfo>
#include <QDebug>

#include "SerialDevice.h"
#include "ResolveConnectionIssuesDialog.h"

ConnectionWidget::ConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionWidget)
{
    ui->setupUi(this);
    ui->textInfo->hide();
    adjustSize();

    m_exploreTimer = new QTimer(this);
    m_exploreTimer->setSingleShot(false);
    QObject::connect(m_exploreTimer, &QTimer::timeout, this, &ConnectionWidget::auto_explore, Qt::UniqueConnection);
    QObject::connect(SerialDevice::instance(), &SerialDevice::errorOccurred, this, &ConnectionWidget::processError);
    //connect();
}

ConnectionWidget::~ConnectionWidget()
{
    delete ui;
}

void ConnectionWidget::connect()
{
    m_exploreTimer->stop();

    m_exploreTimer->start(100);
    auto_explore(); // Try directly
}

void ConnectionWidget::on_showButton_clicked()
{
    if(!ui->textInfo->isVisible()){
        ui->showButton->setText(tr("Moins ..."));
        ui->textInfo->show();
    }
    else{
        ui->showButton->setText(tr("Plus ..."));
        ui->textInfo->hide();
    }
}

void ConnectionWidget::auto_explore()
{
    QList<QSerialPortInfo> devices = SerialDevice::availableDevices();
    for(QSerialPortInfo info : devices)
    {
        ui->textInfo->append(tr("%1 est disponible").arg(info.portName()));
        if(SerialDevice::instance()->connectSerial(info))
        {
            m_exploreTimer->stop();
            ui->textInfo->append(tr("Connecté à %1").arg(info.portName()));
        }
    }
}

void ConnectionWidget::processError(QSerialPort::SerialPortError error)
{
    if(error != QSerialPort::NoError){
        m_exploreTimer->stop();

        ui->textInfo->append(tr("Une erreur est survenue : \"%1\".").arg(error));
        ui->textInfo->append(tr("Lancement de l'utilitaire de résolution de problème"));
        ResolveConnectionIssuesDialog diag(this);
        diag.show();
        diag.setError(error);
        diag.launch();
    }
}
