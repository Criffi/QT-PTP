#ifndef PTPCLOCK_H
#define PTPCLOCK_H

#include <QObject>
#include <QElapsedTimer>


class PTPClock : public QObject
{
    Q_OBJECT
public:
    explicit PTPClock(QObject *parent = nullptr);

    Q_INVOKABLE qint64 bootTime();
    Q_INVOKABLE qint64 getTimestamp();

    Q_INVOKABLE void   setOffset(qint64 offset);
    Q_INVOKABLE qint64 offset();

private:
    QElapsedTimer elapsedTimer;

    qint64 m_offset;

public slots:
};

#endif // PTPCLOCK_H
