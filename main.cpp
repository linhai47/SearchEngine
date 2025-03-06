#include "searchengine.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SearchEngine w;
    QString folderPath = "D:\\qt\\QtOnline\\SearchEngine\\answer\\C4-Literature";
    w.documents = w.readTextFiles(folderPath);

    // 构建倒排索引
    w.invertedIndex =w.buildInvertedIndex(w.documents);
    w.show();
    return a.exec();
}
