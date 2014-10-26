/********************************************************************************
** Form generated from reading UI file 'MidijDraw.ui'
**
** Created: Mon 17. Oct 14:01:04 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIDIJDRAW_H
#define UI_MIDIJDRAW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <qttreepropertybrowser.h>

QT_BEGIN_NAMESPACE

class Ui_MidiJDrawClass
{
public:
    QAction *actionExit;
    QAction *actionSelectProject;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionSave;
    QAction *actionRunScript;
    QAction *actionTriggerBreakpoint;
    QAction *actionStopScript;
    QAction *actionReload;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QDockWidget *projectFrame;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QTreeView *treeView;
    QToolBar *navigationToolbar;
    QDockWidget *controlsFrame;
    QWidget *dockWidgetContents_3;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QTreeWidget *controlBrowser;
    QDockWidget *xmlFrame;
    QWidget *dockWidgetContents_4;
    QGridLayout *gridLayout_5;
    QPlainTextEdit *xmlEdit;
    QDockWidget *propertyFrame;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout_3;
    QtTreePropertyBrowser *propertyBrowser;

    void setupUi(QMainWindow *MidiJDrawClass)
    {
        if (MidiJDrawClass->objectName().isEmpty())
            MidiJDrawClass->setObjectName(QString::fromUtf8("MidiJDrawClass"));
        MidiJDrawClass->resize(900, 640);
        actionExit = new QAction(MidiJDrawClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionSelectProject = new QAction(MidiJDrawClass);
        actionSelectProject->setObjectName(QString::fromUtf8("actionSelectProject"));
        actionZoomIn = new QAction(MidiJDrawClass);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/midij/zoom_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomIn->setIcon(icon);
        actionZoomOut = new QAction(MidiJDrawClass);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/midij/zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomOut->setIcon(icon1);
        actionSave = new QAction(MidiJDrawClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/midij/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon2);
        actionRunScript = new QAction(MidiJDrawClass);
        actionRunScript->setObjectName(QString::fromUtf8("actionRunScript"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/midij/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRunScript->setIcon(icon3);
        actionTriggerBreakpoint = new QAction(MidiJDrawClass);
        actionTriggerBreakpoint->setObjectName(QString::fromUtf8("actionTriggerBreakpoint"));
        actionTriggerBreakpoint->setEnabled(false);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/midij/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTriggerBreakpoint->setIcon(icon4);
        actionStopScript = new QAction(MidiJDrawClass);
        actionStopScript->setObjectName(QString::fromUtf8("actionStopScript"));
        actionStopScript->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/midij/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStopScript->setIcon(icon5);
        actionReload = new QAction(MidiJDrawClass);
        actionReload->setObjectName(QString::fromUtf8("actionReload"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/midij/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReload->setIcon(icon6);
        centralwidget = new QWidget(MidiJDrawClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 1, 0, 0);
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout_2->addWidget(graphicsView, 0, 0, 1, 1);

        MidiJDrawClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MidiJDrawClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 900, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MidiJDrawClass->setMenuBar(menubar);
        statusbar = new QStatusBar(MidiJDrawClass);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MidiJDrawClass->setStatusBar(statusbar);
        projectFrame = new QDockWidget(MidiJDrawClass);
        projectFrame->setObjectName(QString::fromUtf8("projectFrame"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        treeView = new QTreeView(dockWidgetContents);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->header()->setVisible(false);

        gridLayout->addWidget(treeView, 0, 0, 1, 1);

        projectFrame->setWidget(dockWidgetContents);
        MidiJDrawClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), projectFrame);
        navigationToolbar = new QToolBar(MidiJDrawClass);
        navigationToolbar->setObjectName(QString::fromUtf8("navigationToolbar"));
        MidiJDrawClass->addToolBar(Qt::TopToolBarArea, navigationToolbar);
        controlsFrame = new QDockWidget(MidiJDrawClass);
        controlsFrame->setObjectName(QString::fromUtf8("controlsFrame"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        gridLayout_4 = new QGridLayout(dockWidgetContents_3);
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton = new QPushButton(dockWidgetContents_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        controlBrowser = new QTreeWidget(dockWidgetContents_3);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(1, QString::fromUtf8("2"));
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        controlBrowser->setHeaderItem(__qtreewidgetitem);
        controlBrowser->setObjectName(QString::fromUtf8("controlBrowser"));
        controlBrowser->setColumnCount(3);

        verticalLayout->addWidget(controlBrowser);


        gridLayout_4->addLayout(verticalLayout, 0, 0, 1, 1);

        controlsFrame->setWidget(dockWidgetContents_3);
        MidiJDrawClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), controlsFrame);
        xmlFrame = new QDockWidget(MidiJDrawClass);
        xmlFrame->setObjectName(QString::fromUtf8("xmlFrame"));
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        gridLayout_5 = new QGridLayout(dockWidgetContents_4);
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        xmlEdit = new QPlainTextEdit(dockWidgetContents_4);
        xmlEdit->setObjectName(QString::fromUtf8("xmlEdit"));
        xmlEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        xmlEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        gridLayout_5->addWidget(xmlEdit, 0, 0, 1, 1);

        xmlFrame->setWidget(dockWidgetContents_4);
        MidiJDrawClass->addDockWidget(static_cast<Qt::DockWidgetArea>(8), xmlFrame);
        propertyFrame = new QDockWidget(MidiJDrawClass);
        propertyFrame->setObjectName(QString::fromUtf8("propertyFrame"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        gridLayout_3 = new QGridLayout(dockWidgetContents_2);
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        propertyBrowser = new QtTreePropertyBrowser(dockWidgetContents_2);
        propertyBrowser->setObjectName(QString::fromUtf8("propertyBrowser"));
        propertyBrowser->setProperty("alternatingRowColors", QVariant(true));

        gridLayout_3->addWidget(propertyBrowser, 0, 0, 1, 1);

        propertyFrame->setWidget(dockWidgetContents_2);
        MidiJDrawClass->addDockWidget(static_cast<Qt::DockWidgetArea>(8), propertyFrame);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionSelectProject);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        navigationToolbar->addAction(actionSave);
        navigationToolbar->addAction(actionReload);
        navigationToolbar->addAction(actionZoomIn);
        navigationToolbar->addAction(actionZoomOut);
        navigationToolbar->addSeparator();
        navigationToolbar->addAction(actionRunScript);
        navigationToolbar->addAction(actionTriggerBreakpoint);
        navigationToolbar->addAction(actionStopScript);
        navigationToolbar->addSeparator();

        retranslateUi(MidiJDrawClass);
        QObject::connect(actionExit, SIGNAL(triggered()), MidiJDrawClass, SLOT(close()));
        QObject::connect(actionSelectProject, SIGNAL(triggered()), MidiJDrawClass, SLOT(selectProject()));
        QObject::connect(actionZoomIn, SIGNAL(triggered()), MidiJDrawClass, SLOT(zoomIn()));
        QObject::connect(actionZoomOut, SIGNAL(triggered()), MidiJDrawClass, SLOT(zoomOut()));
        QObject::connect(actionSave, SIGNAL(triggered()), MidiJDrawClass, SLOT(saveXml()));
        QObject::connect(actionReload, SIGNAL(triggered()), MidiJDrawClass, SLOT(reload()));
        QObject::connect(actionRunScript, SIGNAL(triggered()), MidiJDrawClass, SLOT(runScript()));
        QObject::connect(actionStopScript, SIGNAL(triggered()), MidiJDrawClass, SLOT(stopScript()));

        QMetaObject::connectSlotsByName(MidiJDrawClass);
    } // setupUi

    void retranslateUi(QMainWindow *MidiJDrawClass)
    {
        MidiJDrawClass->setWindowTitle(QApplication::translate("MidiJDrawClass", "MidiJ Creator", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MidiJDrawClass", "Exit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("MidiJDrawClass", "Exit Application", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExit->setShortcut(QApplication::translate("MidiJDrawClass", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionSelectProject->setText(QApplication::translate("MidiJDrawClass", "Select project", 0, QApplication::UnicodeUTF8));
        actionZoomIn->setText(QApplication::translate("MidiJDrawClass", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoomOut->setText(QApplication::translate("MidiJDrawClass", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MidiJDrawClass", "Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("MidiJDrawClass", "Save XML File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRunScript->setText(QApplication::translate("MidiJDrawClass", "Run Script", 0, QApplication::UnicodeUTF8));
        actionTriggerBreakpoint->setText(QApplication::translate("MidiJDrawClass", "Trigger Breakpoint", 0, QApplication::UnicodeUTF8));
        actionStopScript->setText(QApplication::translate("MidiJDrawClass", "Stop Script", 0, QApplication::UnicodeUTF8));
        actionReload->setText(QApplication::translate("MidiJDrawClass", "Reload", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MidiJDrawClass", "File", 0, QApplication::UnicodeUTF8));
        projectFrame->setWindowTitle(QApplication::translate("MidiJDrawClass", "Project", 0, QApplication::UnicodeUTF8));
        navigationToolbar->setWindowTitle(QApplication::translate("MidiJDrawClass", "Navigation", 0, QApplication::UnicodeUTF8));
        controlsFrame->setWindowTitle(QApplication::translate("MidiJDrawClass", "Control Inspector", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MidiJDrawClass", "PushButton", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = controlBrowser->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("MidiJDrawClass", "3", 0, QApplication::UnicodeUTF8));
        xmlFrame->setWindowTitle(QApplication::translate("MidiJDrawClass", "XML", 0, QApplication::UnicodeUTF8));
        propertyFrame->setWindowTitle(QApplication::translate("MidiJDrawClass", "Property Browser", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MidiJDrawClass: public Ui_MidiJDrawClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIDIJDRAW_H
