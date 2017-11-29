#include "SerialTerminal.h"
#include "ui_SerialTerminal.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>

#include "SerialInterface.h"

SerialTerminal::SerialTerminal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialTerminal)
{
    ui->setupUi(this);
    ui->portView->setModel(&m_portsModel);
    connect(ui->portView, &QListView::clicked, this, &SerialTerminal::on_portSelected);

    connect(&SerialInterface::get(), &SerialInterface::commandReceived, this, &SerialTerminal::on_command);
    connect(&SerialInterface::get(), &SerialInterface::serialDisconnected, this, &SerialTerminal::on_disconnected);

    scanPorts();

    if(SerialInterface::get().isConnected()){
        ui->portConnectedLabel->setText(SerialInterface::get().portInfo().portName());
    }
}

SerialTerminal::~SerialTerminal()
{
    delete ui;
}

void SerialTerminal::on_buttonScan_clicked()
{
    scanPorts();
}

void SerialTerminal::on_buttonConnect_clicked()
{
    const QString portName = m_portsModel.data(ui->portView->currentIndex()).toString();
    const QSerialPortInfo port = getPortByName(portName);
    if(!SerialInterface::get().connectSerial(port)){
        display(tr("<span style=\"font-weight: bold;\">Connexion impossible.</span>"));
    }else{
        display(tr("<span style=\"font-weight: bold;\">Connexion ...</span>"));
         ui->portConnectedLabel->setText(portName);
    }
}

void SerialTerminal::on_buttonDisconnect_clicked()
{
    SerialInterface::get().disconnectSerial();
    ui->portConnectedLabel->clear();
}

void SerialTerminal::on_buttonSend_clicked()
{
    QString text = ui->commandLine->text();
    QByteArray commandData = hexStringToByte(text);

    SerialCommand command;
    bool result = false;
    try
    {
        result = command.readFrom(commandData);
    }
    catch(const std::exception& exept){
        result = false;
        display(tr("Commande invalide (%1)").arg(exept.what()));
        return;
    }

    if(result){
        display(tr("Envoie de [%1]").arg(text));
        SerialInterface::get().pushCommand(command);
    }
    else{
        display("Commande invalide");
    }
}

void SerialTerminal::on_command(const SerialCommand &command)
{
    if(command.data().size() == 0){
        display(tr("Commande %1 reçue sans données.").arg(SerialCommand::typeToString(command.type())));
    }
    else{
        QString stringFromHex;
        stringFromHex.reserve(command.data().size());
        for(int i(0); i < command.data().size(); ++i){
            QChar c = command.data().at(i);
            if(c >= 32 && c <= 126){
                stringFromHex[i]= c;
            }
            else{
                stringFromHex[i] = '.';
            }
        }
        if(command.type() == SerialCommand::Fram)
        {
            QFile out("fram.hex");
            if(!out.open(QIODevice::WriteOnly)){
                qDebug() << "Cant open file";
            }
            else{
                out.write(command.data());
            }
        }
        display(tr("%1 | %2 octets reçus [%3]\n\n").arg(SerialCommand::typeToString(command.type()))
                                              .arg(command.data().size())
                                              .arg(QString(command.data().toHex())));
        ui->textOut->appendPlainText("["+stringFromHex+"]");
    }
}

void SerialTerminal::on_disconnected()
{
    display(tr("<span style=\"font-weight: bold;\">Déconnecté</span>"));
}

QSerialPortInfo SerialTerminal::getPortByName(const QString &name)
{
    const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo port : ports)
    {
        if(port.portName() == name){
            return port;
        }
    }
    return QSerialPortInfo();
}

void SerialTerminal::display(QString text)
{
     QString msg = tr("<span style=\"font-style: italic;\">%1</span> ").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
     msg.append(text);
     ui->textOut->appendHtml(msg);
}

QByteArray SerialTerminal::hexStringToByte(const QString &str)
{
    QByteArray data;

    for(int i(0); i < str.size(); i+=2)
    {
        QString temp;
        temp += str[i];
        if(i+1 < str.size()){
            temp += str[i+1];
        }
        bool ok = false;
        unsigned int n = temp.toUInt(&ok, 16);
        if(ok){
            data.append(static_cast<unsigned char>(n));
        }
    }
    return data;
}

void SerialTerminal::on_portSelected(const QModelIndex &index)
{
    const QString portName = m_portsModel.data(index).toString();
    const QSerialPortInfo port = getPortByName(portName);
    if(!port.isNull()){
        ui->pidLabel->setText(QString::number(port.productIdentifier()));
        ui->vidLabel->setText(QString::number(port.vendorIdentifier()));
        ui->portLabel->setText(portName);
    }
}

void SerialTerminal::scanPorts()
{
    QStringList portsList;
    const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo port : ports)
    {
        portsList << port.portName();
    }
    m_portsModel.setStringList(portsList);
}
