#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>
#include <QTimer>

#include <QSerialPort>

namespace Ui {
class ConnectionWidget;
}

class ConnectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionWidget(QWidget *parent = 0);
    ~ConnectionWidget();

    void connect();

private slots:
    void on_showButton_clicked();

private:
    void auto_explore();
    void processError(QSerialPort::SerialPortError error);

    Ui::ConnectionWidget *ui;

    QTimer* m_exploreTimer;
};

#endif // CONNECTIONWIDGET_H
