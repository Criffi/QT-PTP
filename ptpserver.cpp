#include "ptpserver.h"

#include <QNetworkDatagram>
#include <QDebug>

PTPServer::PTPServer(QObject *parent):
    QObject(parent)
{
    m_udpSocket.bind(0);
    m_udpSocket.setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect(&m_udpSocket, &QUdpSocket::readyRead,
            this, &PTPServer::onRequestReceived);
}


int PTPServer::port(){
    return m_udpSocket.localPort();
}

QString PTPServer::address(){
    return m_udpSocket.localAddress().toString();
}

void PTPServer::onRequestReceived(){
    while (m_udpSocket.hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket.receiveDatagram();
        m_udpSocket.writeDatagram(QByteArray::number(m_clock.bootTime()),
                                   datagram.senderAddress(),
                                   datagram.senderPort());
    }
}

