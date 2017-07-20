#ifndef KEYDIALOG_H
#define KEYDIALOG_H

#include <QDialog>
#include <QValidator>

namespace Ui {
class KeyDialog;
}

class Ascii85Validator : public QValidator
{
public:
    Ascii85Validator(QObject * parent);

    QValidator::State validate(QString &s, int &i) const;
};

class HexValidator : public QValidator
{
public:
    HexValidator(QObject * parent);

    QValidator::State validate(QString &s, int &i) const;
};

class KeyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyDialog(QWidget *parent = 0);
    ~KeyDialog();

    QByteArray key() const;

private slots:
    void on_ascii85Browse_clicked();
    void on_hexBrowse_clicked();

    void on_ascii85_editingFinished();
    void on_hex_editingFinished();

    void on_ascii85_textChanged();
    void on_hex_textChanged();

    void on_ok_clicked();
    void on_cancel_clicked();
    void on_rejected();

private:
    Ui::KeyDialog *ui;
    QByteArray m_key;
};

#endif // KEYDIALOG_H
