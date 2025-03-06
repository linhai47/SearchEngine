#include "searchengine.h"
#include "ui_searchengine.h"

SearchEngine::SearchEngine(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SearchEngine)
    , jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)
{
    ui->setupUi(this);
    connect(ui->searchButton, &QPushButton::clicked, this, &SearchEngine::onSearch);
    ui->dropdown->addItems({"选项1", "选项2", "选项3"});

    // 初始化自动补全器
    completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);  // 设置大小写不敏感
    ui->lineEdit->setCompleter(completer);  // 将 QCompleter 与输入框关联
}

SearchEngine::~SearchEngine()
{
    delete ui;
    // 确保释放自动补全器资源
    if (completer) {
        delete completer;
    }
}

/**
 * @brief 处理搜索按钮点击事件
 */
void SearchEngine::onSearch()
{
    QString query = ui->lineEdit->text();
    // 若查询为空，直接返回
    if (query.isEmpty()) {
        ui->textEdit->clear();
        ui->textEdit->append("请输入查询关键词。");
        return;
    }

    QMap<QString, QList<int>> resultDocs = search(invertedIndex, query);
    QList<QPair<QString, QList<int>>> sortedDocs = sortResults(resultDocs);

    qDebug() << "查询关键词：" << query;
    qDebug() << "搜索结果数量：" << resultDocs.size();
    qDebug() << "排序结果数量：" << sortedDocs.size();

    for (const auto &docPair : sortedDocs) {
        QString docId = docPair.first;
        QList<int> details = docPair.second;
        int frequency = details.first();
        QList<int> positions = details.mid(1);
        qDebug() << "文档ID: " << docId << ", 词频: " << frequency << ", 关键词位置: " << positions;
    }

    ui->textEdit->clear();
    ui->textEdit->append("搜索结果：");

    for (const auto &doc : sortedDocs) {
        QString docId = doc.first;
        QString docText = documents[docId];
        int keywordLength = query.length();
        QList<QString> contexts = getContext(docText, doc.second, keywordLength);

        QString fileLink = docId;
        QString fileTitle = docId.remove(".txt");
        ui->textEdit->append(QString("<a href='%1'>%2</a>").arg(fileLink).arg(fileTitle));
        ui->textEdit->append(QString("词频: %1").arg(doc.second.first()));

        for (const QString &context : contexts) {
            ui->textEdit->append(QString("  上下文: %1").arg(context));
        }
        ui->textEdit->append("---");
    }
    if (sortedDocs.isEmpty()) {
        ui->textEdit->append("未找到相关文档。");
    }
}
/**
 * @brief 构建倒排索引
 * @param documents 文档集合
 * @return 倒排索引
 */
QMap<QString, QMap<QString, QList<int>>> SearchEngine::buildInvertedIndex(const QMap<QString, QString> &documents)
{
    QMap<QString, QMap<QString, QList<int>>> invertedIndex;
    foreach (const QString &docId, documents.keys()) {
        const QString &docText = documents[docId];
        std::vector<std::string> words = cutText(docText);
        for (int pos = 0; pos < words.size(); ++pos) {
            QString word = QString::fromStdString(words[pos]);
            if (!invertedIndex.contains(word)) {
                invertedIndex[word] = QMap<QString, QList<int>>();
            }
            if (!invertedIndex[word].contains(docId)) {
                invertedIndex[word][docId] = QList<int>();
            }
            invertedIndex[word][docId].append(pos);
            invertedIndex[word][docId].first()++; // 记录词频
        }
    }
    return invertedIndex;
}

/**
 * @brief 对文本进行分词
 * @param text 待分词的文本
 * @return 分词结果
 */
std::vector<std::string> SearchEngine::cutText(const QString &text)
{
    std::string str = text.toStdString();
    std::vector<std::string> words;
    jieba.Cut(str, words, true);
    return words;
}

/**
 * @brief 在倒排索引中搜索关键词
 * @param invertedIndex 倒排索引
 * @param query 查询关键词
 * @return 搜索结果
 */
