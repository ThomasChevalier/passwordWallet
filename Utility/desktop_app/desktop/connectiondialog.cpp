#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include <QLabel>
#include <QSerialPortInfo>
#include <QProcess>
#include <QDebug>

#define RESULT_OK_IMG QString("ok.png")
#define RESULT_WARNING_IMG QString("warning.png")
#define RESULT_ERROR_IMG QString("ko.png")
#define RESULT_WAIT_IMG QString("wait.png")

#define MAX_CONNECTION_ATTEMPT_BEFORE_ERROR (10)

ConnectionDialog::ConnectionDialog(SerialDevice &device, DeviceData &data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog),
    m_connectionAttempt(0),
    m_currentState(Connecting),
    m_device(device),
    m_data(data)
{
    // User interface
    ui->setupUi(this);
    ui->button_less->hide();
    ui->text->hide();
    ui->button_close->hide();

    ui->widgetIssueConnectionLinux->hide();

    ui->img_search->setEnabled(true);

    adjustSize();

    // Connection
    m_deviceConnections << connect(&m_device, &SerialDevice::disconnected,        this, &ConnectionDialog::on_disconnected);
    m_deviceConnections << connect(&m_device, &SerialDevice::paramReceived,       this, &ConnectionDialog::on_paramReceived);
    m_deviceConnections << connect(&m_device, &SerialDevice::framReceived,        this, &ConnectionDialog::on_framReceived);
    m_deviceConnections << connect(&m_device, &SerialDevice::keyReceived,         this, &ConnectionDialog::on_keyReceived);
    m_deviceConnections << connect(&m_device, &SerialDevice::framReceiveProgress, this, &ConnectionDialog::on_framReceiveProgress);

    m_exploreTimer = new QTimer(this);
    tryConnect();
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
    for(QMetaObject::Connection con : m_deviceConnections)
    {
        disconnect(con);
    }
}

void ConnectionDialog::addMessage(QString msg)
{
    ui->text->append(msg);
}

void ConnectionDialog::on_disconnected()
{
    addMessage(tr("Périphérique déconnecté."));
    ui->img_content->setEnabled(false);
    ui->img_key->setEnabled(false);
    ui->img_param->setEnabled(false);
    ui->img_search->setEnabled(true);
    ui->img_search->setPixmap(QPixmap(RESULT_WAIT_IMG));

    switch (m_currentState) {
    case WaitingParam:
        ui->img_param->setPixmap(QPixmap(RESULT_ERROR_IMG));
        break;
    case WaitingKey:
        ui->img_param->setPixmap(QPixmap(RESULT_ERROR_IMG));
        break;
    case WaitingFram:
        ui->img_param->setPixmap(QPixmap(RESULT_ERROR_IMG));
    case Complete:
        ui->label_status->setText(tr("<strong>Connexion au périphérique en cours</strong>"));
    default:
        break;
    }

    tryConnect();
}

void ConnectionDialog::on_framReceived(const QByteArray &fram)
{
    m_currentState = Complete;

    m_data.setDeviceFram(fram);
    ui->img_content->setPixmap(QPixmap(RESULT_OK_IMG));
    addMessage(tr("Mémoire reçue"));
    ui->label_status->setText(tr("<strong>Connexion établie</strong>"));
    ui->button_close->show();
    ui->progressBar->hide();
}

void ConnectionDialog::on_keyReceived(const QByteArray &key)
{
    m_currentState = WaitingFram;

    m_data.setDeviceKey(key);
    ui->img_key->setPixmap(RESULT_OK_IMG);
    addMessage(tr("Clef de chiffrement reçue"));

    ui->img_content->setPixmap(RESULT_WAIT_IMG);
    ui->img_content->setEnabled(true);

    m_device.requestFramDump(m_data.getDeviceFramSize());
}

