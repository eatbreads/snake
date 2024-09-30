#include "food.h"

Food::Food()
{
    memset(arr,0,sizeof(arr));// 使用memset初始化arr数组为全0，表示游戏区域开始时都是空的
    srand((unsigned)time(NULL));
    // 随机生成一个在游戏区域内部（避免边缘）的食物位置
    int tempW=rand()%(GAME_WIDTH-3)+3;
    int tempH=rand()%(GAME_HEIGHT-3)+3;
    m_Point.setPoint(tempW,tempH);//设置食物位置
    m_NoFood=false;//表示现在没有食物短缺

}
void Food::giveFood(QList<MyPoint> &v)
{
    memset(arr,0,sizeof(arr));
    for(auto it=v.begin();it!=v.end();++it)
    {
        arr[it->getY()][it->getX()]=1;
    }
    MyPoint p;
    for(int i=0;i<GAME_HEIGHT;++i)
    {
        for(int j=0;j<GAME_WIDTH;++j)
        {
            if(arr[i][j]==0)
            {
                p.setPoint(j,i);
                m_PointArr[this->m_EffPointSum]=p;
                ++m_EffPointSum;
            }
        }
    }
    if(m_EffPointSum==0){m_NoFood=true;return;}
    p=m_PointArr[rand()%m_EffPointSum];
    MyPoint initP;
    for(auto&val:m_PointArr){val=initP;}
    m_EffPointSum=0;
    this->m_Point=p;
}
