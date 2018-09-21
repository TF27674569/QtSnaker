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
    scoreView->setText(QString::fromLocal8Bit("������0"));
    scoreView->move(SIZE+100,150);

    gameStatusView = new QLabel(this);
    gameStatusView->move(SIZE+100,175);
    gameStatusView->setText(QString::fromLocal8Bit("��Ϸ��������"));
    gameStatusView->resize(100,50);

    gameDescribeView = new QLabel(this);
    gameDescribeView->move(SIZE+100,225);
    gameDescribeView->setText(QString::fromLocal8Bit("�������ң�������\n�ķ���\nQ:��ͣ\nO:�л�������ʾ\n״̬"));
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
     scoreView->setText(QString::fromLocal8Bit("������")+QString::number(score));
}

void  MainWindow:: onGameStatesCallback(int status){
    if(status==GAME_OVER){
        if(!isGameOver){
            isGameOver = true;
            gameStatusView->setText(QString::fromLocal8Bit("��Ϸ����"));
            int nRet =QMessageBox::question(this,QString::fromLocal8Bit("��ʾ��Ϣ"),QString::fromLocal8Bit("��Ϸ����"),QMessageBox::Yes);

            if(nRet==QMessageBox::Yes){
                qApp->exit();
            }
        }
    }
}
