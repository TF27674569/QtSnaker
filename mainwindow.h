#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "checkedwidget.h"
#include "callback.h"
#include <QLabel>

namespace Ui {
class Widget;
}


class MainWindow : public QMainWindow,public Callback
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
     Ui::Widget *ui;
     CheckedWidget* checkedWidget;
     QLabel* scoreView;
     QLabel* gameStatusView;
     QLabel* gameDescribeView;
     bool isGameOver;

protected:
     // 绘制事件回调
    void paintEvent(QPaintEvent * e);
    // 键盘相应事件回调
    void keyPressEvent(QKeyEvent *);

signals:


public:
    // callback的虚函数
   virtual void onScoreCallback(int score);

   virtual void onGameStatesCallback(int status);



public slots:
};

#endif // MAINWINDOW_H
