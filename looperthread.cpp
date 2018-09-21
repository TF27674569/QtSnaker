#include "looperthread.h"
#include <windows.h>


LooperThread::LooperThread(QObject *parent):QThread(parent)
{

}

void LooperThread::run()
{
    while(true){
        Sleep(100);
        //每秒触发一次信号
         emit looperThreadSignals();
    }
}
