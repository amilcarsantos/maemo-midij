/********************************************************************************
** Form generated from reading UI file 'MidijStateEditor.ui'
**
** Created: Mon 4. Jul 18:29:58 2011
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIDIJSTATEEDITOR_H
#define UI_MIDIJSTATEEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>

QT_BEGIN_NAMESPACE

class Ui_MidijStateEditorClass
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QComboBox *statesCombo;
    QPushButton *zoomIn;
    QPushButton *zoomOut;
    QFrame *line;
    QCheckBox *gridCheck;
    QComboBox *gridSizeCombo;
    QComboBox *gridExtraCombo;
    QSpacerItem *horizontalSpacer;
    QSplitter *splitter;
    QGraphicsView *graphicsView;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *MidijStateEditorClass)
    {
        if (MidijStateEditorClass->objectName().isEmpty())
            MidijStateEditorClass->setObjectName(QString::fromUtf8("MidijStateEditorClass"));
        MidijStateEditorClass->resize(399, 436);
        gridLayout_2 = new QGridLayout(MidijStateEditorClass);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(2);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(2, 2, -1, 2);
        statesCombo = new QComboBox(MidijStateEditorClass);
        statesCombo->setObjectName(QString::fromUtf8("statesCombo"));

        horizontalLayout->addWidget(statesCombo);

        zoomIn = new QPushButton(MidijStateEditorClass);
        zoomIn->setObjectName(QString::fromUtf8("zoomIn"));
        zoomIn->setContextMenuPolicy(Qt::NoContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/midij/zoom_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomIn->setIcon(icon);
        zoomIn->setIconSize(QSize(24, 24));
        zoomIn->setFlat(true);

        horizontalLayout->addWidget(zoomIn);

        zoomOut = new QPushButton(MidijStateEditorClass);
        zoomOut->setObjectName(QString::fromUtf8("zoomOut"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/midij/zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomOut->setIcon(icon1);
        zoomOut->setIconSize(QSize(24, 24));
        zoomOut->setFlat(true);

        horizontalLayout->addWidget(zoomOut);

        line = new QFrame(MidijStateEditorClass);
        line->setObjectName(QString::fromUtf8("line"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy);
        line->setMaximumSize(QSize(16777215, 24));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        gridCheck = new QCheckBox(MidijStateEditorClass);
        gridCheck->setObjectName(QString::fromUtf8("gridCheck"));
        gridCheck->setChecked(true);

        horizontalLayout->addWidget(gridCheck);

        gridSizeCombo = new QComboBox(MidijStateEditorClass);
        gridSizeCombo->setObjectName(QString::fromUtf8("gridSizeCombo"));

        horizontalLayout->addWidget(gridSizeCombo);

        gridExtraCombo = new QComboBox(MidijStateEditorClass);
        gridExtraCombo->setObjectName(QString::fromUtf8("gridExtraCombo"));

        horizontalLayout->addWidget(gridExtraCombo);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        splitter = new QSplitter(MidijStateEditorClass);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        graphicsView = new QGraphicsView(splitter);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        splitter->addWidget(graphicsView);
        plainTextEdit = new QPlainTextEdit(splitter);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        splitter->addWidget(plainTextEdit);

        gridLayout->addWidget(splitter, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(MidijStateEditorClass);
        QObject::connect(zoomIn, SIGNAL(clicked()), MidijStateEditorClass, SLOT(zoomIn()));
        QObject::connect(zoomOut, SIGNAL(clicked()), MidijStateEditorClass, SLOT(zoomOut()));
        QObject::connect(gridCheck, SIGNAL(clicked(bool)), MidijStateEditorClass, SLOT(gridVisible(bool)));
        QObject::connect(gridSizeCombo, SIGNAL(currentIndexChanged(QString)), MidijStateEditorClass, SLOT(gridSizeSelect(QString)));
        QObject::connect(gridExtraCombo, SIGNAL(currentIndexChanged(QString)), MidijStateEditorClass, SLOT(gridExtraSelect(QString)));
        QObject::connect(statesCombo, SIGNAL(currentIndexChanged(int)), MidijStateEditorClass, SLOT(stateSelect(int)));

        QMetaObject::connectSlotsByName(MidijStateEditorClass);
    } // setupUi

    void retranslateUi(QDialog *MidijStateEditorClass)
    {
        MidijStateEditorClass->setWindowTitle(QApplication::translate("MidijStateEditorClass", "State Editor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        statesCombo->setToolTip(QApplication::translate("MidijStateEditorClass", "Activates a state", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        zoomIn->setToolTip(QApplication::translate("MidijStateEditorClass", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        gridCheck->setText(QApplication::translate("MidijStateEditorClass", "Grid", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        gridSizeCombo->setToolTip(QApplication::translate("MidijStateEditorClass", "Number of items in grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        gridExtraCombo->setToolTip(QApplication::translate("MidijStateEditorClass", "Grid extra space item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class MidijStateEditorClass: public Ui_MidijStateEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIDIJSTATEEDITOR_H
