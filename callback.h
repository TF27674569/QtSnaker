#ifndef CALLBACK_H
#define CALLBACK_H

class Callback
{
 public:
    virtual void onScoreCallback(int score) = 0;

    virtual void onGameStatesCallback(int status) = 0;
};


#endif // CALLBACK_H
