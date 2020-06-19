/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QLabel *label_cam;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLabel *label_2;
    QComboBox *comboBox_start_node;
    QComboBox *comboBox_stop_node;
    QPushButton *pushButton_go;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QDoubleSpinBox *doubleSpinBox_line_speed;
    QDoubleSpinBox *doubleSpinBox_angular_speed;
    QLabel *label_8;
    QLabel *label_9;
    QComboBox *comboBox_direction;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QPushButton *pushButton_down;
    QPushButton *pushButton_left;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_up;
    QPushButton *pushButton_right;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_stop;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_3;
    QTextBrowser *textBrowser;
    QLabel *label;
    QPushButton *pushButton;
    QComboBox *comboBox_serial;
    QPushButton *pushButton_serial_connect;
    QPushButton *pushButton_serial_disconnect;
    QLineEdit *lineEdit_path_status;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLineEdit *lineEdit_command;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1506, 528);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label_cam = new QLabel(centralWidget);
        label_cam->setObjectName(QStringLiteral("label_cam"));
        label_cam->setGeometry(QRect(930, 20, 550, 400));
        label_cam->setMinimumSize(QSize(550, 400));
        label_cam->setMaximumSize(QSize(550, 400));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 295, 446));
        verticalLayout_4 = new QVBoxLayout(layoutWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 1, 1, 1, 1);

        comboBox_start_node = new QComboBox(groupBox);
        comboBox_start_node->setObjectName(QStringLiteral("comboBox_start_node"));

        gridLayout_2->addWidget(comboBox_start_node, 0, 2, 1, 1);

        comboBox_stop_node = new QComboBox(groupBox);
        comboBox_stop_node->setObjectName(QStringLiteral("comboBox_stop_node"));

        gridLayout_2->addWidget(comboBox_stop_node, 1, 2, 1, 1);

        pushButton_go = new QPushButton(groupBox);
        pushButton_go->setObjectName(QStringLiteral("pushButton_go"));

        gridLayout_2->addWidget(pushButton_go, 0, 3, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 2, 1, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 3, 1, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_2->addWidget(label_7, 4, 1, 1, 1);

        doubleSpinBox_line_speed = new QDoubleSpinBox(groupBox);
        doubleSpinBox_line_speed->setObjectName(QStringLiteral("doubleSpinBox_line_speed"));
        doubleSpinBox_line_speed->setDecimals(2);
        doubleSpinBox_line_speed->setSingleStep(0.01);
        doubleSpinBox_line_speed->setValue(0);

        gridLayout_2->addWidget(doubleSpinBox_line_speed, 3, 2, 1, 1);

        doubleSpinBox_angular_speed = new QDoubleSpinBox(groupBox);
        doubleSpinBox_angular_speed->setObjectName(QStringLiteral("doubleSpinBox_angular_speed"));

        gridLayout_2->addWidget(doubleSpinBox_angular_speed, 4, 2, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_2->addWidget(label_8, 3, 3, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_2->addWidget(label_9, 4, 3, 1, 1);

        comboBox_direction = new QComboBox(groupBox);
        comboBox_direction->setObjectName(QStringLiteral("comboBox_direction"));

        gridLayout_2->addWidget(comboBox_direction, 2, 2, 1, 1);


        verticalLayout_4->addWidget(groupBox);

        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_down = new QPushButton(groupBox_2);
        pushButton_down->setObjectName(QStringLiteral("pushButton_down"));
        pushButton_down->setMinimumSize(QSize(80, 80));

        gridLayout->addWidget(pushButton_down, 1, 1, 1, 1);

        pushButton_left = new QPushButton(groupBox_2);
        pushButton_left->setObjectName(QStringLiteral("pushButton_left"));
        pushButton_left->setMinimumSize(QSize(80, 80));
        pushButton_left->setMaximumSize(QSize(80, 80));

        gridLayout->addWidget(pushButton_left, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        pushButton_up = new QPushButton(groupBox_2);
        pushButton_up->setObjectName(QStringLiteral("pushButton_up"));
        pushButton_up->setMinimumSize(QSize(80, 80));

        gridLayout->addWidget(pushButton_up, 0, 1, 1, 1);

        pushButton_right = new QPushButton(groupBox_2);
        pushButton_right->setObjectName(QStringLiteral("pushButton_right"));
        pushButton_right->setMinimumSize(QSize(80, 80));
        pushButton_right->setMaximumSize(QSize(80, 80));

        gridLayout->addWidget(pushButton_right, 1, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        pushButton_stop = new QPushButton(groupBox_2);
        pushButton_stop->setObjectName(QStringLiteral("pushButton_stop"));

        verticalLayout_2->addWidget(pushButton_stop);


        verticalLayout_4->addWidget(groupBox_2);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(296, 10, 301, 411));
        gridLayout_3 = new QGridLayout(layoutWidget1);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        textBrowser = new QTextBrowser(layoutWidget1);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout_3->addWidget(textBrowser, 1, 0, 1, 1);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        pushButton = new QPushButton(layoutWidget1);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_3->addWidget(pushButton, 2, 0, 1, 1);

        comboBox_serial = new QComboBox(centralWidget);
        comboBox_serial->setObjectName(QStringLiteral("comboBox_serial"));
        comboBox_serial->setGeometry(QRect(630, 20, 261, 25));
        pushButton_serial_connect = new QPushButton(centralWidget);
        pushButton_serial_connect->setObjectName(QStringLiteral("pushButton_serial_connect"));
        pushButton_serial_connect->setGeometry(QRect(630, 50, 89, 25));
        pushButton_serial_disconnect = new QPushButton(centralWidget);
        pushButton_serial_disconnect->setObjectName(QStringLiteral("pushButton_serial_disconnect"));
        pushButton_serial_disconnect->setGeometry(QRect(730, 50, 89, 25));
        lineEdit_path_status = new QLineEdit(centralWidget);
        lineEdit_path_status->setObjectName(QStringLiteral("lineEdit_path_status"));
        lineEdit_path_status->setGeometry(QRect(640, 100, 113, 25));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 440, 301, 27));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        lineEdit_command = new QLineEdit(widget);
        lineEdit_command->setObjectName(QStringLiteral("lineEdit_command"));

        horizontalLayout->addWidget(lineEdit_command);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1506, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label_cam->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "config", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "STATR:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "STOP:", nullptr));
        pushButton_go->setText(QApplication::translate("MainWindow", "GO", nullptr));
#ifndef QT_NO_SHORTCUT
        pushButton_go->setShortcut(QApplication::translate("MainWindow", "Space", nullptr));
#endif // QT_NO_SHORTCUT
        label_5->setText(QApplication::translate("MainWindow", "\346\234\235\345\220\221", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "\347\272\277\351\200\237\345\272\246", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "\350\247\222\351\200\237\345\272\246", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "m/s", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "rad/s", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "ctrl", nullptr));
        pushButton_down->setText(QApplication::translate("MainWindow", "DOWN", nullptr));
#ifndef QT_NO_SHORTCUT
        pushButton_down->setShortcut(QApplication::translate("MainWindow", "Down", nullptr));
#endif // QT_NO_SHORTCUT
        pushButton_left->setText(QApplication::translate("MainWindow", "LEFT", nullptr));
#ifndef QT_NO_SHORTCUT
        pushButton_left->setShortcut(QApplication::translate("MainWindow", "Left", nullptr));
#endif // QT_NO_SHORTCUT
        pushButton_up->setText(QApplication::translate("MainWindow", "UP", nullptr));
#ifndef QT_NO_SHORTCUT
        pushButton_up->setShortcut(QApplication::translate("MainWindow", "Up", nullptr));
#endif // QT_NO_SHORTCUT
        pushButton_right->setText(QApplication::translate("MainWindow", "RIGHT", nullptr));
#ifndef QT_NO_SHORTCUT
        pushButton_right->setShortcut(QApplication::translate("MainWindow", "Right", nullptr));
#endif // QT_NO_SHORTCUT
        pushButton_stop->setText(QApplication::translate("MainWindow", "STOP", nullptr));
#ifndef QT_NO_SHORTCUT
        pushButton_stop->setShortcut(QApplication::translate("MainWindow", "+", nullptr));
#endif // QT_NO_SHORTCUT
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Infomation</span></p></body></html>", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "EXIT", nullptr));
        pushButton_serial_connect->setText(QApplication::translate("MainWindow", "connect", nullptr));
#ifndef QT_NO_SHORTCUT
        pushButton_serial_connect->setShortcut(QApplication::translate("MainWindow", "Enter", nullptr));
#endif // QT_NO_SHORTCUT
        pushButton_serial_disconnect->setText(QApplication::translate("MainWindow", "disconnect", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Command", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
