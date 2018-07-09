/********************************************************************************
** Form generated from reading UI file 'QtGuiApplication1.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGUIAPPLICATION1_H
#define UI_QTGUIAPPLICATION1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGuiApplication1Class
{
public:
    QAction *SiteManage;
    QAction *actionExit;
    QAction *actionDebug_Mode;
    QAction *actionRefresh;
    QAction *actionCancel;
    QAction *actionReconnet;
    QAction *actionDisconnect;
    QAction *actSetting;
    QWidget *centralWidget;
    QListWidget *listWidget_2;
    QLineEdit *hostEdit;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *userEdit;
    QLineEdit *passEdit;
    QLabel *label_3;
    QLineEdit *portEdit;
    QLabel *label_4;
    QPushButton *conBtn;
    QTextEdit *textEdit;
    QTreeView *localTree;
    QTreeView *remoteTree;
    QTreeView *downlistTree;
    QMenuBar *menuBar;
    QMenu *menu_F;
    QMenu *menu_V;
    QMenu *menu_E;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *QtGuiApplication1Class)
    {
        if (QtGuiApplication1Class->objectName().isEmpty())
            QtGuiApplication1Class->setObjectName(QStringLiteral("QtGuiApplication1Class"));
        QtGuiApplication1Class->resize(1002, 780);
        SiteManage = new QAction(QtGuiApplication1Class);
        SiteManage->setObjectName(QStringLiteral("SiteManage"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/QtGuiApplication1/SiteManage"), QSize(), QIcon::Normal, QIcon::Off);
        SiteManage->setIcon(icon);
        actionExit = new QAction(QtGuiApplication1Class);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionDebug_Mode = new QAction(QtGuiApplication1Class);
        actionDebug_Mode->setObjectName(QStringLiteral("actionDebug_Mode"));
        actionRefresh = new QAction(QtGuiApplication1Class);
        actionRefresh->setObjectName(QStringLiteral("actionRefresh"));
        actionRefresh->setCheckable(false);
        actionRefresh->setChecked(false);
        actionRefresh->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/QtGuiApplication1/I:/Skins For GUI Programming/Icon/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRefresh->setIcon(icon1);
        actionCancel = new QAction(QtGuiApplication1Class);
        actionCancel->setObjectName(QStringLiteral("actionCancel"));
        actionCancel->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/QtGuiApplication1/I:/Skins For GUI Programming/Icon/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCancel->setIcon(icon2);
        actionReconnet = new QAction(QtGuiApplication1Class);
        actionReconnet->setObjectName(QStringLiteral("actionReconnet"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/QtGuiApplication1/I:/Skins For GUI Programming/Icon/reconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReconnet->setIcon(icon3);
        actionDisconnect = new QAction(QtGuiApplication1Class);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        actionDisconnect->setEnabled(false);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/QtGuiApplication1/I:/Skins For GUI Programming/Icon/disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon4);
        actSetting = new QAction(QtGuiApplication1Class);
        actSetting->setObjectName(QStringLiteral("actSetting"));
        centralWidget = new QWidget(QtGuiApplication1Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        listWidget_2 = new QListWidget(centralWidget);
        listWidget_2->setObjectName(QStringLiteral("listWidget_2"));
        listWidget_2->setGeometry(QRect(0, 0, 1001, 51));
        hostEdit = new QLineEdit(centralWidget);
        hostEdit->setObjectName(QStringLiteral("hostEdit"));
        hostEdit->setGeometry(QRect(60, 10, 131, 31));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 56, 12));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(220, 20, 56, 12));
        userEdit = new QLineEdit(centralWidget);
        userEdit->setObjectName(QStringLiteral("userEdit"));
        userEdit->setGeometry(QRect(260, 10, 131, 31));
        passEdit = new QLineEdit(centralWidget);
        passEdit->setObjectName(QStringLiteral("passEdit"));
        passEdit->setGeometry(QRect(470, 10, 131, 31));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(430, 20, 56, 12));
        portEdit = new QLineEdit(centralWidget);
        portEdit->setObjectName(QStringLiteral("portEdit"));
        portEdit->setGeometry(QRect(680, 10, 51, 31));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(640, 20, 56, 12));
        conBtn = new QPushButton(centralWidget);
        conBtn->setObjectName(QStringLiteral("conBtn"));
        conBtn->setGeometry(QRect(750, 10, 111, 31));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setEnabled(true);
        textEdit->setGeometry(QRect(0, 50, 1001, 251));
        textEdit->setAcceptDrops(false);
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        textEdit->setTabChangesFocus(true);
        textEdit->setReadOnly(true);
        localTree = new QTreeView(centralWidget);
        localTree->setObjectName(QStringLiteral("localTree"));
        localTree->setGeometry(QRect(0, 310, 461, 181));
        remoteTree = new QTreeView(centralWidget);
        remoteTree->setObjectName(QStringLiteral("remoteTree"));
        remoteTree->setGeometry(QRect(490, 310, 511, 181));
        downlistTree = new QTreeView(centralWidget);
        downlistTree->setObjectName(QStringLiteral("downlistTree"));
        downlistTree->setGeometry(QRect(0, 500, 1001, 201));
        QtGuiApplication1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtGuiApplication1Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1002, 21));
        menu_F = new QMenu(menuBar);
        menu_F->setObjectName(QStringLiteral("menu_F"));
        menu_V = new QMenu(menuBar);
        menu_V->setObjectName(QStringLiteral("menu_V"));
        menu_E = new QMenu(menuBar);
        menu_E->setObjectName(QStringLiteral("menu_E"));
        QtGuiApplication1Class->setMenuBar(menuBar);
        statusBar = new QStatusBar(QtGuiApplication1Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtGuiApplication1Class->setStatusBar(statusBar);
        toolBar = new QToolBar(QtGuiApplication1Class);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setEnabled(true);
        toolBar->setAcceptDrops(false);
        toolBar->setIconSize(QSize(24, 24));
        QtGuiApplication1Class->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menu_F->menuAction());
        menuBar->addAction(menu_E->menuAction());
        menuBar->addAction(menu_V->menuAction());
        menu_F->addAction(SiteManage);
        menu_F->addSeparator();
        menu_F->addAction(actionExit);
        menu_V->addAction(actionDebug_Mode);
        menu_E->addAction(actSetting);
        toolBar->addAction(SiteManage);
        toolBar->addSeparator();
        toolBar->addAction(actionRefresh);
        toolBar->addAction(actionCancel);
        toolBar->addAction(actionDisconnect);
        toolBar->addAction(actionReconnet);

        retranslateUi(QtGuiApplication1Class);

        QMetaObject::connectSlotsByName(QtGuiApplication1Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtGuiApplication1Class)
    {
        QtGuiApplication1Class->setWindowTitle(QApplication::translate("QtGuiApplication1Class", "QtGuiApplication1", nullptr));
        SiteManage->setText(QApplication::translate("QtGuiApplication1Class", "SiteManage", nullptr));
        actionExit->setText(QApplication::translate("QtGuiApplication1Class", "Exit", nullptr));
        actionDebug_Mode->setText(QApplication::translate("QtGuiApplication1Class", "Debug Mode", nullptr));
        actionRefresh->setText(QApplication::translate("QtGuiApplication1Class", "Refresh", nullptr));
        actionCancel->setText(QApplication::translate("QtGuiApplication1Class", "cancel", nullptr));
        actionReconnet->setText(QApplication::translate("QtGuiApplication1Class", "reconnect", nullptr));
        actionDisconnect->setText(QApplication::translate("QtGuiApplication1Class", "disconnect", nullptr));
        actSetting->setText(QApplication::translate("QtGuiApplication1Class", "Setting(S)", nullptr));
        label->setText(QApplication::translate("QtGuiApplication1Class", "Host", nullptr));
        label_2->setText(QApplication::translate("QtGuiApplication1Class", "User", nullptr));
        label_3->setText(QApplication::translate("QtGuiApplication1Class", "Pass", nullptr));
        label_4->setText(QApplication::translate("QtGuiApplication1Class", "Port", nullptr));
        conBtn->setText(QApplication::translate("QtGuiApplication1Class", "Connect", nullptr));
        menu_F->setTitle(QApplication::translate("QtGuiApplication1Class", "\355\214\214\354\235\274(F)", nullptr));
        menu_V->setTitle(QApplication::translate("QtGuiApplication1Class", "\353\263\264\352\270\260(V)", nullptr));
        menu_E->setTitle(QApplication::translate("QtGuiApplication1Class", "\355\216\270\354\247\221(E)", nullptr));
        toolBar->setWindowTitle(QApplication::translate("QtGuiApplication1Class", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtGuiApplication1Class: public Ui_QtGuiApplication1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGUIAPPLICATION1_H