void ConnectionDialog::on_paramReceived(const QByteArray &param)
{
    m_currentState = WaitingKey;

    addMessage(tr("Paramètres reçues"));

    ui->img_param->setPixmap(QPixmap(RESULT_OK_IMG));
    m_data.setParameter(param);
    addMessage(tr("Taille de la mémoire : %1").arg(m_data.getDeviceFramSize()));

    ui->img_key->setPixmap(QPixmap(RESULT_WAIT_IMG));
    ui->img_key->setEnabled(true);
    m_device.requestKey();
}

void ConnectionDialog::on_framReceiveProgress(qint64 received)
{
    if(received>6000)
    {
        m_device.end();
    }
    ui->progressBar->setMaximum(m_data.getDeviceFramSize());
    ui->progressBar->setValue(received);
}

void ConnectionDialog::on_issueLinuxEnterUsrName_clicked()
{
    resolveConnectionIssue(ui->issueLinuxUsrName->text());
}


void ConnectionDialog::tryConnect()
{
    m_currentState = Connecting;
    m_connectionAttempt = 0;

    m_exploreTimer->stop();
    connect(m_exploreTimer, &QTimer::timeout, this, &ConnectionDialog::explorePorts, Qt::UniqueConnection);
    m_exploreTimer->start(1000);
    explorePorts(); // Direct start
}

void ConnectionDialog::explorePorts()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo port : ports)
    {
        if(port.vendorIdentifier() == DeviceData::vendorIdentifier() && port.productIdentifier() == DeviceData::productIdentifier())
        {
            addMessage(tr("Périphérique trouvé:VID=0x%1|PID=0x%2|Nom=%3").arg(port.vendorIdentifier(),0,16).arg(port.productIdentifier(), 0, 16).arg(port.portName()));
            connectDevice(port);
        }
    }
}

void ConnectionDialog::connectDevice(QSerialPortInfo &info)
{
    ++m_connectionAttempt;

    //lookForConnectionOn(info);

    if(m_device.connectSerial(info))
    {
        m_exploreTimer->stop();
        m_connectionAttempt = 0;
        addMessage(tr("Connecté avec succés"));
        ui->img_search->setPixmap(QPixmap(RESULT_OK_IMG));
        getParameter();
    }
    else
    {
        if(m_connectionAttempt>=MAX_CONNECTION_ATTEMPT_BEFORE_ERROR)
        {
            m_exploreTimer->stop();
            m_connectionAttempt = 0;
            addMessage(tr("Connexion impossible"));
            ui->img_search->setPixmap(RESULT_ERROR_IMG);
            resolveConnectionIssue();
        }
        else
        {
            addMessage(tr("Connexion refusée. Nouvel essai..."));
        }
    }
}

