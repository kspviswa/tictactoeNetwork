/********************************************************************************
** Form generated from reading UI file 'game2.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAME2_H
#define UI_GAME2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Game2
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *Button_EndGame;
    QPushButton *Button_Submit;
    QListWidget *list_Participants;
    QTextEdit *text_UserName;
    QLabel *label_Username;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *Game2)
    {
        if (Game2->objectName().isEmpty())
            Game2->setObjectName(QStringLiteral("Game2"));
        Game2->resize(571, 500);
        Game2->setMaximumSize(QSize(600, 500));
        Game2->setAutoFillBackground(true);
        centralWidget = new QWidget(Game2);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(40, 10, 241, 211));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 71, 61));
        QFont font;
        font.setPointSize(32);
        font.setBold(true);
        font.setWeight(75);
        pushButton->setFont(font);
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(80, 10, 71, 61));
        pushButton_2->setFont(font);
        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(150, 10, 71, 61));
        pushButton_3->setFont(font);
        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(10, 70, 71, 61));
        pushButton_4->setFont(font);
        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(80, 70, 71, 61));
        pushButton_5->setFont(font);
        pushButton_6 = new QPushButton(groupBox);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(150, 70, 71, 61));
        pushButton_6->setFont(font);
        pushButton_7 = new QPushButton(groupBox);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(10, 130, 71, 61));
        pushButton_7->setFont(font);
        pushButton_8 = new QPushButton(groupBox);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(80, 130, 71, 61));
        pushButton_8->setFont(font);
        pushButton_9 = new QPushButton(groupBox);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(150, 130, 71, 61));
        pushButton_9->setFont(font);
        Button_EndGame = new QPushButton(centralWidget);
        Button_EndGame->setObjectName(QStringLiteral("Button_EndGame"));
        Button_EndGame->setGeometry(QRect(310, 330, 211, 61));
        Button_Submit = new QPushButton(centralWidget);
        Button_Submit->setObjectName(QStringLiteral("Button_Submit"));
        Button_Submit->setGeometry(QRect(50, 330, 211, 61));
        list_Participants = new QListWidget(centralWidget);
        list_Participants->setObjectName(QStringLiteral("list_Participants"));
        list_Participants->setGeometry(QRect(310, 20, 211, 181));
        text_UserName = new QTextEdit(centralWidget);
        text_UserName->setObjectName(QStringLiteral("text_UserName"));
        text_UserName->setGeometry(QRect(150, 270, 371, 31));
        text_UserName->setTabChangesFocus(true);
        label_Username = new QLabel(centralWidget);
        label_Username->setObjectName(QStringLiteral("label_Username"));
        label_Username->setGeometry(QRect(50, 270, 91, 31));
        Game2->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Game2);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 571, 25));
        Game2->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Game2);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Game2->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Game2);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Game2->setStatusBar(statusBar);
        toolBar = new QToolBar(Game2);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        Game2->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(Game2);

        QMetaObject::connectSlotsByName(Game2);
    } // setupUi

    void retranslateUi(QMainWindow *Game2)
    {
        Game2->setWindowTitle(QApplication::translate("Game2", "MainWindow", 0));
        groupBox->setTitle(QString());
        pushButton->setText(QString());
        pushButton_2->setText(QString());
        pushButton_3->setText(QString());
        pushButton_4->setText(QString());
        pushButton_5->setText(QString());
        pushButton_6->setText(QString());
        pushButton_7->setText(QString());
        pushButton_8->setText(QString());
        pushButton_9->setText(QString());
        Button_EndGame->setText(QApplication::translate("Game2", "End Game", 0));
        Button_Submit->setText(QApplication::translate("Game2", "Start", 0));
        label_Username->setText(QApplication::translate("Game2", "Player Name", 0));
        toolBar->setWindowTitle(QApplication::translate("Game2", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class Game2: public Ui_Game2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAME2_H
