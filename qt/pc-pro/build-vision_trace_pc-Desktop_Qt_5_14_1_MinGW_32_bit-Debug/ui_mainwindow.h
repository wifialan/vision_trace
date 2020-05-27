/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_4;
    QTextBrowser *textBrowser;
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButtonStop;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_down;
    QPushButton *pushButton_left;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_up;
    QPushButton *pushButton_right;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *comboBox_start_node;
    QComboBox *comboBox_stop_node;
    QLabel *label_5;
    QLineEdit *lineEdit_current_node;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_direction;
    QPushButton *pushButton_discon_net;
    QPushButton *pushButton_con_net;
    QRadioButton *radioButtonCar;
    QPushButton *pushButton_go;
    QPushButton *pushButton_update;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(726, 486);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(290, 10, 422, 215));
        verticalLayout_4 = new QVBoxLayout(layoutWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        textBrowser = new QTextBrowser(layoutWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setMinimumSize(QSize(420, 0));
        textBrowser->setReadOnly(false);

        verticalLayout_4->addWidget(textBrowser);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 271, 381));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 140, 278, 243));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButtonStop = new QPushButton(groupBox_2);
        pushButtonStop->setObjectName(QString::fromUtf8("pushButtonStop"));
        pushButtonStop->setStyleSheet(QString::fromUtf8("background-color: rgb(164, 0, 0);"));

        verticalLayout_2->addWidget(pushButtonStop);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        pushButton_down = new QPushButton(groupBox_2);
        pushButton_down->setObjectName(QString::fromUtf8("pushButton_down"));
        pushButton_down->setMinimumSize(QSize(80, 80));

        gridLayout->addWidget(pushButton_down, 1, 1, 1, 1);

        pushButton_left = new QPushButton(groupBox_2);
        pushButton_left->setObjectName(QString::fromUtf8("pushButton_left"));
        pushButton_left->setMinimumSize(QSize(80, 80));
        pushButton_left->setMaximumSize(QSize(80, 80));

        gridLayout->addWidget(pushButton_left, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        pushButton_up = new QPushButton(groupBox_2);
        pushButton_up->setObjectName(QString::fromUtf8("pushButton_up"));
        pushButton_up->setMinimumSize(QSize(80, 80));

        gridLayout->addWidget(pushButton_up, 0, 1, 1, 1);

        pushButton_right = new QPushButton(groupBox_2);
        pushButton_right->setObjectName(QString::fromUtf8("pushButton_right"));
        pushButton_right->setMinimumSize(QSize(80, 80));
        pushButton_right->setMaximumSize(QSize(80, 80));

        gridLayout->addWidget(pushButton_right, 1, 2, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));

        gridLayout->addLayout(verticalLayout_3, 0, 3, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 30, 31, 17));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 70, 31, 17));
        comboBox_start_node = new QComboBox(groupBox);
        comboBox_start_node->setObjectName(QString::fromUtf8("comboBox_start_node"));
        comboBox_start_node->setGeometry(QRect(50, 30, 86, 25));
        comboBox_stop_node = new QComboBox(groupBox);
        comboBox_stop_node->setObjectName(QString::fromUtf8("comboBox_stop_node"));
        comboBox_stop_node->setGeometry(QRect(50, 70, 86, 25));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 110, 71, 17));
        lineEdit_current_node = new QLineEdit(groupBox);
        lineEdit_current_node->setObjectName(QString::fromUtf8("lineEdit_current_node"));
        lineEdit_current_node->setGeometry(QRect(70, 110, 61, 25));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 430, 67, 17));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 400, 101, 21));
        label_direction = new QLabel(centralWidget);
        label_direction->setObjectName(QString::fromUtf8("label_direction"));
        label_direction->setGeometry(QRect(110, 400, 67, 21));
        pushButton_discon_net = new QPushButton(centralWidget);
        pushButton_discon_net->setObjectName(QString::fromUtf8("pushButton_discon_net"));
        pushButton_discon_net->setGeometry(QRect(550, 310, 127, 25));
        pushButton_con_net = new QPushButton(centralWidget);
        pushButton_con_net->setObjectName(QString::fromUtf8("pushButton_con_net"));
        pushButton_con_net->setGeometry(QRect(570, 280, 96, 25));
        radioButtonCar = new QRadioButton(centralWidget);
        radioButtonCar->setObjectName(QString::fromUtf8("radioButtonCar"));
        radioButtonCar->setGeometry(QRect(580, 250, 112, 23));
        radioButtonCar->setStyleSheet(QString::fromUtf8("background-color: rgb(138, 226, 52);"));
        radioButtonCar->setCheckable(false);
        radioButtonCar->setAutoExclusive(false);
        pushButton_go = new QPushButton(centralWidget);
        pushButton_go->setObjectName(QString::fromUtf8("pushButton_go"));
        pushButton_go->setGeometry(QRect(370, 290, 89, 25));
        pushButton_update = new QPushButton(centralWidget);
        pushButton_update->setObjectName(QString::fromUtf8("pushButton_update"));
        pushButton_update->setGeometry(QRect(370, 330, 89, 25));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 726, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "config", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "ctrl", nullptr));
        pushButtonStop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonStop->setShortcut(QCoreApplication::translate("MainWindow", "Space", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButton_down->setText(QCoreApplication::translate("MainWindow", "DOWN", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_down->setShortcut(QCoreApplication::translate("MainWindow", "Down", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButton_left->setText(QCoreApplication::translate("MainWindow", "LEFT", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_left->setShortcut(QCoreApplication::translate("MainWindow", "Left", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButton_up->setText(QCoreApplication::translate("MainWindow", "UP", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_up->setShortcut(QCoreApplication::translate("MainWindow", "Up", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButton_right->setText(QCoreApplication::translate("MainWindow", "RIGHT", nullptr));
#if QT_CONFIG(shortcut)
        pushButton_right->setShortcut(QCoreApplication::translate("MainWindow", "Right", nullptr));
#endif // QT_CONFIG(shortcut)
        label_3->setText(QCoreApplication::translate("MainWindow", "\350\265\267\347\202\271", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\347\273\210\347\202\271", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\350\212\202\347\202\271", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\345\260\217\350\275\246\345\275\223\345\211\215\346\234\235\345\220\221", nullptr));
        label_direction->setText(QString());
        pushButton_discon_net->setText(QCoreApplication::translate("MainWindow", "exit network", nullptr));
        pushButton_con_net->setText(QCoreApplication::translate("MainWindow", "ping network", nullptr));
        radioButtonCar->setText(QCoreApplication::translate("MainWindow", "Car", nullptr));
        pushButton_go->setText(QCoreApplication::translate("MainWindow", "GO", nullptr));
        pushButton_update->setText(QCoreApplication::translate("MainWindow", "update", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
