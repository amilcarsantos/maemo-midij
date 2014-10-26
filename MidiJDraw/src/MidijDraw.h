#ifndef MIDIJDRAW_H
#define MIDIJDRAW_H

#include <QtGui/QMainWindow>
#include <QtGui/QComboBox>
#include <qdom.h>
#include <QScriptEngineDebugger>
#include <QMouseEvent>

#include "ui_MidijDraw.h"
#include "MidijDrawGraphicsScene.h"
#include "midij/mixxx/MidiObject.h"

class MidiMappingDraw;
class QtStringPropertyManager;
class QtIntPropertyManager;
class GraphicsMidiControlItem;
class MidijMidiScriptEngine;
class MidiMapping;

class MidiJDraw : public QMainWindow
{
	Q_OBJECT

public:
	MidiJDraw(QWidget *parent = 0);
	~MidiJDraw();

	void setCurrentProject(QString projectDir, QString guiXmlFile, QString midiXmlFile, QString scriptPrefix);

	void drawHotWheelsSkin();
	void drawHwBrowsingSkin();
//	void drawDados6();

protected:
	void updatePropertiesControl(QDomElement* node, QList<QtProperty*> subProperties);
	void updatePropertiesInput(QDomElement* node, QList<QtProperty*> subProperties);
	void updatePropertiesOutput(QDomElement* node, QList<QtProperty*> subProperties);


	// XML to Property eXchange methods:
	void XPX_StringElem(const QString& tag, QDomElement* node, QtProperty* property, const QString& defaultValue = "");
	void XPX_StringAttr(const QString& tag, QDomElement* node, QtProperty* property, const QString& defaultValue = "");
	void XPX_IntAttr(const QString& tag, QDomElement* node, QtProperty* property, bool readonly = false);
	void controlSelected(GraphicsMidiControlItem *control);
	void updateXmlEdit(QDomElement* node);
	void updateMidiStates(QDomElement* node);


	void controlSetNodeArea(GraphicsMidiControlItem *control, const QString& areaValue, QDomElement* node, QtProperty* areaProperty, GraphicsSelectionItem* selectionItem);

    virtual bool eventFilter(QObject *, QEvent *);

public slots:
	void selectProject();
	void propertyBrowserItemChanged(QtBrowserItem *);
	void intValueChanged(QtProperty *property, int val);
	void areaValueChanged(QtProperty* property,const QString &area);
	void controlBrowserItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void graphicsItemClicked(QGraphicsItem *);
	void sceneRectChanged(const QRectF& rect);
	void itemContextMenu(const QPointF& pos);
	void callStateEditor();
	void zoomIn();
	void zoomOut();
	void midiSysexSelected(QString sysexMsg);
	void midiStateSelected(QString midiMsg);
	void xmlModificationChanged(bool modified);
	void saveXml();
	void reload();
	void sceneSavePng();
	void runScript();
	void stopScript();
//	void debuggerSuspended();
	void debuggerResumed();


private:
	Ui::MidiJDrawClass ui;
	QGraphicsScene* m_scene;
	QComboBox* m_midiSysexCombo;
	QComboBox* m_midiState;
	MidiMappingDraw* m_midiMappingGui;
	QList<QGraphicsItem*> m_guiItems;
	QString m_projectDir;
	QString m_guiXmlFile;
	QString m_midiXmlFile;
	QString m_scriptPrefix;
	bool m_makeBak;

	bool m_mousePressShift;
	MidiMapping* m_midiMapping;
	QScriptEngineDebugger* m_debugger;
	QMainWindow *m_debugWindow;
	MidijMidiScriptEngine* m_scriptEngine;


	QtProperty* m_controlProperties;
	QtProperty* m_inputProperties;
	QtProperty* m_outputProperties;


	QtStringPropertyManager* m_stringManager;
	QtIntPropertyManager* m_readIntManager;

	GraphicsSelectionItem* m_selectionItem;
	GraphicsMidiControlItem* m_selectedControl;
};

#endif // MIDIJDRAW_H
