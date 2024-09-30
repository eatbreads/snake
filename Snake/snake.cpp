#include "snake.h"

Snake::Snake()
{
    this->m_Head.setPoint(3,0);
    m_Head.setPoint(2,0);
    m_Body.push_back(MyPoint(0,0));
    m_Body.push_back(MyPoint(1,0));
    m_Body.push_back(MyPoint(2,0));
    m_Direction='d';//初始方向向右
}

// Snake类的成员函数，用于更新蛇的位置
void Snake::nextSnake()
{
    int tempX, tempY; // 临时变量，用于存储蛇头下一步的坐标
    tempX = m_Head.getX(); // 获取当前蛇头的X坐标
    tempY = m_Head.getY(); // 获取当前蛇头的Y坐标

    // 根据蛇的当前方向更新tempX和tempY
    if (m_Direction == 'w') { --tempY; } // 向上移动
    else if (m_Direction == 'a') { --tempX; } // 向左移动
    else if (m_Direction == 's') { ++tempY; } // 向下移动
    else if (m_Direction == 'd') { ++tempX; } // 向右移动

    // 检查新坐标是否超出了游戏边界
    if (tempX < 0 || tempY < 0 || tempX >= GAME_WIDTH || tempY >= GAME_HEIGHT) // 注意：这里应该是tempY >= GAME_HEIGHT
    {
        m_CantCon = true; // 如果超出边界，设置游戏结束标志
        return; // 结束函数
    }

    // 检查新坐标是否与蛇身的其他部分重合
    for (QList<MyPoint>::iterator val = m_Body.begin() + 1; val != m_Body.end(); ++val) // 注意：应该是m_Body.end()，以检查整个蛇身
    {
        if (val->getX() == tempX && val->getY() == tempY)
        {
            m_CantCon = true; // 如果与蛇身重合，设置游戏结束标志
            return; // 结束函数
        }
    }

    // 如果新坐标合法且不与蛇身重合，则更新蛇的位置
    MyPoint nextHead(tempX, tempY); // 创建新的蛇头点
    m_Body.push_back(nextHead); // 将新蛇头添加到蛇身的末尾
    m_Head = nextHead; // 更新蛇头为新位置

    // 注意：在实际应用中，您可能还需要移除蛇身的第一个元素（即最尾部的元素），以保持蛇身的长度不变
    // 例如：m_Body.pop_front(); // 但由于QList不支持pop_front()，您可能需要其他逻辑来处理
}

void Snake::eraseTail()
{
    m_Body.erase(m_Body.begin());
}
