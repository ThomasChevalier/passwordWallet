#include "AddPwdDialog.h"
#include "ui_AddPwdDialog.h"

SizeValidator::SizeValidator(int size, QObject *parent):
    QValidator(parent), m_size(size)
{

}

QValidator::State SizeValidator::validate(QString &s, int &i) const
{
    Q_UNUSED(i);
    if (s.size()<=m_size) {
        return QValidator::Acceptable;
    }
    else
    {
        return QValidator::Invalid;
    }
}

AddPwdDialog::AddPwdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPwdDialog)
{
    ui->setupUi(this);

    SizeValidator* val32 = new SizeValidator(32, this);
    ui->name->setValidator(val32);
    ui->pwd->setValidator(val32);
    SizeValidator* val64 = new SizeValidator(64, this);
    ui->usrName->setValidator(val64);
}

AddPwdDialog::~AddPwdDialog()
{
    delete ui;
}

QString AddPwdDialog::name() const
{
    return ui->name->text();
}

QString AddPwdDialog::password() const
{
    return ui->pwd->text();
}

QString AddPwdDialog::userName() const
{
    return ui->usrName->text();
}

void AddPwdDialog::on_ok_clicked()
{
    accept();
}

void AddPwdDialog::on_cancel_clicked()
{
    reject();
}

void AddPwdDialog::on_name_textChanged()
{
    if(ui->name->text().isEmpty() || ui->pwd->text().isEmpty())
    {
        ui->ok->setEnabled(false);
    }
    else
    {
        ui->ok->setEnabled(true);
    }
}

void AddPwdDialog::on_pwd_textChanged()
{
    on_name_textChanged();
}
