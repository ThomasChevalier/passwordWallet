#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QTextStream>
#include <iostream>
#include <QDebug>

#include <QList>
#include <QPair>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QDataStream>
#include <QFile>

#include "../../../Code/src/Memory/MemoryLayout.h"

#define COM_OK        (0x00)
#define COM_ERR_NOT_INIT (0x01)
#define COM_INIT      (0x02)
#define COM_END       (0x03)
#define COM_ERR_UNAVAILABLE (0x04)
#define COM_BAD_KEY (0x05)
#define COM_TIMEOUT (0x06)

#define COM_GET_FRAM  (0x10)
#define COM_GET_KEY   (0x11)
#define COM_GET_PARAM (0x12)
#define COM_GET_VERSION (0x13)

#define COM_SET_FRAM  (0x20)
#define COM_SET_KEY   (0x21)

#define COM_FRAM (0x30)
#define COM_KEY (0x31)
#define COM_PARAM (0x32)
#define COM_VERSION (0x33)

typedef QPair<uint32_t, QByteArray> pslice;

bool send(QByteArray data, QSerialPort& serial){
    QTextStream qout(stdout);
    qout << "[send] ";
    for(char c : data){
        QString str = QString::number(c, 16);
        if(str.size()== 1){
            qout << "0" << str;
        }else{
            qout << str;
        }
    }
    qout << endl;
    serial.write(data);
    if(!serial.waitForBytesWritten())
    {
        qout << "[fatal] Writing timed out." << endl;
        return false;
    }else{
        qout << "[info] Send success." << endl;
        return true;
    }
}

bool read(QByteArray& out, int num, QSerialPort& serial)
{
    QTextStream qout(stdout);
    while(out.size() < num){
        if(!serial.waitForReadyRead())
        {
            qout << "[fatal] Reading timed out." << endl;
            return false;
        }
        QByteArray r = serial.readAll();
        qout << "[read] ";
        for(char c : r){
            QString str = QString::number(c, 16);
            if(str.size()== 1){
                qout << "0" << str;
            }else{
                qout << str;
            }
        }
        qout << endl;
        out.append(r);
    }
    return true;
}

void list_ports()
{
    QTextStream qout(stdout);
    const QList<QSerialPortInfo> availables = QSerialPortInfo::availablePorts();
    if(availables.size() == 0){
        qout << "[info] No available port." << endl;
        return;
    }else{
        QString plur;
        if(availables.size()>1){
            plur = "s";
        }
        qout << "[info] " << availables.size() << QString(" available port%1 :").arg(plur) << endl;
    }
    for(QSerialPortInfo info : availables){
        qout << "\t" << info.portName() << QObject::tr(" | PID = %1 | VID = %2 |" ).arg(info.productIdentifier()).arg(info.vendorIdentifier()) << endl;
    }
}

QByteArray get_fram(QSerialPort& serial)
{
    QTextStream qout(stdout);
    QByteArray command(4, 0);
    command[0] = COM_GET_FRAM;
    command[1] = 0;
    command[2] = 0;
    command[3] = 0;
    qout << "[info] Requesting memory." << endl;
    if(!send(command, serial)){
        return QByteArray();
    }

    // Reading response
    QByteArray data;
    int c = 0;
    while(data.size() < 4)
    {
        if(!serial.waitForReadyRead())
        {
            qout << "[fatal] Reading timed out." << endl;
            return QByteArray();
        }
        QByteArray r = serial.readAll();
        qout << "[read] ";
        for(int i(0); i < r.size() && c < 4; ++i){
            ++c;
            QString str = QString::number(r[i], 16);
            if(str.size()== 1){
                qout << "0" << str;
            }else{
                qout << str;
            }
        }
        qout << endl;
        data.append(r);
    }

    if(data[0] != COM_FRAM){
        qout << "[fatal] Unexpected data." << endl;
        return QByteArray();
    }
    quint32 framSize = (data[3] << 16) | (data[2] << 8) | data[1];
    qout << "[info] Memory size is " << framSize << " bytes" << endl;
    data = data.mid(4);
    while(data.size() < framSize)
    {
        if(!serial.waitForReadyRead())
        {
            qout << "[fatal] Reading timed out." << endl;
            return QByteArray();
        }
        data.append(serial.readAll());
        int percent = data.size()*100/framSize;
        std::cout << "\r|";
        for(int i(0); i < 100; ++i)
        {
            if(i<percent){
                std::cout << "#";
            }
            else{
                std::cout << " ";
            }
        }
        std::cout << "| " << percent << "%"  << std::flush;
    }
    qout << "\n[info] Memory received (" << data.size() << " bytes)" << endl;
    if(data.size()!=framSize){
        qout << "[warn] Additionnal bytes received (discarding them): ";
    }
    for(int i(framSize); i < data.size(); ++i){
        QString str = QString::number(data[i], 16);
        if(str.size()== 1){
            qout << "0" << str;
        }else{
            qout << str;
        }
    }
    if(data.size() != framSize){
        qout << endl;
    }
    data.resize(framSize);
    return data;
}

