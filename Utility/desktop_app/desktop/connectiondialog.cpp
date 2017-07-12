#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include <QLabel>

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog),
    m_currentStep(0)
{
    ui->setupUi(this);
    setWindowTitle(tr("Connexion"));
    ui->button_less->hide();
    ui->text->hide();
    ui->button_close->hide();
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::goToStep(int step)
{
    m_currentStep = step;
   if(step==0)
   {
       ui->img_param->setEnabled(false);
       ui->img_key->setEnabled(false);
       ui->img_content->setEnabled(false);
   }
   else if(step == 1)
   {
       ui->img_key->setEnabled(false);
       ui->img_content->setEnabled(false);
   }
   else if(step==2)
   {
       ui->img_content->setEnabled(false);
   }
}

void ConnectionDialog::setStepResult(ConnectionDialog::StepResult res)
{
    switch (m_currentStep) {
    case 0:
        applyResult(ui->img_search, res);
        break;
    case 1:
        applyResult(ui->img_param, res);
        break;
    case 2:
        applyResult(ui->img_key, res);
        break;
    case 3:
        applyResult(ui->img_content, res);
        break;
    default:
        break;
    }
}

void ConnectionDialog::stepComplete(ConnectionDialog::StepResult res)
{
    setStepResult(res);
    ++m_currentStep;
    setStepResult(Wait);
    if(m_currentStep==1)
    {
        ui->img_param->setEnabled(true);
    }
    if(m_currentStep==2)
    {
        ui->img_key->setEnabled(true);
    }
    if(m_currentStep == 3)
    {
        ui->img_content->setEnabled(true);
    }
}

void ConnectionDialog::setProgress(int progress, int min, int max)
{
    ui->progressBar->setMinimum(min);
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(progress);
}

void ConnectionDialog::addMessage(QString msg)
{
    ui->text->appendPlainText(msg);
}

void ConnectionDialog::allIsComplete()
{
    ui->label_status->setText(tr("<strong>Connexion établie</strong>"));
    ui->button_close->show();
    ui->progressBar->hide();
}

void ConnectionDialog::reset()
{
    ui->label_status->setText(tr("<strong>Connexion au périphérique en cours</strong>"));
    ui->img_search->setPixmap(QPixmap("wait.png"));
    ui->img_param->setPixmap(QPixmap("wait.png"));
    ui->img_key->setPixmap(QPixmap("wait.png"));
    ui->img_content->setPixmap(QPixmap("wait.png"));
    ui->progressBar->show();
    ui->button_close->hide();
    ui->button_less->hide();
    ui->button_more->show();
    ui->text->hide();
}

void ConnectionDialog::applyResult(QLabel *lbl, ConnectionDialog::StepResult res)
{
    lbl->setEnabled(true);
    switch (res) {
    case Ok:
        lbl->setPixmap(QPixmap("ok.png"));
        break;
    case Warning:
        lbl->setPixmap(QPixmap("warning.png"));
        break;
    case Error:
        lbl->setPixmap(QPixmap("ko.png"));
        break;
    case Wait:
        lbl->setPixmap(QPixmap("wait.png"));
    default:
        break;
    }
}
