#ifndef SERIALABSTRACTINTERFACE_H
#define SERIALABSTRACTINTERFACE_H

#include <memory>

#include <QObject>
#include <QByteArray>

#include "AbstractOption.h"
#include "AbstractPassword.h"

class SerialAbstractInterface : public QObject
{
    Q_OBJECT
public:
    explicit SerialAbstractInterface(QObject *parent = nullptr);

    virtual void setPassword(int pwdIndex, std::unique_ptr<AbstractPassword> pwd) = 0;
    virtual std::unique_ptr<AbstractPassword> getPassword(int pwdIndex) const  = 0;

    virtual void setOption(std::unique_ptr<AbstractOption> opt) = 0;
    virtual std::unique_ptr<AbstractOption> getOption() const = 0;

    virtual void setMemory(QByteArray mem) = 0;
    virtual QByteArray getMemory() const = 0;

    virtual void setKey(QByteArray key) = 0;
    virtual QByteArray getKey() const = 0;

signals:
    void initComplete();
    void initProgress(float progress);

protected:
    std::unique_ptr<AbstractOption> m_option;
    QList< std::unique_ptr<AbstractPassword> > m_passwords;
    QByteArray m_memory;
    QByteArray m_key;
};

#endif // SERIALABSTRACTINTERFACE_H
