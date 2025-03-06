/********************************************************************************
** Form generated from reading UI file 'searchengine.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHENGINE_H
#define UI_SEARCHENGINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchEngine
{
public:
    QWidget *centralwidget;
    QPushButton *searchButton;
    QLineEdit *lineEdit;
    QTextEdit *textEdit;
    QComboBox *dropdown;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SearchEngine)
    {
        if (SearchEngine->objectName().isEmpty())
            SearchEngine->setObjectName("SearchEngine");
        SearchEngine->resize(800, 600);
        centralwidget = new QWidget(SearchEngine);
        centralwidget->setObjectName("centralwidget");
        searchButton = new QPushButton(centralwidget);
        searchButton->setObjectName("searchButton");
        searchButton->setGeometry(QRect(190, 200, 281, 18));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(190, 170, 281, 20));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(190, 230, 391, 211));
        dropdown = new QComboBox(centralwidget);
        dropdown->setObjectName("dropdown");
        dropdown->setGeometry(QRect(480, 170, 101, 22));
        SearchEngine->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SearchEngine);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 18));
        SearchEngine->setMenuBar(menubar);
        statusbar = new QStatusBar(SearchEngine);
        statusbar->setObjectName("statusbar");
        SearchEngine->setStatusBar(statusbar);

        retranslateUi(SearchEngine);

        QMetaObject::connectSlotsByName(SearchEngine);
    } // setupUi

    void retranslateUi(QMainWindow *SearchEngine)
    {
        SearchEngine->setWindowTitle(QCoreApplication::translate("SearchEngine", "SearchEngine", nullptr));
        searchButton->setText(QCoreApplication::translate("SearchEngine", "\346\220\234\347\264\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchEngine: public Ui_SearchEngine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHENGINE_H
