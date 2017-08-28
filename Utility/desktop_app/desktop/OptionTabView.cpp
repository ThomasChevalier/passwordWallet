#include "OptionTabView.h"
#include "ui_OptionTabView.h"

OptionTabView::OptionTabView(DeviceData &data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionTabView),
    m_data(data)
{
    ui->setupUi(this);
}

OptionTabView::~OptionTabView()
{
    delete ui;
}
