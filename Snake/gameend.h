#ifndef GAMEEND_H
#define GAMEEND_H

#include <QWidget>
#include <QLabel>
#include "mypushbutton.h"
#include <QString>
//新加的头文件
#include <QKeyEvent>

class GameEnd : public QWidget
{
    Q_OBJECT
public:
    // 0 Over 1 Win
    QLabel * m_Label;

    MyPushButton * m_But;

    GameEnd(QString str,int end);
    /////////////////新加入
protected:
    // bool eventFilter(QObject *obj, QEvent *event) override {
    //     if (obj == this && event->type() == QEvent::KeyPress) {
    //         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    //         if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
    //             emit newGame();
    //             return true; // 事件已处理
    //         }
    //     }
    //     // 对于未处理的事件，调用基类的 eventFilter 方法
    //     return QWidget::eventFilter(obj, event);
    // }
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (obj == this && event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                emit newGame();
                return true; // 事件已处理
            }
        }
        // 对于未处理的事件，调用基类的 eventFilter 方法
        return QWidget::eventFilter(obj, event);
    }



    ///////////////
signals:
    void newGame();

public slots:
};

#endif // GAMEEND_H
