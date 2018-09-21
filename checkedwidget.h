#ifndef CHECKEDWIDGET_H
#define CHECKEDWIDGET_H

#include <QWidget>
#include  "looperthread.h"
#include <functional>
#include "callback.h"


#define SIZE 400
#define GAME_SUCCESS 1
#define GAME_OVER 0

namespace Ui {
class Widget;
}

// 蛇的身体构造
enum SnakerBody{
    HEAD,BODY,FOOT
};

/**
 * 定义设的身体
 * 使用链表的结构
 */
typedef struct SNAKER {
    SnakerBody body;
    int x;
    int y;
    SNAKER* next;
} Snaker;

/**
 * 食物
 */
typedef struct FOOD {
    int x;
    int y;
} Food;



class CheckedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CheckedWidget(QWidget *parent = nullptr);
    ~CheckedWidget();

private:
     Ui::Widget *ui;
     Snaker* snaker;
     Food food;
     int orientation;
     int score;
     LooperThread * thread;
     Callback* callback;
     bool isShowLine;

public:
     // 设置回调函数
    void setCallback(Callback* callback);

    // 显示线条
    void showLine(bool isShowLine);


signals:


public slots:
     // 线程回调的槽
     void threadCallback();


public:
     // 绘制事件回调
    void paintEvent(QPaintEvent * e);
    // 键盘相应事件回调
    void keyPressEvent(QKeyEvent *);

private:
    // 画背景格子
    void drawBackground();
    // 画蛇身
    void drawSnaker();
    // 画食物
    void drawFood();
    // 创建食物
    void createFood();

    // 游戏结束
    void gameOver();

private:// 检测部分

    // 检测游戏运行状态
    bool checkGameStatu(int x,int y);

    // 是否吃到食物
    bool eatFoot(int x,int y);

    // 移动
    bool move();

    // 改变蛇的形状
    bool change(int x, int y);


};


#endif // CHECKEDWIDGET_H
