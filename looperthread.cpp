#include "looperthread.h"
#include <windows.h>


LooperThread::LooperThread(QObject *parent):QThread(parent)
{

}

void LooperThread::run()
{
    while(true){
        Sleep(100);
        //ÿ�봥��һ���ź�
         emit looperThreadSignals();
    }
}
