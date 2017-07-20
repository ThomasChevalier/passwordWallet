#include "KeyDialog.h"
#include "ui_KeyDialog.h"

#include <QString>
#include <QFileDialog>
#include <QDebug>
#include "Ascii85.h"

Ascii85Validator::Ascii85Validator(QObject *parent):
    QValidator(parent)
{

}

QValidator::State Ascii85Validator::validate(QString &s, int &i) const
{
    Q_UNUSED(i);
    if (s.isEmpty()) {
        return QValidator::Intermediate;
    }

    for(QChar c : s)
    {
        if(c > 'u' || c < '!')
        {
            return QValidator::Invalid;
        }
    }
    if(s.size() < 20)
    {
        return QValidator::Intermediate;
    }
    else if(s.size() > 20)
    {
        return QValidator::Invalid;
    }
    return QValidator::Acceptable;
}

HexValidator::HexValidator(QObject *parent):
    QValidator(parent)
{

}

QValidator::State HexValidator::validate(QString &s, int &i) const
{
    Q_UNUSED(i);
    if(s.isEmpty())
    {
        return QValidator::Intermediate;
    }
    for(QChar c : s)
    {
        c = c.toUpper();
        if(!c.isDigit() && (c<'A'|| c>'F'))
        {
            return QValidator::Invalid;
        }
    }
    if(s.size() < 32)
    {
        return QValidator::Intermediate;
    }
    else if(s.size() > 32)
    {
        return QValidator::Invalid;
    }
    return QValidator::Acceptable;
}

KeyDialog::KeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyDialog)
{
    ui->setupUi(this);
    Ascii85Validator* val = new Ascii85Validator(this);
    ui->ascii85->setValidator(val);
    HexValidator *val_hex = new HexValidator(this);
    ui->hex->setValidator(val_hex);

    connect(this, &QDialog::rejected, this, &KeyDialog::on_rejected);
}

KeyDialog::~KeyDialog()
{
    delete ui;
}

QByteArray KeyDialog::key() const
{
    return m_key;
}

void KeyDialog::on_ascii85Browse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Fichier contenant votre clef"));
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly) && file.size() == 20)
        {
            ui->ascii85->setText(QString::fromUtf8(file.readAll()));
        }
    }
}

void KeyDialog::on_hexBrowse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Fichier contenant votre clef"));
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly) && file.size() == 16)
        {
            QByteArray content = file.readAll();
            QString line;
            for(QByteArray::size_type i(0); i < content.size(); ++i)
            {
                if(static_cast<char>(content[i])==0)
                {
                    line += "00";
                }
                else
                {
                    if(static_cast<char>(content[i])<=0x0F)
                    {
                        line += "0";
                    }
                    line += QString::number(content[i], 16).toUpper();
                }
            }
            ui->hex->setText(line);
        }
    }
}

void KeyDialog::on_ascii85_editingFinished()
{
    quint8 out[16];
    char in[20];
    const QString str = ui->ascii85->text();
    for(QString::size_type i(0); i < str.size(); ++i)
    {
        in[i] = str[i].toLatin1();
    }
    decode_16B(in, out);
    ui->hex->clear();
    QString line;
    for(int i(0); i < 16; ++i)
    {
        if(out[i]==0)
        {
            line += "00";
        }
        else
        {
            if(out[i]<=0x0F)
            {
                line += "0";
            }
            line += QString::number(out[i], 16).toUpper();
        }
    }
    ui->hex->setText(line);
}

void KeyDialog::on_hex_editingFinished()
{
    char out[20];
    encode_16B(reinterpret_cast<quint8*>(QByteArray::fromHex(ui->hex->text().toUtf8()).data()), out);
    ui->ascii85->setText(QString::fromLatin1(out,20));
}

void KeyDialog::on_ascii85_textChanged()
{
    if(ui->ascii85->text().size() != 20 &&
       ui->hex->text().size() != 32)
    {
        ui->ok->setEnabled(false);
    }
    else
    {
        ui->ok->setEnabled(true);
    }
}

void KeyDialog::on_hex_textChanged()
{
    on_ascii85_textChanged();
}

void KeyDialog::on_ok_clicked()
{
    m_key = QByteArray::fromHex(ui->hex->text().toUtf8());
    accept();
}

void KeyDialog::on_cancel_clicked()
{
    reject();
}

void KeyDialog::on_rejected()
{
    m_key = QByteArray();
}
