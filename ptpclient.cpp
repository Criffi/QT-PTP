#include "ptpclient.h"

#include <QDebug>
#include <QNetworkDatagram>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include <QtMath>

PTPClient::PTPClient(QObject *parent):
    QObject(parent),
    m_masterClockPort(0),
    m_masterClockAddress(QHostAddress::Null),
    m_masterClockAddressIp(""),
    m_isValid(false)
{
    m_udpSocket.bind(0);
    m_udpSocket.setSocketOption(QAbstractSocket::LowDelayOption, 1);
    m_masterClockAddress.setAddress(QHostAddress::LocalHost);
}

void PTPClient::setPort(int newPort){
    if(m_masterClockPort != newPort){
        m_masterClockPort = newPort;
        emit portChanged();
        validate();
    }
}

void PTPClient::setAddress(QString newAddress){
    if(newAddress != m_masterClockAddressIp){
        m_masterClockAddressIp = newAddress;
        m_masterClockAddress.setAddress(m_masterClockAddressIp);

        emit addressChanged();
        validate();
    }
}

void PTPClient::validate(){
    bool valid = (m_masterClockPort != 0
                    && !m_masterClockAddress.isNull());

    if(this->m_isValid != valid){
        m_isValid = valid;
        emit isValidChanged();
    }
}

void PTPClient::requestSync(){
    synchronize();
}

void PTPClient::synchronize(){
    validate();

    if(!m_isValid){
        qDebug()<<"Invalid ";
        return;
    }

    qDebug()<<"valid";

    qint64 serverTime1;
    qint64 serverTime2;
    qint64 clientTime1;
    qint64 clientTime2;

    m_udpSocket.writeDatagram(QByteArray::number(m_clock.bootTime()),
                              m_masterClockAddress,
                              m_masterClockPort);

    if(!m_udpSocket.waitForReadyRead(1000)){
        return;
    }

    serverTime1 = m_udpSocket.receiveDatagram().data().toLongLong();
    clientTime1 = m_clock.bootTime();

    m_udpSocket.writeDatagram(QByteArray::number(clientTime1),
                              m_masterClockAddress,
                              m_masterClockPort);

    if(!m_udpSocket.waitForReadyRead(1000)){
        return;
    }

    serverTime2 =  m_udpSocket.receiveDatagram().data().toLongLong();
    clientTime2 = m_clock.bootTime();

    qint64 offset = (clientTime1 - serverTime1 - serverTime2 + clientTime2)/2;

    emit synchronized(offset);
}


