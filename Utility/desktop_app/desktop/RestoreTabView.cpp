#include "RestoreTabView.h"
#include "ui_RestoreTabView.h"

#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

RestoreTabView::RestoreTabView(DeviceData &data, SerialDevice &device, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestoreTabView),
    m_data(data), m_device(device)
{
    ui->setupUi(this);

}

RestoreTabView::~RestoreTabView()
{
    delete ui;
}


void RestoreTabView::on_buttonSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Sauvegarder la mémoire"));
    if(fileName.isEmpty()) {
        return;
    }

    QFile framFile(fileName);
    if(!framFile.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this, tr("Erreur de fichier"), tr("Impossible d'ouvrir le fichier %1 en écriture. Disque plein ?").arg(fileName));
        return;
    }
    framFile.write(m_data.memory());
}

void RestoreTabView::on_buttonRestore_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Restaurer la mémoire"));
    if(fileName.isEmpty()) {
        return;
    }

    QFile framFile(fileName);
    if(!framFile.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, tr("Erreur de fichier"), tr("Impossible d'ouvrir le fichier %1 en lecture. Permission ?").arg(fileName));
        return;
    }
    if(m_device.isConnected())
    {
        m_device.setFram(framFile.readAll());
    }
    m_data.setMemory(framFile.readAll());
}
