#include "port.h"
//#include <QDebug>
//#include <unistd.h>
Port::Port(QObject *parent) :
    QObject(parent)
{
}

Port::~Port()
{
//    qDebug("By in Thread!");
    emit finished_Port();
}

void Port :: process_Port(){
    connect(&thisPort,SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(&thisPort, SIGNAL(readyRead()),this,SLOT(ReadInPort()));
}

void Port :: Write_Settings_Port(QString name, int baudrate){
    SettingsPort.name = name;
    SettingsPort.baudRate = (QSerialPort::BaudRate) baudrate;
}

void Port :: ConnectPort(void){//
    thisPort.setPortName(SettingsPort.name);
    if (thisPort.open(QIODevice::ReadWrite)) {
        if (thisPort.setBaudRate(SettingsPort.baudRate))

        {
            if (thisPort.isOpen()){
                error_((SettingsPort.name+ " >> Открыт!\r").toLocal8Bit());
            }
        } else {
            thisPort.close();
            error_(thisPort.errorString().toLocal8Bit());
          }
    } else {
        thisPort.close();
        error_(thisPort.errorString().toLocal8Bit());
    }
}

void Port::handleError(QSerialPort::SerialPortError error)//
{
    if ( (thisPort.isOpen()) && (error == QSerialPort::ResourceError)) {
        error_(thisPort.errorString().toLocal8Bit());
        DisconnectPort();
    }
}

void  Port::DisconnectPort(){
    if(thisPort.isOpen()){
        thisPort.close();
        error_(SettingsPort.name.toLocal8Bit() + " >> Закрыт!\r");
    }
//    timer->stop();
}
//ot tuta kovuratji!!!
void Port :: WriteToPort(QByteArray data){
    if(thisPort.isOpen()){
        thisPort.write(data);
    }
}
//
void Port :: ReadInPort(){
    QByteArray data;

    data.append(thisPort.readAll());

    outPort(data);
    usleep(100000);

    //((QString)(adr.toInt())).toLatin1().toHex()
}
