#ifndef ASKTEXTDIALOG_H
#define ASKTEXTDIALOG_H

#include <QDialog>

namespace Ui {
class AskTextDialog;
}

class AskTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AskTextDialog(QString title, QWidget *parent = 0);
    ~AskTextDialog();

    QString text() const;

private:
    Ui::AskTextDialog *ui;
};

#endif // ASKTEXTDIALOG_H
