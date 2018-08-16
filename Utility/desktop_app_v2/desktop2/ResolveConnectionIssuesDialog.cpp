#include "ResolveConnectionIssuesDialog.h"
#include "ui_ResolveConnectionIssuesDialog.h"

#include <QDateTime>
#include <QProcess>
#include <QCoreApplication>

#include "AskTextDialog.h"
#include "SerialDevice.h"

ResolveConnectionIssuesDialog::ResolveConnectionIssuesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResolveConnectionIssuesDialog)
{
    ui->setupUi(this);
    //launch();
}

ResolveConnectionIssuesDialog::~ResolveConnectionIssuesDialog()
{
    delete ui;
}

void ResolveConnectionIssuesDialog::setError(QSerialPort::SerialPortError error)
{
    msg(tr("L'utilitaire de résolution de problème a été lancé pour l'erreur suivante : %1").arg(error));
}

void ResolveConnectionIssuesDialog::msg(const QString &text)
{
    QString msg = tr("<span style=\"font-style: italic;font-size:small;\">%1</span> ").arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
    msg.append(text);
    ui->infoText->append(msg);
}

void ResolveConnectionIssuesDialog::launch()
{
    ui->infoText->append(tr("<span style=\"font-style: italic;\">Bienvenue dans l'assistant de résolution des problèmes de connexion.<br/>"
                            "Cet assistant va lancer des commandes courantes pour vérifier que votre compte est autorisé à accéder "
                            "au port série et qu'aucun autre logiciel ne gêne votre connexion.<br/></span>"));
    if(SerialDevice::instance()->isConnected()){
        msg(tr("Déconnexion de l'appareil"));
        SerialDevice::instance()->disconnectSerial();
    }

    tryConnect();
}

void ResolveConnectionIssuesDialog::tryConnect()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo port : ports)
    {
        if(port.vendorIdentifier() == 0x03EB && port.productIdentifier() == 0x2044)
        {
            msg(tr("Périphérique trouvé : VID=0x%1|PID=0x%2|Nom=%3").arg(port.vendorIdentifier(),0,16).arg(port.productIdentifier(), 0, 16).arg(port.portName()));
            m_portInfo = port;
            break;
        }
    }
    if(m_portInfo.isNull())
    {
        noPeriphericFound();
        return;
    }

    m_serial.setPort(m_portInfo);
    const bool result = m_serial.open(QSerialPort::ReadWrite);
    if(result)
    {
        msg(tr("Connexion établie.<br/>"
               "<span style=\"color:green;\">Aucun problème détécté.</span>"));
    }
    else
    {
        msg(tr("Échec de la connexion, erreur : \"%1\"").arg(m_serial.errorString()));
        switch(m_serial.error())
        {
        case QSerialPort::PermissionError:
            permissionError();
            break;
        case QSerialPort::ResourceError:
            resourceError();
            break;
        default:
            msg(tr("Problème inconnue."));
            break;
        }
    }
}

void ResolveConnectionIssuesDialog::noPeriphericFound()
{
    msg(tr("Aucun périphérique n'a été détécté. Cela peut provenir d'une connexion "
           "déféctueuse (cable USB mal branché, faux contact dans l'appareil). "
           "Vérifiez également que votre appareil ne soit pas en veille au moment de la connexion à l'ordinateur. "
           "Si c'est le cas appuyez simplement sur un des boutons de l'appareil.<br/>"
           "Il est également possible que votre appareil souffre d'une panne. Dans "
           "ce cas veuillez contacter le service après vente."));
}

void ResolveConnectionIssuesDialog::permissionError()
{
    msg(tr("<span style=\"font-weight:bold;\">Résolution des problèmes relatifs aux permissions.</span>"));

    QProcess proc_who;
    msg(tr("Éxécution de la commande 'whoami'."));
    proc_who.start("whoami");
    if(!proc_who.waitForFinished())
    {
        msg(tr("Échec. Veuillez rentrez votre nom d'utilisateur."));
        AskTextDialog diag(tr("Veuillez renseigner votre nom d'utilisateur"), this);
        diag.exec();
        m_userName = diag.text();
    }
    else
    {
        m_userName = QString(proc_who.readAllStandardOutput());
        m_userName.remove("\n");
        msg(tr("Sortie de 'whoami' : %1").arg(m_userName));
        if(!m_userName.isEmpty())
        {
            msg(tr("Le nom d'utilisateur a été récupéré avec succés."));
        }
        else
        {
            msg(tr("Nom d'utilisateur vide. Veuillez rentrez votre nom d'utilisateur."));
            AskTextDialog diag(tr("Veuillez renseigner votre nom d'utilisateur"), this);
            diag.exec();
            m_userName = diag.text();
        }
    }

    QStringList goodGroups;
    goodGroups << "root" << "dialout"; // Defaults groups

    // Try to know who can access the serial
    QProcess proc_ls;
    msg(tr("Éxécution de la commande 'ls -l /dev/%1'.").arg(m_portInfo.portName()));
    proc_ls.start("ls -l /dev/"+m_portInfo.portName());
    if(proc_ls.waitForFinished())
    {
        QString output = proc_ls.readAllStandardOutput();
        msg(tr("Sortie de 'ls -l /dev/%1' : %2").arg(m_portInfo.portName()).arg(output));
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
        msg(tr("Vous devez appartenir à un des groupes suivant : %1").arg(goodGroups.join(" ")));
    }
    else
    {
        msg(tr("Échec de la commande. Utilisation des groupes par défaut : %1").arg(goodGroups.join(" ")));
    }

    // Read user groups
    QProcess proc_groups;
    msg(tr("Éxécution de la commande 'groups %1'.").arg(m_userName));
    proc_groups.start("groups " +m_userName);
    if(!proc_groups.waitForFinished())
    {
        msg(tr("Échec."));
        severalInstances();
        return;
    }

    QString groups = proc_groups.readAllStandardOutput();
    if(!groups.isEmpty() && groups.at(groups.size()-1)=='\n')
    {
        groups.remove(groups.size()-1,1);
    }
    msg(tr("Sortie de 'groups %1' : %2").arg(m_userName).arg(groups));
    if(groups.isEmpty())
    {
        msg(tr("Mauvais nom d'utilisateur, '%1' n'existe pas").arg(m_userName));
        permissionError(); // Restart
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
            msg(tr("L'utilisateur appartient au groupe autorisé suivant : %1").arg(str));
            belongToGoodGroup = true;
            break;
        }
    }
    if(belongToGoodGroup)
    {
        resourceError();
        severalInstances();
        return;
    }
    else
    {
        msg(tr("<span style=\"color:red;\">L'utilisateur n'appartient à aucun des groupes autorisé.</span>"));
        msg(tr("Essayer d'éxécuter la commande suivante en tant qu'administrateur :<br/>usermod -a -G [groupe] %1<br/>"
                                         "Où [groupe] doit être remplacé par un des groupes suivants : %2<br/>"
                                         "Votre session doit être redémarrée pour que le changement prenne effet.").arg(m_userName).arg(goodGroups.join(" ")));
    }
}

