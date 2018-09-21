#include "checkedwidget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QColor>
#include <QTime>
#include <QKeyEvent>


#define STOP -1
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define MAGRIN 1

// 一个格子的宽高
#define ITEM 20
// 单方向 上最大的个数
#define MAXCOUNT ((SIZE-MAGRIN*2)/(ITEM))
// 计算真实坐标
#define POSITION(x) (MAGRIN+x*ITEM)


CheckedWidget::CheckedWidget(QWidget *parent)
    :QWidget(parent),
      ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize( QSize( SIZE, SIZE ));

    // 方向默认没有方向
    orientation =STOP;

    // 初始位置
    snaker = (Snaker*)malloc(sizeof(Snaker));
    snaker->body = SnakerBody::HEAD;
    snaker->x = POSITION(10);
    snaker->y = POSITION(11);

    Snaker* snaker1 = (Snaker*)malloc(sizeof(Snaker));
    snaker1->body = SnakerBody::BODY;
    snaker1->x = POSITION(10);
    snaker1->y = POSITION(12);

    Snaker* snaker2 = (Snaker*)malloc(sizeof(Snaker));
    snaker2->body = SnakerBody::BODY;
    snaker2->x = POSITION(10);
    snaker2->y = POSITION(13);

    // 指针一定要初始化否则后面判断会出问题
    snaker2->next = nullptr;
    snaker1->next = snaker2;
    snaker->next = snaker1;

    // 创建一个食物
    createFood();
    score = 0;
    callback = nullptr;
    isShowLine = false;

    // 创建一个线程
    thread = new LooperThread;
    // 关联信号与槽
    connect(thread, SIGNAL(looperThreadSignals()), this, SLOT(threadCallback()));
    thread->start();



}

CheckedWidget::~CheckedWidget()
{
    delete ui;
}

void CheckedWidget::setCallback(Callback* callback)
{
    this->callback = callback;
}


void CheckedWidget::showLine(bool isShowLine)
{
    this->isShowLine = isShowLine;
}

void CheckedWidget::threadCallback()
{
    // 刷新之前判断蛇的移动方向以及移动
    if(move()){
        // 强制刷新
        repaint();
    }
}

void CheckedWidget::paintEvent(QPaintEvent *)
{
    drawBackground();
    drawSnaker();
    drawFood();
}

void CheckedWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
       {
       case Qt::Key_W :
       case Qt::Key_Up :
        if(orientation!=DOWN)
        orientation = UP;
        break;
       case Qt::Key_S :
       case Qt::Key_Down :
         if(orientation!=UP)
        orientation =DOWN;
        break;
       case Qt::Key_A :
       case Qt::Key_Left :
        if(orientation!=RIGHT)
         orientation =LEFT;
        break;
       case Qt::Key_D :
        case Qt::Key_Right :
        if(orientation!=LEFT)
        orientation =RIGHT;
        break;
       case Qt::Key_Q :// 暂停
       orientation =STOP;
        break;
       case Qt::Key_O :
         isShowLine =!isShowLine;
         repaint();
       break;
       }
}

void CheckedWidget::drawBackground()
{
    // 画图工具
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);


    // 画边界4条线
     painter.drawLine(MAGRIN,MAGRIN,SIZE-MAGRIN,MAGRIN);
     painter.drawLine(MAGRIN,SIZE-MAGRIN,SIZE-MAGRIN,SIZE-MAGRIN);

     painter.drawLine(MAGRIN,MAGRIN,MAGRIN,SIZE-MAGRIN);
     painter.drawLine(SIZE-MAGRIN,MAGRIN,SIZE-MAGRIN,SIZE-MAGRIN);

    if(isShowLine){
        // 计算画出的个数
        int itemCount = MAXCOUNT;
        // 画横线
        for(int i=1;i<=itemCount;i++){
            int p = MAGRIN+ITEM*i;
            // 画横线
            painter.drawLine(MAGRIN,p,SIZE-MAGRIN,p);
            // 画竖线
            painter.drawLine(p,MAGRIN,p,SIZE-MAGRIN);
        }
    }
}

