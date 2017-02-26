#include "SerialSetupDialog.h"
#include "ui_SerialSetupDialog.h"

SerialSetupDialog::SerialSetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialSetupDialog)
{
    ui->setupUi(this);
    on_buttonRefresh_clicked();
}

SerialSetupDialog::~SerialSetupDialog()
{
    delete ui;
}

QSerialPortInfo SerialSetupDialog::port() const
{
    if(m_ports.isEmpty())
    {
        return QSerialPortInfo();
    }

    return m_ports.at(ui->comboSerialAvailable->currentIndex());
}

void SerialSetupDialog::on_buttonRefresh_clicked()
{
    m_ports = QSerialPortInfo::availablePorts();
    if(m_ports.isEmpty())
    {
        ui->labelNoPort->show();
        ui->widgetPortAvailable->hide();
    }
    else
    {
        ui->labelNoPort->hide();
        ui->widgetPortAvailable->show();
        ui->comboSerialAvailable->clear();
        for(QSerialPortInfo port : m_ports)
        {
            ui->comboSerialAvailable->addItem(port.portName());
        }
    }
}

void SerialSetupDialog::on_comboSerialAvailable_currentIndexChanged(int index)
{
    if(m_ports.isEmpty() || index >= m_ports.size() || index < 0)
        return;

    QSerialPortInfo port = m_ports.at(index);

    // Description
    if(port.description().isEmpty())
    {
        ui->labelDescription->hide();
    }
    else
    {
        ui->labelDescription->setText(port.description());
        ui->labelDescription->show();
    }
    // PID
    if(!port.hasProductIdentifier())
    {
        ui->labelPid->setText("none");
    }
    else
    {
        ui->labelPid->setText(QString::number(port.productIdentifier()));
    }
    // VID
    if(!port.hasVendorIdentifier())
    {
        ui->labelVid->setText("none");
    }
    else
    {
        ui->labelVid->setText(QString::number(port.vendorIdentifier()));
    }
    // Constructer
    ui->labelManufacturer->setText(port.manufacturer());
    // Location
    ui->labelLocation->setText(port.systemLocation());
}
