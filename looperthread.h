#ifndef LOOPERTHREAD_H
#define LOOPERTHREAD_H
#include <QThread>

class LooperThread: public QThread
{
    Q_OBJECT
    public:
        explicit LooperThread(QObject *parent = nullptr);

    protected:
       // 线程执行提
        void run();

    signals:
        // 线程信号
        void looperThreadSignals();

};

#endif // LOOPERTHREAD_H
