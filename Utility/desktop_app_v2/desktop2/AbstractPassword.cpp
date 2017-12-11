#include "AbstractPassword.h"

AbstractPassword::AbstractPassword()
{

}

QString AbstractPassword::name() const
{
    return m_name;
}

void AbstractPassword::setName(const QString &name)
{
    m_name = name;
}

QByteArray AbstractPassword::ivPwd() const
{
    return m_ivPwd;
}

void AbstractPassword::setIvPwd(const QByteArray &ivPwd)
{
    m_ivPwd = ivPwd;
}

QByteArray AbstractPassword::pwd() const
{
    return m_pwd;
}

void AbstractPassword::setPwd(const QByteArray &pwd)
{
    m_pwd = pwd;
}

QByteArray AbstractPassword::ivUsrName() const
{
    return m_ivUsrName;
}

void AbstractPassword::setIvUsrName(const QByteArray &ivUsrName)
{
    m_ivUsrName = ivUsrName;
}

QString AbstractPassword::userName() const
{
    return m_userName;
}

void AbstractPassword::setUserName(const QString &userName)
{
    m_userName = userName;
}

int AbstractPassword::counter() const
{
    return m_counter;
}

void AbstractPassword::setCounter(int counter)
{
    m_counter = counter;
}

int AbstractPassword::prevUse() const
{
    return m_prevUse;
}

int AbstractPassword::nextUse() const
{
    return m_nextUse;
}

int AbstractPassword::prevAlpha() const
{
    return m_prevAlpha;
}

int AbstractPassword::nextAlpha() const
{
    return m_nextAlpha;
}