void CheckedWidget::drawSnaker()
{
    // 画图工具
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    Snaker * temp = snaker;

    do{
        switch (temp->body) {
            case SnakerBody::HEAD:
                 painter.setBrush(QColor(255,0,0));
//                 painter.drawRoundRect(temp->x,temp->y,ITEM,ITEM,99,99);
                 break;
            case SnakerBody::BODY:
            case SnakerBody::FOOT:
                 painter.setBrush(QColor(0,255,0));
                 break;
        }
//        painter.drawRect(temp->x,temp->y,ITEM,ITEM);
         painter.drawRoundRect(temp->x,temp->y,ITEM,ITEM,60,60);

    }while((temp=temp->next)!=nullptr);
}

void CheckedWidget::drawFood()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(255,0,0));
    painter.drawRoundRect(food.x,food.y,ITEM,ITEM,99,99);

}

void CheckedWidget::createFood()
{
        // 这是qt的随机数使用我直接扣的百度
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

        // goto关键字的标记
        lable:
        int x = qrand()%MAXCOUNT; // [0-MAXCOUNT]
        int y =qrand()%MAXCOUNT;

        // 转为实际坐标
        food.x = POSITION(x);
        food.y = POSITION(y);

        Snaker *judge = snaker;
        //遍历排除蛇身重复
        while ((judge->next)!=nullptr) {
            if (food.x == judge->x && food.y == judge->y) {
                // 遇到重复的数字就直接重来
                goto lable;
            }
            judge = judge->next;
        }
        // 到此生产了一个合法的食物坐标
}

void CheckedWidget::gameOver()
{
    if(callback!=nullptr){
        callback->onGameStatesCallback(GAME_OVER);
    }
    // 停止线程
    thread->exit();
    delete thread;
}

bool CheckedWidget::checkGameStatu(int x,int y)
{
    // 判断是否撞了墙
    if (x < MAGRIN || x > SIZE-MAGRIN || y < MAGRIN|| y >SIZE-MAGRIN ){
          gameOver();
          return true;
        }

        // 2. 蛇头撞蛇身没
        Snaker *p = snaker->next;
        while (p->next!=nullptr) {
            if (x == p->x && y == p->y) {
                gameOver();
                return true;
            }
            p = p->next;
        }

    return false;

}



bool CheckedWidget::eatFoot(int x,int y)
{
    if (x == food.x && y == food.y) {
            snaker->body = SnakerBody::BODY;
            Snaker *_new = (Snaker*) malloc(sizeof(Snaker));
            _new->x = food.x;
            _new->y = food.y;
            _new->body = SnakerBody::HEAD;
            _new->next = snaker;
            snaker = _new;
            score+=10;
            // 打印分数
            if(callback!=nullptr){
                callback->onScoreCallback(score);
            }

            //吃完食物再创建
            createFood();
            return true;
        }
    return false;
}

bool CheckedWidget::move()
{
    switch (orientation) {
        case UP:
            return change(snaker->x, snaker->y - ITEM);
        case DOWN:
            return change(snaker->x, snaker->y + ITEM);
        case LEFT:
            return change(snaker->x - ITEM, snaker->y);
        case RIGHT:
            return change(snaker->x + ITEM, snaker->y);
        default:
            break;
        }
    return false;
}

bool CheckedWidget::change(int x, int y)
{

    if(checkGameStatu(x,y)){
        // 游戏结束
        return false;
    }

    if(eatFoot(x,y)){
        // 被吃掉我在头上加了一个
        // 可以直接打印不用去掉尾部
        return true;
     }

      snaker->body = SnakerBody::BODY;
      Snaker* p = snaker;


        // 移除尾部
        while (p != nullptr) {
            if (p->next->next == nullptr) {
                break;
            }
            p = p->next;
        }

        Snaker* temp = p->next->next;
        p->next = nullptr;
        // 释放最后一个
        free(temp);
        // 滞空最后一个

        // 新建一个 x y 放头部
        temp = (Snaker*) malloc(sizeof(Snaker));
        temp->x = x;
        temp->y = y;
        temp->next = snaker;
        temp->body = SnakerBody::HEAD;
        snaker = temp;
        return true;
}