QList< QPair<uint32_t, QByteArray> > diff(QByteArray oldData, QByteArray newData)
{
    if(oldData.size() != newData.size()){
        qDebug() << "mem size mismatch";
        return QList<pslice>();
    }
    // First, make a slice every time bytes are not equal

    QList<pslice> slices;
    for(uint32_t i(0); i < oldData.size(); ++i)
    {
        // Ignore entropy pool
        if(i > OFFSET_ENTROPY_POOL && i < OFFSET_ENTROPY_POOL+SIZE_ENTROPY_POOL){
            continue;
        }
        if(oldData[i] != newData[i])
        {
            // Continue the slice
            if(!slices.isEmpty() && slices.last().second.size() + slices.last().first == i){
                slices.last().second.append(newData[i]);
            }
            // New slice
            else{
                slices.push_back(pslice(i, QByteArray(1, newData[i])));
            }
        }
    }

    // merge slices too close to each other
    const uint32_t threshold = 7; // Optimum threshold
    // No merge possible
    if(slices.size()<=1){
        return slices;
    }
    for(int i(0); i < slices.size()-1;)
    {
        pslice& curr = slices[i];
        pslice& next = slices[i+1];
        const uint32_t delta = next.first - (curr.first+curr.second.size());
        if(delta <= threshold){
            // complete the current slice with oldData
            for(uint32_t i(0); i < delta; ++i){
                curr.second.append(oldData[i+delta]);
            }
            // Append the next slice
            curr.second.append(next.second);
            // Delete the 'next' slice
            slices.removeOne(next);
        }else{
            ++i;
        }
    }
    return slices;
}

