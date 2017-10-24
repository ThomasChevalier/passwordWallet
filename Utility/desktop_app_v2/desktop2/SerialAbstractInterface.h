#ifndef SERIALABSTRACTINTERFACE_H
#define SERIALABSTRACTINTERFACE_H

#include <QObject>
#include <QByteArray>

class SerialAbstractInterface : public QObject
{
    Q_OBJECT
public:
    explicit SerialAbstractInterface(QObject *parent = nullptr);

//    virtual void setPassword(int pwdIndex, Password pwd);
//    virtual Password getPassword(int pwdIndex) const;

//    virtual void setOption(AbstractOption* opt);
//    virtual AbstractOption* getOption() const;

//    virtual void setMemory(QByteArray mem);
//    virtual QByteArray getMemory() const;

//    virtual void setKey(QByteArray key);
//    virtual QByteArray getKey() const;

signals:
    void initComplete();
    void initProgress(float progress);

public slots:
};

#endif // SERIALABSTRACTINTERFACE_H
