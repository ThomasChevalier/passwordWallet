#include "PasswordTabView.h"
#include "ui_PasswordTabView.h"

#include <QDebug>

#include "AddPwdDialog.h"


PasswordTabView::PasswordTabView(DeviceData& data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PasswordTabView),
    m_data(data)
{
    ui->setupUi(this);
    m_model = new QStringListModel(this);
    ui->pwdView->setModel(m_model);
    ui->pwdBox->hide();
}

PasswordTabView::~PasswordTabView()
{
    delete ui;
}

void PasswordTabView::parseData()
{
    if(m_data.memory().isEmpty())
    {
        return;
    }
    if(m_data.passwordNum()>1)
    {
        ui->lblNumPwd->setText(tr("<span style=\"font-size:8pt;\">%1 mots de passe</span>").arg(m_data.passwordNum()));
    }
    else
    {
        ui->lblNumPwd->setText(tr("<span style=\"font-size:8pt;\">%1 mot de passe</span>").arg(m_data.passwordNum()));
    }

    // Construct list of all the password
    QList<Password> all = m_data.allPassword();
    QStringList nameList;
    m_indexToId.clear();

    for(const Password& pwd : all)
    {
        nameList << pwd.name();
        m_indexToId << pwd.id();
    }
    m_model->setStringList(nameList);

}

void PasswordTabView::on_pwdView_activated(const QModelIndex &index)
{
    displayPwd(m_indexToId.at(index.row()));
}

void PasswordTabView::displayPwd(quint8 id)
{
    ui->pwdBox->show();
    Password pwd = m_data.password(id);
    ui->pwdBox->setTitle(pwd.name());
    ui->lblPwd->setText(pwd.password(m_data.key()));
    ui->lblUser->setText(pwd.userName(m_data.key()));
    ui->lblUse->setText(tr("Utilisé %1 fois").arg(pwd.count()));
}

void PasswordTabView::on_buttonAddPwd_clicked()
{
    AddPwdDialog diag;
    if(diag.exec()==QDialog::Accepted)
    {
        // Add password
    }
}
