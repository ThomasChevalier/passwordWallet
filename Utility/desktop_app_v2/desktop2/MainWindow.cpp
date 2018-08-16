#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SerialTerminal.h"
#include "MemoryLayout.h"

#include "ResolveConnectionIssuesDialog.h"

#include "ConnectionWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_widgets(),
    m_current(nullptr)
{
    ui->setupUi(this);

    m_widgets.append(new SerialTerminal(this));
    m_widgets.append(new ConnectionWidget(this));
    m_current = m_widgets.first();

    for(QWidget* w : m_widgets){
        ui->centralWidget->layout()->addWidget(w);
    }

    hide_all();

    m_current->show();

    MemoryLayout::get();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionTerminal_triggered()
{
    hide_all();
    show_widget<SerialTerminal>();
}

void MainWindow::on_actionConnection_triggered()
{
    hide_all();
    show_widget<ConnectionWidget>();
}

void MainWindow::hide_all()
{
    for(QWidget* widget : m_widgets)
    {
        widget->hide();
    }
}