void ConnectionDialog::resolveConnectionIssue(QString userName)
{
    #if defined(Q_OS_LINUX)
    ui->widgetIssueConnectionLinux->show();
    adjustSize();

    if(m_device.error() == QSerialPort::PermissionError)
    {
        bool needUsrName = false;

        QString userNameWhoami;
        if(userName.isEmpty())
        {
            QProcess proc_who;
            addMessage(tr("Éxécution de la commande 'whoami'."));
            proc_who.start("whoami");
            if(!proc_who.waitForFinished())
            {
                needUsrName = true;
                addMessage(tr("Échec de la commande."));
            }
            else
            {
                userNameWhoami = QString(proc_who.readAllStandardOutput());
                userNameWhoami.remove("\n");
                addMessage(tr("Sortie de 'whoami' : %1").arg(userNameWhoami));
                if(!userNameWhoami.isEmpty())
                {
                    addMessage(tr("Le nom d'utilisateur a été récupéré avec succés."));
                    ui->widgetAskUsrName->hide();
                    userName = userNameWhoami;
                }
                needUsrName = userNameWhoami.isEmpty();
            }
            if(needUsrName || userName.isEmpty())
            {
                ui->widgetAskUsrName->show();
                adjustSize();
                return;
            }
            else
            {
                userName = userNameWhoami;
            }
        }
        else
        {
            ui->widgetAskUsrName->hide();
        }

        QStringList goodGroups;
        goodGroups << "root" << "dialout"; // Defaults groups

        // Try to know who can access the serial
        QProcess proc_ls;
        addMessage(tr("Éxécution de la commande 'ls -l /dev/%1'.").arg(m_device.portName()));
        proc_ls.start("ls -l /dev/"+m_device.portName());
        if(proc_ls.waitForFinished())
        {
            QString output = proc_ls.readAllStandardOutput();
            addMessage(tr("Sortie de 'ls -l /dev/%1' : %2").arg(m_device.portName()).arg(output));
            QStringList outList = output.split(" ");
            outList.removeFirst(); // Remove chmod info
            outList.removeFirst(); // Remove the number of file in dir
            goodGroups.clear();
            for(QString str : outList)
            {
                if(str.isEmpty())
                {
                    continue;
                }
                if(str.at(0).isDigit())
                {
                    break;
                }
                goodGroups << str;
            }
            addMessage(tr("Vous devez appartenir à un des groupes suivant : %1").arg(goodGroups.join(" ")));
        }
        else
        {
            addMessage(tr("Échec de la commande. Utilisation des groupes par défaut : %1").arg(goodGroups.join(" ")));
        }

        // Read user groups
        QProcess proc_groups;
        addMessage(tr("Éxécution de la commande 'groups %1'.").arg(userName));
        proc_groups.start("groups " +userName);
        if(!proc_groups.waitForFinished())
        {
            addMessage(tr("Échec de la commande."));
            ui->issueLinuxStatus->setText(tr("Si plusieurs instances de cette application sont ouvertes alors la communication avec l'appareil risque de mal fonctionner."));
            return;
        }

        QString groups = proc_groups.readAllStandardOutput();
        if(!groups.isEmpty() && groups.at(groups.size()-1)=='\n')
        {
            groups.remove(groups.size()-1,1);
        }
        addMessage(tr("Sortie de 'groups %1' : %2").arg(userName).arg(groups));
        if(groups.isEmpty())
        {
            addMessage(tr("Mauvais nom d'utilisateur, '%1' n'existe pas").arg(userName));
            ui->widgetAskUsrName->show();
            ui->issueLinuxStatus->setText(tr("Nom d'utilisateur incorrect."));
            return;
        }

        // Remove the string "user_name :"
        QStringList realGroups = groups.split(" ").mid(2);
        bool belongToGoodGroup = false;
        // Search if the user belong to a group that have a good permission
        for(QString str : realGroups)
        {
            if(goodGroups.contains(str))
            {
                addMessage(tr("L'utilisateur appartient au groupe autorisé suivant : %1").arg(str));
                belongToGoodGroup = true;
                break;
            }
        }
        if(belongToGoodGroup)
        {
            ui->issueLinuxStatus->setText(tr("Si plusieurs instances de cette application sont ouvertes alors la communication avec l'appareil risque de mal fonctionner."));
        }
        else
        {
            addMessage(tr("L'utilisateur n'appartient à aucun des groupes autorisé."));
            ui->issueLinuxStatus->setText(tr("Essayer d'éxécuter la commande suivante en tant qu'administrateur :<br/>usermod -a -G [groupe] %1<br/>"
                                             "Où [groupe] doit être remplacé par un des groupes suivants : %2<br/>"
                                             "Votre session doit être rédemarrée pour que le changement prenne effet.").arg(userName).arg(goodGroups.join(" ")));
        }
    }


    #endif
}

void ConnectionDialog::getParameter()
{
    m_currentState = WaitingParam;

    ui->img_param->setPixmap(QPixmap(RESULT_WAIT_IMG));
    ui->img_param->setEnabled(true);
    // Wait for the inital message to be send
    while(m_device.busy()){QCoreApplication::processEvents();}
    m_device.requestParam();
}

