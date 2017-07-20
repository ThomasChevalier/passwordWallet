#ifndef PASSWORDTABVIEW_H
#define PASSWORDTABVIEW_H

#include <QWidget>
#include <QStringListModel>
#include <QList>

#include "DeviceData.h"

namespace Ui {
class PasswordTabView;
}

class PasswordTabView : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordTabView(DeviceData& data, QWidget *parent = 0);
    ~PasswordTabView();

    void parseData();

private slots:
    void on_pwdView_activated(const QModelIndex &index);
    void on_buttonAddPwd_clicked();

private:

    void displayPwd(quint8 id);

    QList<quint8> m_indexToId;

    Ui::PasswordTabView *ui;
    DeviceData &m_data;

    QStringListModel *m_model;
};

#endif // PASSWORDTABVIEW_H
