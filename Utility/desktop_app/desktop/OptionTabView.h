#ifndef OPTIONTABVIEW_H
#define OPTIONTABVIEW_H

#include <QWidget>

#include "DeviceData.h"

namespace Ui {
class OptionTabView;
}

class OptionTabView : public QWidget
{
    Q_OBJECT

public:
    explicit OptionTabView(DeviceData& data, QWidget *parent = 0);
    ~OptionTabView();

private:
    Ui::OptionTabView *ui;
    DeviceData &m_data;
};

#endif // OPTIONTABVIEW_H
