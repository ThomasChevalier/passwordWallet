#ifndef OPTION_1_0_H
#define OPTION_1_0_H

#include "AbstractOption.h"

class Option_1_0 : public AbstractOption
{
public:
    Option_1_0();
    int size() override;
    QByteArray toByteArray() override;
    bool readFrom(QByteArray& deviceData) override;
    bool isValid() const override;

private:
    bool m_valid;
    enum StateRead
    {
        Flags,
        PwdUse,
        PwdAlpha,
        NumPwd,
        EntropyPool,
        MemoryMap,
        BackupSection,
        Done
    };
    StateRead m_state;

    quint8 m_optFlags;
    quint8 m_firstPwdUse;
    quint8 m_firstPwdAlpha;
    quint8 m_numPwd;
    QByteArray m_entropyPool;
    QByteArray m_memoryMap;
    QByteArray m_backupSection;
};

#endif // OPTION_1_0_H
