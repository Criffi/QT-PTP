#ifndef PTPCLIENT_H
#define PTPCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>

#include "ptpclock.h"

class PTPClient : public QObject
{
    Q_OBJECT
public:
    explicit PTPClient(QObject *parent = nullptr);

    Q_INVOKABLE void requestSync();

public:
    Q_PROPERTY(int port MEMBER m_masterClockPort WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString address MEMBER m_masterClockAddressIp
               WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(bool isValid MEMBER m_isValid NOTIFY isValidChanged)

public:
    void setPort(int newPort);
    void setAddress(QString newAddress);

private:
    void validate();
    void synchronize();

private:
    QUdpSocket   m_udpSocket;
    int          m_masterClockPort;
    QHostAddress m_masterClockAddress;
    QString      m_masterClockAddressIp;

    PTPClock m_clock;

    bool m_isValid;

signals:
    void portChanged();
    void addressChanged();
    void isValidChanged();
    void synchronized(qint64 offset);

};

#endif // PTPCLIENT_H
