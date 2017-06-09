#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenProject_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a project"));
    if(dir.isEmpty())
    {
        return;
    }
    m_projectPath = dir;
    launchSimulation();
}

static void recurseAddDir(QDir d, QStringList & list) {

    QStringList qsl = d.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    for(QString file : qsl)
    {
        QFileInfo finfo(QString("%1/%2").arg(d.path()).arg(file));
        if (finfo.isSymLink())
        {
            return;
        }
        if (finfo.isDir())
        {
            QDir sd(finfo.filePath());
            recurseAddDir(sd, list);
        }
        else
        {
            QStringList whiteList;
            whiteList << "c" << "h" << "cpp" << "hpp";
            if(whiteList.contains(finfo.suffix(), Qt::CaseInsensitive))
            {
                list << QDir::toNativeSeparators(finfo.filePath());
            }
        }
    }
}

void MainWindow::launchSimulation()
{
    // Create simulation directory
    QDir simulDir(QCoreApplication::applicationDirPath() + "/simulation");

    simulDir.removeRecursively();
    QDir appDir(QCoreApplication::applicationDirPath());
    if(!appDir.mkdir("simulation"))
    {
        QMessageBox::critical(this, tr("Cannot create directory"), tr("Error : cannot create the directory \"%1\"").arg(simulDir.dirName()));
        return;
    }

    QDir projDir(m_projectPath);
    QStringList files;
    // Get all files from all direcoty of projDir
    recurseAddDir(projDir, files);
    for(QString file : files)
    {
        QDir filePath(QFileInfo(file).absolutePath());
        if(!filePath.exists())
        {
            qDebug() << filePath.absolutePath() << " does not exist";
            filePath.mkpath(filePath.absolutePath());
        }
        QFile::copy(file, QString(file).replace(m_projectPath, simulDir.path()));
    }
}
