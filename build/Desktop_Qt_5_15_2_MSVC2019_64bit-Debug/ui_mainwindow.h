/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qwebengineview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWebEngineView *mapView;
    QVBoxLayout *verticalLayout;
    QPushButton *openMyCom1;
    QPushButton *closeMyCom1;
    QLabel *latitudeLabel;
    QLabel *longitudeLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mapView = new QWebEngineView(centralwidget);
        mapView->setObjectName(QString::fromUtf8("mapView"));
        mapView->setMinimumSize(QSize(400, 0));

        horizontalLayout->addWidget(mapView);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        openMyCom1 = new QPushButton(centralwidget);
        openMyCom1->setObjectName(QString::fromUtf8("openMyCom1"));

        verticalLayout->addWidget(openMyCom1);

        closeMyCom1 = new QPushButton(centralwidget);
        closeMyCom1->setObjectName(QString::fromUtf8("closeMyCom1"));
        closeMyCom1->setEnabled(false);

        verticalLayout->addWidget(closeMyCom1);

        latitudeLabel = new QLabel(centralwidget);
        latitudeLabel->setObjectName(QString::fromUtf8("latitudeLabel"));

        verticalLayout->addWidget(latitudeLabel);

        longitudeLabel = new QLabel(centralwidget);
        longitudeLabel->setObjectName(QString::fromUtf8("longitudeLabel"));

        verticalLayout->addWidget(longitudeLabel);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        openMyCom1->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        closeMyCom1->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255\344\270\262\345\217\243", nullptr));
        latitudeLabel->setText(QCoreApplication::translate("MainWindow", "\347\272\254\345\272\246: ", nullptr));
        longitudeLabel->setText(QCoreApplication::translate("MainWindow", "\347\273\217\345\272\246: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
