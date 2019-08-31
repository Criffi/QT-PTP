#ifndef PTPSERVER_H
#define PTPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include "qptp/ptpclock.h"

class PTPServer : public QObject
{
    Q_OBJECT
public:
    explicit PTPServer(QObject *parent = nullptr);

public:
    Q_PROPERTY(int port READ port NOTIFY portChanged)
    Q_PROPERTY(QString address READ address NOTIFY ipAddressChanged)

public:
    int port();
    QString address();

private:
    QUdpSocket  m_udpSocket;
    PTPClock    m_clock;

private slots:
    void onRequestReceived();

signals:
    void ipAddressChanged();
    void portChanged();

};

#endif // PTPSERVER_H
