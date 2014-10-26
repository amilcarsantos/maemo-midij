/********************************************************************************
** Form generated from reading UI file 'MidiJGw.ui'
**
** Created: Tue 11. Sep 22:59:14 2012
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIDIJGW_H
#define UI_MIDIJGW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "lightline.h"

QT_BEGIN_NAMESPACE

class Ui_midiJGwClass
{
public:
    QAction *actionDirectMidi;
    QAction *actionDualDirectMidi;
    QGridLayout *gridLayout;
    QVBoxLayout *windowLayout;
    QLabel *label;
    QLabel *midiJGwLabel;
    QLabel *errorLabel;
    QWidget *controlsWidget;
    QHBoxLayout *controlsLayout;
    QSpacerItem *leftSpacer;
    QVBoxLayout *midiControlsLayout;
    QCommandLinkButton *midiController1;
    QCommandLinkButton *midiController2;
    QVBoxLayout *midiLinesLayout;
    LightLine *midiController1Line;
    LightLine *midiController2Line;
    QCommandLinkButton *gatewayController;
    LightLine *gatewayControllerLine;
    QCommandLinkButton *midiOutput;
    QSpacerItem *rightSpacer;
    QFormLayout *formLayout;
    QLabel *MIDI_DEBUG;
    QFrame *midiController1LineOLD;

    void setupUi(QWidget *midiJGwClass)
    {
        if (midiJGwClass->objectName().isEmpty())
            midiJGwClass->setObjectName(QString::fromUtf8("midiJGwClass"));
        midiJGwClass->resize(703, 220);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/midijgw/midijgw_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        midiJGwClass->setWindowIcon(icon);
        actionDirectMidi = new QAction(midiJGwClass);
        actionDirectMidi->setObjectName(QString::fromUtf8("actionDirectMidi"));
        actionDualDirectMidi = new QAction(midiJGwClass);
        actionDualDirectMidi->setObjectName(QString::fromUtf8("actionDualDirectMidi"));
        gridLayout = new QGridLayout(midiJGwClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        windowLayout = new QVBoxLayout();
        windowLayout->setSpacing(0);
        windowLayout->setObjectName(QString::fromUtf8("windowLayout"));
        label = new QLabel(midiJGwClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 20));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));

        windowLayout->addWidget(label);

        midiJGwLabel = new QLabel(midiJGwClass);
        midiJGwLabel->setObjectName(QString::fromUtf8("midiJGwLabel"));
        midiJGwLabel->setMaximumSize(QSize(16777215, 80));
        QFont font;
        font.setFamily(QString::fromUtf8("Trebuchet MS"));
        font.setPointSize(24);
        font.setBold(true);
        font.setWeight(75);
        midiJGwLabel->setFont(font);
        midiJGwLabel->setAutoFillBackground(false);
        midiJGwLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);"));
        midiJGwLabel->setTextFormat(Qt::PlainText);
        midiJGwLabel->setMargin(0);
        midiJGwLabel->setIndent(20);

        windowLayout->addWidget(midiJGwLabel);

        errorLabel = new QLabel(midiJGwClass);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setMinimumSize(QSize(0, 20));
        errorLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        errorLabel->setAlignment(Qt::AlignCenter);

        windowLayout->addWidget(errorLabel);

        controlsWidget = new QWidget(midiJGwClass);
        controlsWidget->setObjectName(QString::fromUtf8("controlsWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(controlsWidget->sizePolicy().hasHeightForWidth());
        controlsWidget->setSizePolicy(sizePolicy);
        controlsWidget->setMinimumSize(QSize(0, 96));
        controlsLayout = new QHBoxLayout(controlsWidget);
        controlsLayout->setSpacing(4);
        controlsLayout->setContentsMargins(0, 0, 0, 0);
        controlsLayout->setObjectName(QString::fromUtf8("controlsLayout"));
        leftSpacer = new QSpacerItem(13, 39, QSizePolicy::Expanding, QSizePolicy::Minimum);

        controlsLayout->addItem(leftSpacer);

        midiControlsLayout = new QVBoxLayout();
        midiControlsLayout->setSpacing(4);
        midiControlsLayout->setObjectName(QString::fromUtf8("midiControlsLayout"));
        midiController1 = new QCommandLinkButton(controlsWidget);
        midiController1->setObjectName(QString::fromUtf8("midiController1"));
        midiController1->setMaximumSize(QSize(16777215, 54));

        midiControlsLayout->addWidget(midiController1);

        midiController2 = new QCommandLinkButton(controlsWidget);
        midiController2->setObjectName(QString::fromUtf8("midiController2"));
        midiController2->setMaximumSize(QSize(16777215, 54));

        midiControlsLayout->addWidget(midiController2);


        controlsLayout->addLayout(midiControlsLayout);

        midiLinesLayout = new QVBoxLayout();
        midiLinesLayout->setSpacing(6);
        midiLinesLayout->setObjectName(QString::fromUtf8("midiLinesLayout"));
        midiController1Line = new LightLine(controlsWidget);
        midiController1Line->setObjectName(QString::fromUtf8("midiController1Line"));
        midiController1Line->setMinimumSize(QSize(20, 0));
        midiController1Line->setFrameShape(QFrame::HLine);
        midiController1Line->setFrameShadow(QFrame::Sunken);
        midiController1Line->setMidLineWidth(2);

        midiLinesLayout->addWidget(midiController1Line);

        midiController2Line = new LightLine(controlsWidget);
        midiController2Line->setObjectName(QString::fromUtf8("midiController2Line"));
        midiController2Line->setMinimumSize(QSize(20, 0));
        midiController2Line->setFrameShape(QFrame::HLine);
        midiController2Line->setFrameShadow(QFrame::Sunken);
        midiController2Line->setMidLineWidth(2);

        midiLinesLayout->addWidget(midiController2Line);


        controlsLayout->addLayout(midiLinesLayout);

        gatewayController = new QCommandLinkButton(controlsWidget);
        gatewayController->setObjectName(QString::fromUtf8("gatewayController"));
        gatewayController->setMaximumSize(QSize(16777215, 54));

        controlsLayout->addWidget(gatewayController);

        gatewayControllerLine = new LightLine(controlsWidget);
        gatewayControllerLine->setObjectName(QString::fromUtf8("gatewayControllerLine"));
        gatewayControllerLine->setMinimumSize(QSize(20, 0));
        gatewayControllerLine->setFrameShape(QFrame::HLine);
        gatewayControllerLine->setFrameShadow(QFrame::Sunken);
        gatewayControllerLine->setMidLineWidth(2);

        controlsLayout->addWidget(gatewayControllerLine);

        midiOutput = new QCommandLinkButton(controlsWidget);
        midiOutput->setObjectName(QString::fromUtf8("midiOutput"));
        midiOutput->setMaximumSize(QSize(16777215, 54));

        controlsLayout->addWidget(midiOutput);

        rightSpacer = new QSpacerItem(13, 39, QSizePolicy::Expanding, QSizePolicy::Minimum);

        controlsLayout->addItem(rightSpacer);


        windowLayout->addWidget(controlsWidget);


        gridLayout->addLayout(windowLayout, 0, 0, 1, 1);

        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setHorizontalSpacing(0);
        formLayout->setVerticalSpacing(0);
        MIDI_DEBUG = new QLabel(midiJGwClass);
        MIDI_DEBUG->setObjectName(QString::fromUtf8("MIDI_DEBUG"));

        formLayout->setWidget(1, QFormLayout::FieldRole, MIDI_DEBUG);

        midiController1LineOLD = new QFrame(midiJGwClass);
        midiController1LineOLD->setObjectName(QString::fromUtf8("midiController1LineOLD"));
        midiController1LineOLD->setMinimumSize(QSize(20, 0));
        midiController1LineOLD->setLineWidth(1);
        midiController1LineOLD->setMidLineWidth(2);
        midiController1LineOLD->setFrameShape(QFrame::HLine);
        midiController1LineOLD->setFrameShadow(QFrame::Sunken);

        formLayout->setWidget(0, QFormLayout::FieldRole, midiController1LineOLD);


        gridLayout->addLayout(formLayout, 1, 0, 1, 1);


        retranslateUi(midiJGwClass);

        QMetaObject::connectSlotsByName(midiJGwClass);
    } // setupUi

    void retranslateUi(QWidget *midiJGwClass)
    {
        midiJGwClass->setWindowTitle(QApplication::translate("midiJGwClass", "MidiJ Gateway", 0, QApplication::UnicodeUTF8));
        actionDirectMidi->setText(QApplication::translate("midiJGwClass", "DirectMidi", 0, QApplication::UnicodeUTF8));
        actionDualDirectMidi->setText(QApplication::translate("midiJGwClass", "DualDirectMidi", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        midiJGwLabel->setText(QApplication::translate("midiJGwClass", "MidiJ Gateway", 0, QApplication::UnicodeUTF8));
        errorLabel->setText(QApplication::translate("midiJGwClass", "ERROR", 0, QApplication::UnicodeUTF8));
        midiController1->setText(QApplication::translate("midiJGwClass", "MidiJ - UDP", 0, QApplication::UnicodeUTF8));
        midiController1->setDescription(QApplication::translate("midiJGwClass", "MIDI Controller", 0, QApplication::UnicodeUTF8));
        midiController2->setText(QApplication::translate("midiJGwClass", "MidiJ - UDP", 0, QApplication::UnicodeUTF8));
        midiController2->setDescription(QApplication::translate("midiJGwClass", "MIDI Controller", 0, QApplication::UnicodeUTF8));
        gatewayController->setText(QApplication::translate("midiJGwClass", "Dual Direct MIDI", 0, QApplication::UnicodeUTF8));
        gatewayController->setDescription(QApplication::translate("midiJGwClass", "Gateway Controller", 0, QApplication::UnicodeUTF8));
        midiOutput->setText(QApplication::translate("midiJGwClass", "LoopBE 1", 0, QApplication::UnicodeUTF8));
        midiOutput->setDescription(QApplication::translate("midiJGwClass", "MIDI Output", 0, QApplication::UnicodeUTF8));
        MIDI_DEBUG->setText(QApplication::translate("midiJGwClass", "DEBUG:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class midiJGwClass: public Ui_midiJGwClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIDIJGW_H
