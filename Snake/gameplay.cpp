#include "gameplay.h"

// GamePlay类的构造函数
GamePlay::GamePlay(QWidget *parent) : QWidget(parent)
{
    m_CurrentScore = 0;
    m_HighScore = 0;


    // 设置窗口标题
    this->setWindowTitle(WINDOW_NAME);
    // 设置窗口的固定大小
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // 创建一个10x10的透明图像，并用作窗口图标
    QImage image(QSize(10,10), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPixmap tempPixmap = QPixmap::fromImage(image);
    this->setWindowIcon(QIcon(tempPixmap));

    // 初始化游戏定时器
    gameTimer = new QTimer(this);
    gameTimer->start(60); // 设置定时器事件每100毫秒触发一次
    // 延迟500毫秒后连接定时器信号到游戏运行槽,表示500毫秒之后计时器开始运行
    QTimer::singleShot(500, this, [=](){
        connect(gameTimer, &QTimer::timeout, [=](){
            gameRun(); // 定时器触发时调用游戏运行函数
        });
    });

    // 设置窗口不接收焦点
    setFocusPolicy(Qt::NoFocus);
}

// 游戏主运行函数
void GamePlay::gameRun()
{
    m_Snake.nextSnake(); // 蛇的下一步移动
    isEat(); // 检查是否吃到食物
    if (!this->eatFood)
    {
        m_Snake.eraseTail(); // 没有吃到食物则删除蛇尾
    }
    this->eatFood = false; // 重置吃到食物的标志

    update(); // 重绘界面

    // 检查游戏是否结束
    if (m_Snake.m_CantCon)
    {
        if(m_CurrentScore>m_HighScore)
        {
            m_HighScore = m_CurrentScore;
        }
        emit gameOver(); // 发射游戏结束信号
    }
    // 检查是否吃完所有食物
    if (m_Food.m_NoFood)
    {
        emit gameWin(); // 发射游戏胜利信号
    }
}

// 检查蛇是否吃到食物
void GamePlay::isEat()
{
    if (m_Snake.m_Head == m_Food.m_Point)
    {
        this->eatFood = true;
        this->m_CurrentScore ++;
        m_Food.giveFood(m_Snake.m_Body); // 生成新的食物
    }
}

// 绘制游戏界面
void GamePlay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    // 设置画笔和画刷
    QPen pen; pen.setStyle(Qt::NoPen);
    QBrush brush; brush.setColor(QColor(0,255,0,120)); brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen); painter.setBrush(brush);
    // 绘制蛇的身体
    for (auto it = m_Snake.m_Body.begin(); it != m_Snake.m_Body.end(); ++it)
    {//这个game_step可以理解为游戏的方格长度,或者理解为像素
        painter.drawRect(it->getX()*GAME_STEP, it->getY()*GAME_STEP, GAME_STEP, GAME_STEP);
    }
    // 设置食物的颜色并绘制食物
    brush.setColor(QColor(255,0,0,120));
    painter.setBrush(brush);
    if (!m_Food.m_NoFood)
    {
        painter.drawRect(m_Food.m_Point.getX()*GAME_STEP, m_Food.m_Point.getY()*GAME_STEP, GAME_STEP, GAME_STEP);
    }
    // 设置网格线的颜色并绘制网格
    painter.setPen(QPen(QColor(79,79,79,80)));
    //绘制竖线
    for (int i = 0; i <= GAME_WIDTH; ++i)
    {
        painter.drawLine(i*GAME_STEP, 0, i*GAME_STEP, GAME_HEIGHT*GAME_STEP);
    }
   // 绘制横线
    for (int i = 0; i <= GAME_HEIGHT; ++i)
    {
        painter.drawLine(0, i*GAME_STEP, GAME_WIDTH*GAME_STEP, i*GAME_STEP);
    }

    paint_CurrentScore(painter);
    paint_HighScore(painter);
}

// 处理键盘按键事件
void GamePlay::keyPressEvent(QKeyEvent *event)
{
    // 根据按键改变蛇的移动方向
    switch(event->key())
    {
    case Qt::Key_Up:
    case Qt::Key_W:
        if (m_Snake.m_Direction == 's') return;
        m_Snake.m_Direction = 'w';
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        if (m_Snake.m_Direction == 'd') return;
        m_Snake.m_Direction = 'a';
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        if (m_Snake.m_Direction == 'w') return;
        m_Snake.m_Direction = 's';
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        if (m_Snake.m_Direction == 'a') return;
        m_Snake.m_Direction = 'd';
        break;
    default:
        break;
    }
}


void GamePlay::paint_CurrentScore(QPainter & painter)
{
    //QColor scoreColor(255, 255, 255); // 白色
    QColor scoreColor(0, 0, 0); // 设定为黑色
    QFont scoreFont("Arial", 16, QFont::Bold);
    painter.setPen(QPen(scoreColor));
    painter.setFont(scoreFont);

    // 计算分数表单的位置和大小
    int scoreWidth = 200; // 分数区域的宽度
    int scoreMargin = 30; // 分数区域与游戏窗口之间的边距
    int scoreX = width() - scoreWidth - scoreMargin; // 分数区域的X坐标
    int scoreY = scoreMargin; // 分数区域的Y坐标（可以从顶部开始）
    int lineHeight = 30; // 每行文本的高度

    // 绘制当前分数
    QString currentScoreText = QString("分数: %1").arg(m_CurrentScore);
    painter.drawText(scoreX, scoreY, currentScoreText);
    scoreY += lineHeight;
    // void printHighScores(const QStringList &highScores) {
    //     foreach (const QString &line, highScores) {
    //         QStringList parts = line.split(":", Qt::SkipEmptyParts);
    //         if (parts.size() == 2) {
    //             QString rank = parts[0];
    //             QString score = parts[1];
    //             qDebug() << rank << ":" << score;
    //         }
    //     }
    // }
    // 绘制历史最高分数


        QString highScoreText = QString("历史最高分数:%1").arg(m_HighScore);
        painter.drawText(scoreX, scoreY, highScoreText);


}

void GamePlay::paint_HighScore(QPainter & painter)
{

}
