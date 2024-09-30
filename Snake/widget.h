#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"config.h"
#include<QPushButton>
#include"gameplay.h"
#include"mypushbutton.h"
#include <QPainter>
#include <QTime>
#include <QTimer>
#include "gameend.h"

#include "savegame.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    MyPushButton * m_GameStartBut;//开始按钮指针

    GamePlay * m_GameWindow;//游戏界面指针

    GameEnd * m_GameWinWindow;//游戏胜利
    SaveGame * m_savegame;
    GameEnd * m_GameOverWindow;//游戏失败

    QTimer * m_An_Timer;
 // 以下变量可能用于动画效果中的位置计算
    int an_XU,an_YU,an_XD,an_YD;// 动画的起始坐标和结束坐标（或方向坐标）

    int m_XU,m_YU,m_XD,m_YD;// 可能是当前动画或对象的位置坐标

    double an_Zoom;// 动画的缩放比例，可能用于实现缩放动画效果
// 自定义方法，可能用于启动或更新动画
    void newAn();
 // 重写QWidget的paintEvent方法，用于自定义绘制逻辑
    void paintEvent(QPaintEvent *);

    Widget(QWidget *parent = 0);

    ~Widget();
};

#endif // WIDGET_H
