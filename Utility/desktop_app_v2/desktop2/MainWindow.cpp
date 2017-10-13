#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SerialTerminal.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_terminal = new SerialTerminal(this);
    ui->centralWidget->layout()->addWidget(m_terminal);
    m_terminal->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionTerminal_triggered()
{
    m_terminal->show();
}
