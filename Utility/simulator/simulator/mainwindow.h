#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpenProject_triggered();

private:
    void launchSimulation();

    Ui::MainWindow *ui;

    QString m_projectPath;
};

#endif // MAINWINDOW_H
