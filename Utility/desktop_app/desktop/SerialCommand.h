#ifndef SERIALCOMMAND_H
#define SERIALCOMMAND_H

#include <QByteArray>

class SerialCommand
{
public:
    enum Type
    {
        InitCommunication,
        EndCommunication,
        GetFram,
        SetFram,
        GetKey,
        SetKey,
        RequestParam,

        None
    };
    SerialCommand(Type type_ = None, const QByteArray& data_ = QByteArray());

    bool readFrom(QByteArray& deviceData);
    bool isValid() const;

    QByteArray toByteArray() const;

    Type type() const;
    QByteArray data() const;

private:
    Type m_type;
    QByteArray m_data;

    enum Read_State
    {
        WaitId,
        WaitSize1,
        WaitSize2,
        WaitData
    };

    Read_State m_state;
    bool m_valid;
};

#endif // SERIALCOMMAND_H
