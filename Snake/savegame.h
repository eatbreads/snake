#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>

class SaveGame {
public:
    SaveGame() {
        // 初始化保存目录
        saveDir = QDir::currentPath() + "/SaveGames";
        if (!QDir().exists(saveDir)) {
            QDir().mkpath(saveDir);
        }
    }

    // 保存历史最大值
    bool saveHighScore(int highScore) {
        QString filePath = saveDir + "/highScore.txt";
        QFile file(filePath);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Cannot open file for writing:" << file.errorString();
            return false;
        }

        QTextStream out(&file);
        out << highScore;

        file.close();
        return true;
    }

    // 读取历史最大值
    int loadHighScore() {
        QString filePath = saveDir + "/highScore.txt";
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Cannot open file for reading:" << file.errorString();
            // 返回一个默认值，比如0或者一个表示无效分数的特殊值
            return -1; // 或者你可以选择一个更适合你应用程序的默认值
        }

        QTextStream in(&file);
        QString line = in.readLine().trimmed(); // 读取一行并去除前后的空白字符
        bool ok;
        int highScore = line.toInt(&ok); // 尝试将字符串转换为整数

        file.close();

        if (!ok) {
            qDebug() << "Failed to convert the read string to an integer.";
            // 如果转换失败，同样返回一个默认值
            return -1; // 或者你之前选择的默认值
        }

        return highScore;
    }

private:
    QString saveDir;
};