bool send_slice(const pslice& slice, QSerialPort& serial)
{
    QTextStream qout(stdout);
    uint32_t pos = slice.first;
    QByteArray data = slice.second;
    if(data.size() > 0xFFFFFF - 3){
        qout << "[info] slice too big, dividing it." << endl;
        bool res = send_slice(pslice(pos, data.mid(0, data.size()/2)), serial);
        if(res && send_slice(pslice(pos+data.size()/2, data.mid(data.size()/2)), serial )){
            return true;
        }else{
            return false;
        }
    }
    else{
        QByteArray command;
        command.resize(1+3+3);
        uint32_t comSize = 3 + data.size();
        command[0] = COM_SET_FRAM;
        command[1] = comSize & 0xFF;
        command[2] = (comSize>>8) & 0xFF;
        command[3] = (comSize>>16) & 0xFF;

        command[4] = pos & 0xFF;
        command[5] = (pos>>8) & 0xFF;
        command[6] = (pos>>16) & 0xFF;

        command.append(data);

        qout << "[send] " << command.size() << " bytes" << endl;
        serial.write(command);

        if(!serial.waitForBytesWritten())
        {
            qout << "[fatal] Writing timed out." << endl;
            return false;
        }
        qout << "[info] Send success." << endl;

        QByteArray rep;
        if(!read(rep, 4, serial)){
            return false;
        }
        else if(rep[0] != COM_OK)
        {
            qout << "[fatal] Device respond with " << static_cast<int>(rep[0]) << endl;
            return false;
        }
        return true;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream qout(stdout);

    QString intro = QObject::tr("transfer_fram by Thomas Chevalier.\n"
                                "(c) 2018 Thomas Chevalier - All rights reserved.\n"
                                "thomasom.chevalier@gmail.com\n");

    QString help = intro + QObject::tr(
                    "Usage : transfer_fram [serialPort] [fileName]\n\t"
                    "serialPort = The name of the port on which the device is connected.\n"
                    "fileName = The file name that contains the memory to write to the device.\n"
                    "-l : list the available ports\n");

    if(app.arguments().size() == 2 && app.arguments().at(1)!="-l")
    {
        qout << "Invalid arguments.\n" << endl;
        qout << help << endl;
        return 1;
    }
    else if(app.arguments().size()!=3 && app.arguments().size()!=2){
        qout << "Invalid arguments.\n" << endl;
        qout << help << endl;
        return 1;
    }
    else{
        qout << intro << endl;
    }

    if(app.arguments().size()==2){
        list_ports();
        return 0;
    }

    QFile file(app.arguments().at(2));
    if(!file.open(QIODevice::ReadOnly)){
        qout << "[fatal] Unable to open \"" << file.fileName() << "\"" << endl;
        return 1;
    }

    const QList<QSerialPortInfo> availables = QSerialPortInfo::availablePorts();
    QSerialPortInfo selected;
    if(availables.size() == 0){
        qout << "[info] No available port." << endl;
        return 1;
    }else{
        qout << "[info] Available ports :" << endl;
    }
    for(QSerialPortInfo info : availables){
        if(info.portName() == app.arguments().at(1)){
            qout << "\t[X] ";
            selected = info;
        }
        else{
            qout << "\t[ ] ";
        }
        qout << info.portName() << QObject::tr(" | PID = %1 | VID = %2 |" ).arg(info.productIdentifier()).arg(info.vendorIdentifier()) << endl;
    }
    if(selected.isNull()){
        qout << "[fatal] Unable to find serial port." << endl;
        return 1;
    }
    QSerialPort serial(selected);
    if(!serial.open(QIODevice::ReadWrite)){
        qout << "[fatal] Unable to open serial port." << endl;
        return 1;
    }
    qout << "[info] Connected to " << selected.portName() << endl;

    QByteArray commandData;
    commandData.resize(4);
    commandData[0] = COM_INIT;
    commandData[1] = 0;
    commandData[2] = 0;
    commandData[3] = 0;
    if(!send(commandData, serial)){
        return 1;
    }

    QByteArray received;
    if(!read(received, 4, serial)){
        return 1;
    }

    if(received.at(0) == COM_OK){
        qout << "[info] Device accepts communication." << endl;
    }else if (received.at(0) == COM_END){
        qout << "[fatal] Access denied." << endl;
        return 1;
    }else{
        qout << "[fatal] Unexpected data." << endl;
        return 1;
    }

    QByteArray mem_device = get_fram(serial);
    qout << "[info] Creating a backup of the memory in .fram.backup" << endl;
    QFile backupFile(".fram.backup");
    if(!backupFile.open(QIODevice::WriteOnly)){
        qout << "[fatal] Unable to create .fram.backup" << endl;
        return 1;
    }
    backupFile.write(mem_device);
    backupFile.close();

    QByteArray mem = file.readAll();

    QList<pslice> slices = diff(mem_device, mem);
    QString plur;
    if(slices.size() == 0){
        qout << "[info] No update required." << endl;
        return 0;
    }else if(slices.size()>1){
        plur = "s";
    }
    uint32_t totalSize = 0;
    for(pslice& s : slices){
        totalSize += s.second.size();
    }
    qout << "[info] Update require to write " << slices.size() << QString(" slice%1").arg(plur) << " totalizing " << totalSize << " bytes." << endl;

    for(pslice& s : slices){
        if(!send_slice(s, serial)){
            return 1;
        }
    }

    qout << "[info] Veririfying memory." << endl;
    QByteArray new_memDevice = get_fram(serial);
    if(mem.size() != new_memDevice.size()){
        qout << "[fatal] Size mismatch." << endl;
        return 1;
    }
    unsigned mismatch = 0;
    for(int i(0); i < mem.size(); ++i){
        // Ignore entropy pool
        if(i >= OFFSET_ENTROPY_POOL && i < OFFSET_ENTROPY_POOL+SIZE_ENTROPY_POOL){
            continue;
        }
        if(mem[i] != new_memDevice[i]){
            qout << QString::number(i, 16) << endl;
            ++mismatch;
        }
    }
    if(mismatch != 0){
        qout << "[fatal] " << mismatch << " bytes are different." << endl;
        return 1;
    }
    qout << "[success] Memory successfuly written" << endl;

    QTimer::singleShot(0, &app, SLOT(quit()));

    return app.exec(); // Run the (empty) application
}
