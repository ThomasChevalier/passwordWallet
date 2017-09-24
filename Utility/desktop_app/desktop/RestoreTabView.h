#ifndef RESTORETABVIEW_H
#define RESTORETABVIEW_H

#include <QWidget>

#include "DeviceData.h"
#include "SerialDevice.h.old"

namespace Ui {
class RestoreTabView;
}

class RestoreTabView : public QWidget
{
    Q_OBJECT

public:
    explicit RestoreTabView(DeviceData& data, SerialDevice& device, QWidget *parent = 0);
    ~RestoreTabView();

public slots:
    void on_buttonSave_clicked();
    void on_buttonRestore_clicked();

private:
    Ui::RestoreTabView *ui;
    DeviceData &m_data;
    SerialDevice &m_device;
};

#endif // RESTORETABVIEW_H
