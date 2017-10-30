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

QByteArray AbstractPassword::data() const
{
    return m_data;
}

void AbstractPassword::setData(const QByteArray &data)
{
    m_data = data;
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
