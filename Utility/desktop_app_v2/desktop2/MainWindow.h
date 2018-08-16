#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

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
    void on_actionTerminal_triggered();
    void on_actionConnection_triggered();

private:
    Ui::MainWindow *ui;

    void hide_all();

    template <typename T>
    void show_widget()
    {
        for(int i(0); i < m_widgets.size(); ++i){
            QWidget* w = m_widgets[i];
            if(qobject_cast<T*>(w) != nullptr){
                w->show();
                return;
            }
        }
    }


    QList<QWidget*> m_widgets;
    QWidget* m_current;
};

#endif // MAINWINDOW_H
