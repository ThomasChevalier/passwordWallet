#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class SerialTerminal;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionTerminal_triggered();

private:
    Ui::MainWindow *ui;
    SerialTerminal *m_terminal;
};

#endif // MAINWINDOW_H
