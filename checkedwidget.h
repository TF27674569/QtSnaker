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

// �ߵ����幹��
enum SnakerBody{
    HEAD,BODY,FOOT
};

/**
 * �����������
 * ʹ������Ľṹ
 */
typedef struct SNAKER {
    SnakerBody body;
    int x;
    int y;
    SNAKER* next;
} Snaker;

/**
 * ʳ��
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
     // ���ûص�����
    void setCallback(Callback* callback);

    // ��ʾ����
    void showLine(bool isShowLine);


signals:


public slots:
     // �̻߳ص��Ĳ�
     void threadCallback();


public:
     // �����¼��ص�
    void paintEvent(QPaintEvent * e);
    // ������Ӧ�¼��ص�
    void keyPressEvent(QKeyEvent *);

private:
    // ����������
    void drawBackground();
    // ������
    void drawSnaker();
    // ��ʳ��
    void drawFood();
    // ����ʳ��
    void createFood();

    // ��Ϸ����
    void gameOver();

private:// ��ⲿ��

    // �����Ϸ����״̬
    bool checkGameStatu(int x,int y);

    // �Ƿ�Ե�ʳ��
    bool eatFoot(int x,int y);

    // �ƶ�
    bool move();

    // �ı��ߵ���״
    bool change(int x, int y);


};


#endif // CHECKEDWIDGET_H
