#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{//设置标题
    this->setWindowTitle(WINDOW_NAME);
    this->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);


    //创建存放类
    m_savegame= new SaveGame();
    //创建一个10*10的透明image
    QImage image(QSize(10,10),QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    //将其转换成pixmap
     QPixmap tempPixmap = QPixmap::fromImage(image);
    this->setWindowIcon(QIcon(tempPixmap));

    m_An_Timer=new QTimer(this);
    connect(m_An_Timer,&QTimer::timeout,[=](){
        this->newAn();
    });
    m_GameStartBut=new MyPushButton("游戏开始",200,200*0.618-30,30);
    m_GameStartBut->setParent(this);
    m_GameStartBut->move((WINDOW_WIDTH-m_GameStartBut->width())*0.5,WINDOW_HEIGHT*0.5);
    m_GameStartBut->show();
// 创建两个GameEnd对象，分别用于游戏胜利和游戏失败场景
    m_GameWinWindow=new GameEnd("GAME WIN",1);
    m_GameWinWindow->hide();
    m_GameOverWindow=new GameEnd("GAME OVER,按任意键开始",0);
    m_GameOverWindow->hide();

    //点击游戏开始按钮之后就会进入这个lambda函数
    connect(m_GameStartBut,&MyPushButton::clicked,[=](){
        m_GameWindow=new GamePlay();//游戏场景
        m_GameWindow->m_HighScore=m_savegame->loadHighScore();
        //然后把游戏开始按钮隐藏
        m_GameStartBut->hide();
        m_GameWindow->move(this->x(),this->y());
        //执行动画效果 执行完后前往下一个场景,即显示游戏场景
        m_An_Timer->start(3);
        QTimer::singleShot(300,this,[=](){
            this->hide();//把当前场景隐藏,显示游戏场景
            m_GameWindow->show();
        });
        // 使用Qt的信号和槽机制，连接GamePlay类的gameWin信号到一个lambda表达式
        // 当gameWin信号被发射时，lambda表达式中的代码将被执行
        connect(m_GameWindow, &GamePlay::gameWin, [=](){
            // 当游戏胜利时，隐藏当前的游戏窗口
            m_GameWindow->hide();

            // 将游戏胜利窗口移动到游戏窗口原来的位置
            // 这里假设m_GameWinWindow是游戏胜利时显示的窗口
            m_GameWinWindow->move(m_GameWindow->x(), m_GameWindow->y());

            // 显示游戏胜利窗口
            m_GameWinWindow->show();

            // 删除并释放游戏窗口的内存
            // 注意：这里的删除操作需要确保在GamePlay类中没有其他地方再使用m_GameWindow指针
            // 或者在删除前将其置为nullptr，防止野指针问题
            delete m_GameWindow;

            // 将m_GameWindow指针置为nullptr，避免野指针问题
            m_GameWindow = nullptr;
        });

        // 类似于上面的connect调用，但这次是连接gameOver信号
        connect(m_GameWindow, &GamePlay::gameOver, [=](){
            // 当游戏结束时，隐藏当前的游戏窗口
            m_GameWindow->hide();

            // 将游戏结束窗口移动到游戏窗口原来的位置
            // 这里假设m_GameOverWindow是游戏结束时显示的窗口
            m_GameOverWindow->move(m_GameWindow->x(), m_GameWindow->y());

            // 显示游戏结束窗口
            m_GameOverWindow->show();

            // 删除并释放游戏窗口的内存
            // 同上，需要确保没有其他地方在使用m_GameWindow
            delete m_GameWindow;

            // 将m_GameWindow指针置为nullptr
            m_GameWindow = nullptr;
        });
    });

    GameEnd*GE;
    for(int i=0;i<2;++i)//这个for好像是在分批初始化GameEnd,底下那俩个疑似式GameEnd的派生类
    {
        GE=(i==0?m_GameWinWindow:m_GameOverWindow);
        connect(GE,&GameEnd::newGame,[=](){//绑定需要重新开始游戏的信号
            m_GameWindow=new GamePlay();//游戏场景
            //这边也是新加的
            m_GameWindow->m_HighScore=m_savegame->loadHighScore();
            GE->hide();
            m_GameWindow->move(GE->x(),GE->y());
            m_GameWindow->show();
            connect(m_GameWindow,&GamePlay::gameWin,[=](){//如果是赢了的信号
                m_GameWindow->hide();
                m_GameWinWindow->move(m_GameWindow->x(),m_GameWindow->y());
                m_GameWinWindow->show();
                delete m_GameWindow;
                m_GameWindow=nullptr;
            });
            connect(m_GameWindow,&GamePlay::gameOver,[=](){//如果是结束了的信号
                m_GameWindow->hide();
                //这个score是int,这个SaveGame是Qstirnglist
               int ready_to_save=m_GameWindow->m_HighScore;
                m_savegame->saveHighScore(ready_to_save);

                m_GameOverWindow->move(m_GameWindow->x(),m_GameWindow->y());
                m_GameOverWindow->show();
                delete m_GameWindow;
                m_GameWindow=nullptr;
            });
        });
    }
    // 以下代码似乎用于初始化动画的起始和结束位置，但似乎并未完全使用
    an_XU=m_GameStartBut->x();
    an_YU=m_GameStartBut->y();
    an_XD=m_GameStartBut->x();
    an_YD=m_GameStartBut->y()+m_GameStartBut->height();
    an_Zoom=-0.1f;
    // 计算动画缩放后的位置（这里可能是一个未完成的动画效果实现）
    m_XU=WINDOW_WIDTH*(1.f-an_Zoom)+an_XU*an_Zoom;
    m_YU=WINDOW_HEIGHT*(1.f-an_Zoom)+an_YU*an_Zoom;
    m_XD=WINDOW_WIDTH*(1.f-an_Zoom)+an_XD*an_Zoom;
    m_YD=an_YD*an_Zoom;


}
void Widget::newAn()
{
    static bool needInit=true;// 静态标志，用于控制动画的初始化
    // 如果是第一次调用或需要重置动画
    if(needInit)
    {
        an_Zoom=1.f;// 初始化缩放比例为1，表示没有缩放
        needInit=false;// 标记为已初始化
    }
    // 更新当前位置坐标，通过缩放比例和动画起始/结束坐标计算
    m_XU=WINDOW_WIDTH*(1.f-an_Zoom)+an_XU*an_Zoom;
    m_YU=WINDOW_HEIGHT*(1.f-an_Zoom)+an_YU*an_Zoom;
    m_XD=WINDOW_WIDTH*(1.f-an_Zoom)+an_XD*an_Zoom;// 这里m_XD的计算可能不完全符合常规逻辑，具体取决于使用场景
    m_YD=an_YD*an_Zoom;// YD的缩放计算
    an_Zoom-=0.01; // 缩放比例逐渐减小，用于实现缩放动画
 // 如果缩放比例小于或等于-0.1（这在实际应用中可能不太合理，通常缩放比例不会小于0），停止定时器
    if(an_Zoom<=-0.1f){
        m_An_Timer->stop(); // 停止定时器，动画结束
        needInit=true;// 标记为需要重置动画
    }
    update(); // 调用QWidget的update方法，请求重绘Widget，可能是为了显示动画的最新状态
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //绘制版本信息：在窗口的右下角（距离右边100像素、距离底部20像素的位置）

    painter.drawText(WINDOW_WIDTH-100,WINDOW_HEIGHT-20,QString(VERSION));
    //四行代码绘制了四条线条
    //第一条线条从点(m_XU, m_YU)绘制到点(m_XD, m_YD)
    painter.drawLine(m_XU,m_YU,m_XD,m_YD);
    //第二线条看起来像是第一条线条相对于窗口中心进行的水平翻转。
    painter.drawLine(WINDOW_WIDTH-m_XU,m_YU,WINDOW_WIDTH-m_XD,m_YD);
    //第三条线条从点(m_XU, m_YU)绘制到点(WINDOW_WIDTH-m_XU, m_YU)，即水平方向穿过窗口中心的一条线，其y坐标与第一条和第二条线条的起点和终点相同。
    painter.drawLine(m_XU,m_YU,WINDOW_WIDTH-m_XU,m_YU);
    //第四条线条从点(m_XD, m_YD)绘制到点(WINDOW_WIDTH-m_XD, m_YD)，这条线条与第三条线条平行，但位于不同的y坐标上。
    painter.drawLine(m_XD,m_YD,WINDOW_WIDTH-m_XD,m_YD);

}
Widget::~Widget()
{

}
