#ifndef PASSWORD_H
#define PASSWORD_H

#include <QString>
#include <QByteArray>

class Password
{
public:
    Password(quint8 id, QByteArray data);

    quint8 prevPwdUse() const;
    quint8 nextPwdUse() const;
    quint8 prevPwdAlpha() const;
    quint8 nextPwdAlpha() const;

    quint16 count() const;
    QString name() const;
    QString password(QByteArray key) const;
    QString userName(QByteArray key) const;
    quint8 id() const;
    const QByteArray &data() const;

    void setCount(quint16 c);
    bool setName(const QString& name);
    bool setPassword(QString pwd, QByteArray key);
    bool setUserName(QString usr, QByteArray key);

    void setPrevPwdUse(uint8_t val);
    void setNextPwdUse(uint8_t val);
    void setPrevPwdAlpha(uint8_t val);
    void setNextPwdAlpha(uint8_t val);


private:
    quint8 m_id;
    QByteArray m_data;
};

#endif // PASSWORD_H
