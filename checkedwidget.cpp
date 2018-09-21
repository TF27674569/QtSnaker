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

// һ�����ӵĿ��
#define ITEM 20
// ������ �����ĸ���
#define MAXCOUNT ((SIZE-MAGRIN*2)/(ITEM))
// ������ʵ����
#define POSITION(x) (MAGRIN+x*ITEM)


CheckedWidget::CheckedWidget(QWidget *parent)
    :QWidget(parent),
      ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize( QSize( SIZE, SIZE ));

    // ����Ĭ��û�з���
    orientation =STOP;

    // ��ʼλ��
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

    // ָ��һ��Ҫ��ʼ����������жϻ������
    snaker2->next = nullptr;
    snaker1->next = snaker2;
    snaker->next = snaker1;

    // ����һ��ʳ��
    createFood();
    score = 0;
    callback = nullptr;
    isShowLine = false;

    // ����һ���߳�
    thread = new LooperThread;
    // �����ź����
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
    // ˢ��֮ǰ�ж��ߵ��ƶ������Լ��ƶ�
    if(move()){
        // ǿ��ˢ��
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
       case Qt::Key_Q :// ��ͣ
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
    // ��ͼ����
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);


    // ���߽�4����
     painter.drawLine(MAGRIN,MAGRIN,SIZE-MAGRIN,MAGRIN);
     painter.drawLine(MAGRIN,SIZE-MAGRIN,SIZE-MAGRIN,SIZE-MAGRIN);

     painter.drawLine(MAGRIN,MAGRIN,MAGRIN,SIZE-MAGRIN);
     painter.drawLine(SIZE-MAGRIN,MAGRIN,SIZE-MAGRIN,SIZE-MAGRIN);

    if(isShowLine){
        // ���㻭���ĸ���
        int itemCount = MAXCOUNT;
        // ������
        for(int i=1;i<=itemCount;i++){
            int p = MAGRIN+ITEM*i;
            // ������
            painter.drawLine(MAGRIN,p,SIZE-MAGRIN,p);
            // ������
            painter.drawLine(p,MAGRIN,p,SIZE-MAGRIN);
        }
    }
}

void CheckedWidget::drawSnaker()
{
    // ��ͼ����
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
        // ����qt�������ʹ����ֱ�ӿ۵İٶ�
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

        // goto�ؼ��ֵı��
        lable:
        int x = qrand()%MAXCOUNT; // [0-MAXCOUNT]
        int y =qrand()%MAXCOUNT;

        // תΪʵ������
        food.x = POSITION(x);
        food.y = POSITION(y);

        Snaker *judge = snaker;
        //�����ų������ظ�
        while ((judge->next)!=nullptr) {
            if (food.x == judge->x && food.y == judge->y) {
                // �����ظ������־�ֱ������
                goto lable;
            }
            judge = judge->next;
        }
        // ����������һ���Ϸ���ʳ������
}

void CheckedWidget::gameOver()
{
    if(callback!=nullptr){
        callback->onGameStatesCallback(GAME_OVER);
    }
    // ֹͣ�߳�
    thread->exit();
}

bool CheckedWidget::checkGameStatu(int x,int y)
{
    // �ж��Ƿ�ײ��ǽ
    if (x < MAGRIN || x > SIZE-MAGRIN || y < MAGRIN|| y >SIZE-MAGRIN ){
          gameOver();
          return true;
        }

        // 2. ��ͷײ����û
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
            // ��ӡ����
            if(callback!=nullptr){
                callback->onScoreCallback(score);
            }

            //����ʳ���ٴ���
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
        // ��Ϸ����
        return false;
    }

    if(eatFoot(x,y)){
        // ���Ե�����ͷ�ϼ���һ��
        // ����ֱ�Ӵ�ӡ����ȥ��β��
        return true;
     }

      snaker->body = SnakerBody::BODY;
      Snaker* p = snaker;


        // �Ƴ�β��
        while (p != nullptr) {
            if (p->next->next == nullptr) {
                break;
            }
            p = p->next;
        }

        Snaker* temp = p->next->next;
        p->next = nullptr;
        // �ͷ����һ��
        free(temp);
        // �Ϳ����һ��

        // �½�һ�� x y ��ͷ��
        temp = (Snaker*) malloc(sizeof(Snaker));
        temp->x = x;
        temp->y = y;
        temp->next = snaker;
        temp->body = SnakerBody::HEAD;
        snaker = temp;
        return true;
}