QMap<QString, QList<int>> SearchEngine::search(const QMap<QString, QMap<QString, QList<int>>> &invertedIndex, const QString &query)
{
    QMap<QString, QList<int>> resultDocs;
    std::vector<std::string> queryWords = cutText(query);
    foreach (const std::string &word, queryWords) {
        QString qWord = QString::fromStdString(word);
        if (invertedIndex.contains(qWord)) {
            const QMap<QString, QList<int>>& docs = invertedIndex[qWord];
            foreach (const QString &docId, docs.keys()) {
                if (!resultDocs.contains(docId)) {
                    resultDocs[docId] = QList<int>();
                }
                resultDocs[docId].append(docs[docId].first()); // 累加词频
                resultDocs[docId].append(docs[docId].mid(1)); // 记录位置
            }
        }
    }
    return resultDocs;
}

/**
 * @brief 对搜索结果进行排序
 * @param resultDocs 搜索结果
 * @return 排序后的搜索结果
 */
QList<QPair<QString, QList<int>>> SearchEngine::sortResults(const QMap<QString, QList<int>> &resultDocs)
{
    QList<QPair<QString, QList<int>>> sortedDocs;
    foreach (const QString &docId, resultDocs.keys()) {
        sortedDocs.append(qMakePair(docId, resultDocs[docId]));
    }
    std::sort(sortedDocs.begin(), sortedDocs.end(), [](const QPair<QString, QList<int>>& a, const QPair<QString, QList<int>>& b) {
        return a.second.first() > b.second.first();
    });
    return sortedDocs;
}

/**
 * @brief 获取关键词的上下文
 * @param docText 文档文本
 * @param positions 关键词位置
 * @param keywordLength 关键词长度
 * @param contextLength 上下文长度
 * @return 上下文列表
 */
QList<QString> SearchEngine::getContext(const QString &docText, const QList<int> &positions, int keywordLength, int contextLength)
{
    QList<QString> contexts;
    foreach (int pos, positions.mid(1)) {
        int start = qMax(0, pos - contextLength);
        int end = qMin(docText.length(), pos + keywordLength + contextLength);
        QString context = docText.mid(start, end - start);
        contexts.append(context);
    }
    return contexts;
}

/**
 * @brief 爬取网页并保存为文本文件
 * @param url 网页链接
 * @param saveFolder 保存文件夹
 * @return 保存的文件路径
 */
QString crawlAndSave(QString url, const QString& saveFolder) {
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QString html = reply->readAll();

        // 简单解析文本（实际需更复杂处理）
        QDomDocument doc;
        if (doc.setContent(html)) {
            QString text = doc.documentElement().text();

            // 保存文件
            QRegularExpression re("https?://");
            QString fileName = url.remove(re).remove("/").append(".txt");
            QString filePath = QDir(saveFolder).filePath(fileName);
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out.setEncoding(QStringConverter::Utf8);
                out << text;
                file.close();
                return filePath;
            }
        }
    }
    return QString();
}

/**
 * @brief 读取指定文件夹下的文本文件
 * @param folderPath 文件夹路径
 * @return 文档集合
 */
QMap<QString, QString> SearchEngine::readTextFiles(const QString& folderPath) {
    QMap<QString, QString> documents;
    completionWords.clear();  // 清空补全词库

    QDir dir(folderPath);
    if (!dir.exists()) {
        qDebug() << "文件夹路径不存在: " << folderPath;
        return documents;
    }

    QStringList filters;
    filters << "*.txt";

    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    // 获取GBK编码
    QTextCodec *codec = QTextCodec::codecForName("GBK");

    for (const auto &fileInfo : fileList) {
        QFile file(fileInfo.absoluteFilePath());
        if (file.open(QIODevice::ReadOnly)) {
            // 读取文件的所有数据
            QByteArray data = file.readAll();
            // 使用GBK编码将数据转换为QString
            QString text = codec->toUnicode(data);
            documents[fileInfo.fileName()] = text;

            // 提取单词或短语添加到补全词库
            QStringList words = text.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);
            for (const QString& word : words) {
                if (!completionWords.contains(word)) {
                    completionWords.append(word);
                }
            }

            file.close();
        } else {
            qDebug() << "无法打开文件: " << fileInfo.absoluteFilePath() << ", 错误信息: " << file.errorString();
        }
    }

    // 更新 QCompleter 的数据
    completer->setModel(new QStringListModel(completionWords, completer));

    return documents;
}
