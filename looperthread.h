#ifndef LOOPERTHREAD_H
#define LOOPERTHREAD_H
#include <QThread>

class LooperThread: public QThread
{
    Q_OBJECT
    public:
        explicit LooperThread(QObject *parent = nullptr);

    protected:
       // �߳�ִ����
        void run();

    signals:
        // �߳��ź�
        void looperThreadSignals();

};

#endif // LOOPERTHREAD_H
