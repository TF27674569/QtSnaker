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
    // 画笔
//    QPainter painter(this);
//    painter.drawLine(QPointF(0,0),QPointF(100,100));


//       QPainter painter(this);
//       QPen pen; //画笔
//       pen.setColor(QColor(255, 0, 0));
//       pen.setStyle(Qt::DotLine);
//       QBrush brush(QColor(0, 255, 0, 125)); //画刷
//       painter.setPen(pen); //添加画笔
//       painter.setBrush(brush); //添加画刷
//       painter.drawRect(50, 50, 200, 100); //绘制矩形


    //线性渐变
       QLinearGradient linearGradient(QPointF(40, 190),
                                      QPointF(70, 190));
       //插入颜色
       linearGradient.setColorAt(0, Qt::yellow);
       linearGradient.setColorAt(0.5, Qt::red);
       linearGradient.setColorAt(1, Qt::green);
       //指定渐变区域以外的区域的扩散方式
       linearGradient.setSpread(QGradient::RepeatSpread);
       //使用渐变作为画刷
       QPainter painter(this);
       painter.setBrush(linearGradient);
       painter.drawRect(100, 100, 90, 40);

}
