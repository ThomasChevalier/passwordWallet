#ifndef SERIALCOMMAND_H
#define SERIALCOMMAND_H

#include <QByteArray>
#include <QString>

class SerialCommand
{
public:
    enum Type
    {
        Ok                = 0,
        Err_Not_Init      = 1,
        InitCommunication = 2,
        EndCommunication  = 3,

        GetFram           = 0x10,
        GetKey            = 0x11,
        GetParam          = 0x12,

        SetFram           = 0x20,
        SetKey            = 0x21,

        Fram              = 0x30,
        Key               = 0x31,
        Param             = 0x32,

        None
    };
    SerialCommand(Type type_ = None, const QByteArray& data_ = QByteArray());

    bool readFrom(QByteArray& deviceData);
    bool isValid() const;

    QByteArray toByteArray() const;

    Type type() const;
    QByteArray data() const;

    static QString typeToString(Type t);

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