void ResolveConnectionIssuesDialog::resourceError()
{
    msg(tr("<span style=\"font-weight:bold;\">Résolution des problèmes relatifs à l'accés au port.</span>"));

    QProcess proc_lsof;
    msg(tr("Éxécution de la commande 'lsof -FpcL /dev/%1'.").arg(m_portInfo.portName()));
    proc_lsof.start("lsof -FpcL /dev/"+m_portInfo.portName());
    if(!proc_lsof.waitForFinished())
    {
        msg(tr("Échec."));
        severalInstances();
        return;
    }
    else
    {
        QString out = proc_lsof.readAllStandardOutput();
        if(!out.isEmpty() && out[out.size()-1]=='\n')
        {
            out.remove(out.size()-1);
        }
        msg(tr("Sortie de 'lsof -FpcL /dev/%1' : %2").arg(m_portInfo.portName()).arg(out));
        if(out.isEmpty())
        {
            msg(tr("Aucun processus ne semble utiliser le port %1").arg(m_portInfo.portName()));
            msg(tr("L'utilitaire de résolution de problème n'a pas détécté d'erreur "
                   "mais il se peut qu'il ne dispose pas des droits suffisants. "
                   "Vous pouvez éxécuter la commande suivante en tant qu'administrateur "
                   "pour vérifier qu'aucun autre processus n'utilise votre périphérique :<br/>"
                   "lsof /dev/%1<br/>"
                   "La commande suivante peut vous aider à désactiver <strong>ModemManager</strong> :<br/>"
                   "mv /usr/share/dbus-1/system-services/org.freedesktop.ModemManager.service /usr/share/dbus-1/system-services/org.freedesktop.ModemManager.service.disabled").arg(m_portInfo.portName()));
        }
        else
        {
            QString pid;
            QString name;
            QString user;

            for(QString line : out.split("\n"))
            {
                if(line.isEmpty())
                {
                    continue;
                }
                switch (line[0].toLatin1()) {
                case 'p': pid  = line.remove(0, 1); break;
                case 'c': name = line.remove(0, 1); break;
                case 'L': user = line.remove(0, 1); break;
                default: break;
                }
            }
            msg(tr("<span style=\"color:red;\">Il semble que le processus '%1' [PID=%2], appartenant à l'utilisateur '%3' "
                                                       "utilise votre périphérique en ce moment.</span>").arg(name, pid, user));
        }
    }
}

void ResolveConnectionIssuesDialog::severalInstances()
{
    msg(tr("<span style=\"font-weight:bold;\">Recherche des autres applications ouvertes.</span>"));
    QProcess proc_ps;
    msg(tr("Éxécution de la commande 'ps ax'."));
    proc_ps.start("ps ax");
    if(!proc_ps.waitForFinished())
    {
        msg("Échec.");
        return;
    }
    else
    {
        QStringList badProcesses;
        badProcesses << QCoreApplication::applicationName() << "ModemManager";
        QString result = proc_ps.readAllStandardOutput();
        QStringList resultList = result.split('\n');
        QStringList foundBad;

        for(QString line : resultList)
        {
            for(QString badName : badProcesses)
            {
                if(line.contains(badName))
                {
                    QStringList splittedLine = line.split(' ', QString::SkipEmptyParts);
                    if(splittedLine.first() != QString::number(QCoreApplication::applicationPid())){
                        foundBad << badName + "<span style=\"font-size:small;\"> [ PID = " + splittedLine.first() + "]</span>";
                    }
                }
            }

        }
        msg(tr("Les applications suivantes ont été détéctés comme s'éxécutant actuellement sur votre ordinateur : <span style=\"color:red;\">%1</span><br/>"
               "Il est conseillé de les fermer car elles peuvent perturber le bon fonctionnement de celle-ci.").arg(foundBad.join(" ")));
    }

}


