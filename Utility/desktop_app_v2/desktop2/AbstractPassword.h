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

    QByteArray ivPwd() const;
    void setIvPwd(const QByteArray &ivPwd);

    QByteArray pwd() const;
    void setPwd(const QByteArray &pwd);

    QByteArray ivUsrName() const;
    void setIvUsrName(const QByteArray &ivUsrName);

    QString userName() const;
    void setUserName(const QString &userName);

    int counter() const;
    void setCounter(int counter);

    static int size();
    virtual bool readFrom(QByteArray& deviceData) = 0;
    virtual QByteArray toByteArray() = 0;

    int prevUse() const;

    int nextUse() const;

    int prevAlpha() const;

    int nextAlpha() const;

protected:

    QString m_name;
    QByteArray m_ivPwd;
    QByteArray m_pwd;
    QByteArray m_ivUsrName;
    QString m_userName;
    int m_counter;
    int m_prevUse;
    int m_nextUse;
    int m_prevAlpha;
    int m_nextAlpha;
};

#endif // ABSTRACTPASSWORD_H
