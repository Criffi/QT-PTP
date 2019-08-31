#include "ptpclock.h"
#include <QElapsedTimer>

#if defined(Q_OS_ANDROID)
    #include <QAndroidJniObject>
    #include <QAndroidJniEnvironment>
    #include <QtAndroid>
#elif defined(Q_OS_IOS)
    #include "ios/uptime.h"
#endif


PTPClock::PTPClock(QObject *parent):
    QObject(parent),
    m_offset(0){

}

qint64 PTPClock::getTimestamp(){
    return bootTime() - m_offset;
}

qint64 PTPClock::bootTime(){
#ifdef Q_OS_ANDROID
    return  static_cast<qint64>(QtAndroid::androidActivity().callMethod<jlong>("elapsedRealTime"));
#elif  defined(Q_OS_IOS)
    return Uptime::get();
#else
    return elapsedTimer.msecsSinceReference();
#endif
}

void PTPClock::setOffset(qint64 newOffset){
    m_offset = newOffset;
}

qint64 PTPClock::offset(){
    return m_offset;
}
