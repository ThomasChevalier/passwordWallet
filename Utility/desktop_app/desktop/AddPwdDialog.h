#ifndef ADDPWDDIALOG_H
#define ADDPWDDIALOG_H

#include <QDialog>
#include <QValidator>

namespace Ui {
class AddPwdDialog;
}

class SizeValidator : public QValidator
{
public:
    SizeValidator(int size, QObject * parent);

    QValidator::State validate(QString &s, int &i) const;
private:
    int m_size;
};

class AddPwdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPwdDialog(QWidget *parent = 0);
    ~AddPwdDialog();

    QString name() const;
    QString password() const;
    QString userName() const;

private slots:

    void on_ok_clicked();
    void on_cancel_clicked();

    void on_name_textChanged();
    void on_pwd_textChanged();

private:
    Ui::AddPwdDialog *ui;
};

#endif // ADDPWDDIALOG_H
