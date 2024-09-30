#ifndef FOOD_H
#define FOOD_H

#include"mypoint.h"
#include<QList>
#include"config.h"
#include<ctime>
#include<iostream>
class Food
{
public:
    MyPoint m_Point; // 当前食物的位置点

    bool arr[GAME_HEIGHT][GAME_WIDTH];// 一个二维布尔数组，用于标记游戏区域中哪些位置已有食物或障碍物
    //下面这个是一维的
    MyPoint m_PointArr[GAME_WIDTH*GAME_HEIGHT+10];// 一个MyPoint数组，用于临时存储可能的食物位置

    int m_EffPointSum=0;// 有效食物位置的数量

    bool m_NoFood;// 标记是否没有可用的食物位置

    void giveFood(QList<MyPoint>&v);// 成员函数，用于在给定的QList<MyPoint>中放置食物，并更新当前食物位置

    Food();
};

#endif // FOOD_H
