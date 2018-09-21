#include "mainwindow.h"
#include "ui_widget.h"
#include <QHBoxLayout>
#include <QString>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize( QSize( 700, SIZE ));
    isGameOver = false;

    scoreView = new QLabel(this);
    scoreView->setText(QString::fromLocal8Bit("分数：0"));
    scoreView->move(SIZE+100,150);

    gameStatusView = new QLabel(this);
    gameStatusView->move(SIZE+100,175);
    gameStatusView->setText(QString::fromLocal8Bit("游戏正在运行"));
    gameStatusView->resize(100,50);

    gameDescribeView = new QLabel(this);
    gameDescribeView->move(SIZE+100,225);
    gameDescribeView->setText(QString::fromLocal8Bit("上下左右：控制蛇\n的方向\nQ:暂停\nO:切换格子显示\n状态"));
    gameDescribeView->resize(100,100);

    checkedWidget = new CheckedWidget(this);
    checkedWidget->showLine(false);
    checkedWidget->setCallback(this);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete checkedWidget;
    delete scoreView;
    delete gameStatusView;
    delete gameDescribeView;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    if(!isGameOver){
        checkedWidget->paintEvent(e);
    }
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
    if(!isGameOver){
       checkedWidget->keyPressEvent(e);
    }
}



void MainWindow:: onScoreCallback(int score){
     scoreView->setText(QString::fromLocal8Bit("分数：")+QString::number(score));
}

void  MainWindow:: onGameStatesCallback(int status){
    if(status==GAME_OVER){
        if(!isGameOver){
            isGameOver = true;
            gameStatusView->setText(QString::fromLocal8Bit("游戏结束"));
            int nRet =QMessageBox::question(this,QString::fromLocal8Bit("提示信息"),QString::fromLocal8Bit("游戏结束"),QMessageBox::Yes);

            if(nRet==QMessageBox::Yes){
                qApp->exit();
            }
        }
    }
}
