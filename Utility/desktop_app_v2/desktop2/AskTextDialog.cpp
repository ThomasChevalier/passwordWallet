#include "AskTextDialog.h"
#include "ui_AskTextDialog.h"

AskTextDialog::AskTextDialog(QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskTextDialog)
{
    ui->setupUi(this);
    ui->titleLabel->setText(title);
}

AskTextDialog::~AskTextDialog()
{
    delete ui;
}

QString AskTextDialog::text() const
{
    return ui->inputLine->text();
}
