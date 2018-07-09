/********************************************************************************
** Form generated from reading UI file 'FtpSettingWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FTPSETTINGWIDGET_H
#define UI_FTPSETTINGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FtpSettingWidget
{
public:
    QPushButton *okBtn;
    QPushButton *closeBtn;
    QComboBox *logLevelBox;
    QLabel *label;
    QComboBox *modeBox;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *pathEdit;

    void setupUi(QWidget *FtpSettingWidget)
    {
        if (FtpSettingWidget->objectName().isEmpty())
            FtpSettingWidget->setObjectName(QStringLiteral("FtpSettingWidget"));
        FtpSettingWidget->resize(455, 223);
        okBtn = new QPushButton(FtpSettingWidget);
        okBtn->setObjectName(QStringLiteral("okBtn"));
        okBtn->setGeometry(QRect(130, 160, 75, 31));
        closeBtn = new QPushButton(FtpSettingWidget);
        closeBtn->setObjectName(QStringLiteral("closeBtn"));
        closeBtn->setGeometry(QRect(230, 160, 75, 31));
        logLevelBox = new QComboBox(FtpSettingWidget);
        logLevelBox->setObjectName(QStringLiteral("logLevelBox"));
        logLevelBox->setGeometry(QRect(100, 30, 101, 21));
        label = new QLabel(FtpSettingWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 30, 71, 21));
        modeBox = new QComboBox(FtpSettingWidget);
        modeBox->setObjectName(QStringLiteral("modeBox"));
        modeBox->setGeometry(QRect(100, 70, 101, 21));
        label_2 = new QLabel(FtpSettingWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 70, 71, 21));
        label_3 = new QLabel(FtpSettingWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 110, 71, 21));
        pathEdit = new QLineEdit(FtpSettingWidget);
        pathEdit->setObjectName(QStringLiteral("pathEdit"));
        pathEdit->setGeometry(QRect(100, 110, 101, 20));

        retranslateUi(FtpSettingWidget);

        QMetaObject::connectSlotsByName(FtpSettingWidget);
    } // setupUi

    void retranslateUi(QWidget *FtpSettingWidget)
    {
        FtpSettingWidget->setWindowTitle(QApplication::translate("FtpSettingWidget", "FtpSettingWidget", nullptr));
        okBtn->setText(QApplication::translate("FtpSettingWidget", "\355\231\225\354\235\270", nullptr));
        closeBtn->setText(QApplication::translate("FtpSettingWidget", "\354\267\250\354\206\214", nullptr));
        label->setText(QApplication::translate("FtpSettingWidget", "LogLevel", nullptr));
        label_2->setText(QApplication::translate("FtpSettingWidget", "Mode", nullptr));
        label_3->setText(QApplication::translate("FtpSettingWidget", "Local DIR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FtpSettingWidget: public Ui_FtpSettingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FTPSETTINGWIDGET_H
