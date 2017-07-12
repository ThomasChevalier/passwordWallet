#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

class QLabel;

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = 0);
    ~ConnectionDialog();

    enum StepResult
    {
        Ok,
        Warning,
        Error,
        Wait
    };

public slots:
    void goToStep(int step);
    void setStepResult(StepResult res);
    void stepComplete(StepResult res);
    void setProgress(int progress, int min, int max); // For busy : 0, 0, 0
    void addMessage(QString msg);
    void allIsComplete();
    void reset();

private:
    void applyResult(QLabel* lbl, StepResult res);

    Ui::ConnectionDialog *ui;
    unsigned m_currentStep;
};

#endif // CONNECTIONDIALOG_H
