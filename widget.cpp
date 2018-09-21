#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent * e)
{
    // ����
//    QPainter painter(this);
//    painter.drawLine(QPointF(0,0),QPointF(100,100));


//       QPainter painter(this);
//       QPen pen; //����
//       pen.setColor(QColor(255, 0, 0));
//       pen.setStyle(Qt::DotLine);
//       QBrush brush(QColor(0, 255, 0, 125)); //��ˢ
//       painter.setPen(pen); //��ӻ���
//       painter.setBrush(brush); //��ӻ�ˢ
//       painter.drawRect(50, 50, 200, 100); //���ƾ���


    //���Խ���
       QLinearGradient linearGradient(QPointF(40, 190),
                                      QPointF(70, 190));
       //������ɫ
       linearGradient.setColorAt(0, Qt::yellow);
       linearGradient.setColorAt(0.5, Qt::red);
       linearGradient.setColorAt(1, Qt::green);
       //ָ����������������������ɢ��ʽ
       linearGradient.setSpread(QGradient::RepeatSpread);
       //ʹ�ý�����Ϊ��ˢ
       QPainter painter(this);
       painter.setBrush(linearGradient);
       painter.drawRect(100, 100, 90, 40);

}
