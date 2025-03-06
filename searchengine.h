#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H
#include <QMainWindow>
#include <QDir>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDomDocument>
#include <QRegularExpression>
#include <QMap>
#include <QString>
#include <vector>
#include <QCompleter>
#include <QStringListModel>
#include "Jieba.hpp"
QT_BEGIN_NAMESPACE
const char* const DICT_PATH = "D:/qt/QtOnline/SearchEngine/dict/jieba.dict.utf8";
const char* const HMM_PATH = "D:/qt/QtOnline/SearchEngine/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "D:/qt/QtOnline/SearchEngine/dict/user.dict.utf8";
const char* const IDF_PATH = "D:/qt/QtOnline/SearchEngine/dict/idf.utf8";
const char* const STOP_WORD_PATH = "D:/qt/QtOnline/SearchEngine/dict/stop_words.utf8";
namespace Ui {
class SearchEngine;
}
QT_END_NAMESPACE

class SearchEngine : public QMainWindow
{
    Q_OBJECT


public:


     cppjieba::Jieba jieba;
    SearchEngine(QWidget *parent = nullptr);
    ~SearchEngine();

    QMap<QString, QString> readTextFiles(const QString& folderPath);
    QString crawlAndSave(const QString& url, const QString& saveFolder) ;
    QMap<QString, QMap<QString, QList<int>>> buildInvertedIndex(const QMap<QString, QString>& documents);
    std::vector<std::string> cutText(const QString& text);
    QMap<QString, QList<int>> search(const QMap<QString, QMap<QString, QList<int>>>& invertedIndex, const QString& query);
    QList<QPair<QString, QList<int>>> sortResults(const QMap<QString, QList<int>>& resultDocs);
    QList<QString> getContext(const QString& docText, const QList<int>& positions, int keywordLength, int contextLength = 20);
     QMap<QString, QString> documents;
    QMap<QString, QMap<QString, QList<int>>> invertedIndex;
private slots:
    void onSearch();
private:
    Ui::SearchEngine *ui;
    QCompleter *completer;  // 声明 QCompleter 指针
    QStringList completionWords;  // 存储补全词库
};
#endif // SEARCHENGINE_H
