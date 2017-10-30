#ifndef ABSTRACTPASSWORD_H
#define ABSTRACTPASSWORD_H

#include <QByteArray>
#include <QString>

class AbstractPassword
{
public:
    AbstractPassword();

    QString name() const;
    void setName(const QString &name);

    QByteArray data() const;
    void setData(const QByteArray &data);

    QString userName() const;
    void setUserName(const QString &userName);

    int counter() const;
    void setCounter(int counter);

    virtual int size() = 0;
    virtual QByteArray toByteArray() = 0;

protected:
    QString m_name;
    QByteArray m_data;
    QString m_userName;
    int m_counter;
};

#endif // ABSTRACTPASSWORD_H
