#include "MidijDraw.h"
#include <QtGui>
//#include <QFileDialog>
#include <qtpropertymanager.h>
#include <qteditorfactory.h>

#include "MidiMappingDraw.h"
#include "midij/gui/GraphicsMidiControlItem.h"
#include "midij/gui/GuiUtils.h"
#include "midij/Utils.h"

#include "MidijMidiScriptEngine.h"
#include "MidijMidiObject.h"
#include "midij/mixxx/MidiMapping.h"
#include "XmlHighlighter.h"


#define MIDIJDRAW_CONTROL_NAME		100

QDomElement* midiControlXmlElement(GraphicsMidiControlItem* control, int midiControlXmlElement = 0)
{
	if (midiControlXmlElement > 0)
		return (QDomElement*) control->data(midiControlXmlElement).value<void*>();

	if (!control->hasOutput())
	{
		midiControlXmlElement = MIDICONTROL_XMLELEMENT_INPUT;
	}
	else
	{
		midiControlXmlElement = MIDICONTROL_XMLELEMENT_OUTPUT;
	}
	return (QDomElement*) control->data(midiControlXmlElement).value<void*>();;
}


QtProperty* findSubProperty(QtProperty* parentProperty, const QString& propertyName)
{
	foreach(QtProperty* property, parentProperty->subProperties())
	{
		if (property->propertyName() == propertyName)
			return property;
	}
	return NULL;
}

QRect areaToRect(const QString& areaCoords)
{
	QStringList coords = areaCoords.split(",");
	if (coords.size() != 4)
	{
		//qCritical() << "invalid area size:" << areaCoords << "; line:" << areaNode.lineNumber();
		return QRect();
	}
	return QRect(coords.at(0).toInt(), coords.at(1).toInt(), coords.at(2).toInt(), coords.at(3).toInt());
}



MidiJDraw::MidiJDraw(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

//	QDockWidget* toolbarDock = new QDockWidget("Toolbar", this);
//	QMainWindow* toolbarContainer = new QMainWindow(this);
//	toolbarContainer->setWindowFlags(Qt::Widget);
//	toolbarContainer->addToolBar(ui.navigationToolbar);
//	toolbarDock->setWidget(toolbarContainer);
//	addDockWidget(Qt::TopDockWidgetArea, toolbarDock);

	m_mousePressShift = false;
	m_debugger = NULL;
	m_debugWindow = NULL;
	m_scriptEngine = new MidijMidiScriptEngine();

	m_midiMappingGui = new MidiMappingDraw();


	//QPixmap bg = QPixmap("../MidiJ/data/schemes/collusion/midij_collusion06_b_fxeq_black.png");//QPixmap(480,800);
	//bg.fill(Qt::black);

	// GRAPHICS
	m_scene = new MidijDrawGraphicsScene(this);
	m_scene->setSceneRect(QRect(QPoint(), QSize(480, 800)));
	//scene->addPixmap(bg);

	ui.graphicsView->setScene(m_scene);
	m_selectionItem = new GraphicsSelectionItem();
	m_scene->addItem(m_selectionItem);
	connect(m_selectionItem, SIGNAL(rectChanged(const QRectF &)), SLOT(sceneRectChanged(const QRectF &)));
	connect(m_selectionItem, SIGNAL(contextMenu(const QPointF &)), SLOT(itemContextMenu(const QPointF &)));
	connect(m_scene, SIGNAL(itemClicked(QGraphicsItem*)), SLOT(graphicsItemClicked(QGraphicsItem*)));

	// PROPERTY BROWSER
	m_readIntManager = new QtIntPropertyManager(this);
	QtIntPropertyManager* intManager = new QtIntPropertyManager(this);
	QtSpinBoxFactory* spinEditor = new QtSpinBoxFactory(this);
	connect(intManager, SIGNAL(valueChanged(QtProperty*,int)), SLOT(intValueChanged(QtProperty*,int)));
	ui.propertyBrowser->setFactoryForManager(intManager, spinEditor);

	m_stringManager = new QtStringPropertyManager(this);
	QtLineEditFactory* editEditor = new QtLineEditFactory(this);
	//connect(intManager, SIGNAL(valueChanged(QtProperty *, int)), SLOT(intValueChanged(QtProperty *, int)));
	ui.propertyBrowser->setFactoryForManager(m_stringManager, editEditor);
	connect(m_stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)), SLOT(areaValueChanged(QtProperty*,const QString &)));

	QtGroupPropertyManager* groupManager = new QtGroupPropertyManager(this);

	m_controlProperties = groupManager->addProperty("Control");
	m_inputProperties = groupManager->addProperty("Input");
	m_outputProperties = groupManager->addProperty("Output");

	QtProperty* name = m_stringManager->addProperty("name");
	QtProperty* lineNumber = m_readIntManager->addProperty("lineNumber");
	//teste->setEnabled(false);
	m_controlProperties->addSubProperty(name);
	m_controlProperties->addSubProperty(lineNumber);

	QtProperty* area = m_stringManager->addProperty("area");
	QtProperty* midiNo = m_stringManager->addProperty("midino");
	//teste->setEnabled(false);
	m_outputProperties->addSubProperty(m_stringManager->addProperty("type"));
	m_outputProperties->addSubProperty(midiNo);
	m_outputProperties->addSubProperty(area);

	//teste->setEnabled(false);
	m_inputProperties->addSubProperty(midiNo);
	m_inputProperties->addSubProperty(area);
	//QtProperty* state = groupManager->addProperty("State");
	QtProperty* state = m_stringManager->addProperty("state");
	state->addSubProperty(intManager->addProperty("value"));
	//state->addSubProperty(stringManager->addProperty("Image"));
	m_inputProperties->addSubProperty(state);

//	ui.propertyBrowser->addProperty(group);
	ui.propertyBrowser->setIndentation(10);

//	connect(ui.propertyBrowser, SIGNAL(currentItemChanged(QtBrowserItem *)), SLOT(propertyBrowserItemChanged(QtBrowserItem *)));


	// TOOLBAR ITEMS
	QStringList labels;
	labels << "Control" << "Class" << "MIDI No";
	ui.controlBrowser->setHeaderLabels(labels);

	m_midiSysexCombo = new QComboBox(this);
	m_midiSysexCombo->setToolTip("MIDI Sysex messages");
	connect(m_midiSysexCombo, SIGNAL(currentIndexChanged(QString)), SLOT(midiSysexSelected(QString)));
	m_midiState = new QComboBox(this);
	m_midiState->setToolTip("Control MIDI state");
	connect(m_midiState, SIGNAL(currentIndexChanged(QString)), SLOT(midiStateSelected(QString)));
	ui.navigationToolbar->addWidget(m_midiSysexCombo);
	ui.navigationToolbar->addWidget(m_midiState);

	connect(ui.xmlEdit, SIGNAL(modificationChanged(bool)), SLOT(xmlModificationChanged(bool)));

	// MidiControl -> property browser
	connect(ui.controlBrowser, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), SLOT(controlBrowserItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

	new XmlHighlighter(ui.xmlEdit->document(), this);
}

MidiJDraw::~MidiJDraw()
{
	/* FIXME foreach(MidiCtrl*)
	 * if data(MIDICONROL_XML_INPUT) delete xmlelement;
	 */
	stopScript();
	qDeleteAll(m_guiItems);
	delete m_selectionItem;
}


void MidiJDraw::selectProject()
{
	QFileDialog dlg;



	//dlg.setOption(QFileDialog::ShowDirsOnly, true);
	dlg.setFileMode(QFileDialog::DirectoryOnly);

	if (dlg.exec())
		setCurrentProject(dlg.selectedFiles().at(0), "midijGui_Collusion.xml", "", "");

}

void MidiJDraw::propertyBrowserItemChanged(QtBrowserItem *item)
{
	qDebug(__PRETTY_FUNCTION__);

	// indica a o utilizador mudou de seleccao
	QString debug = QString("item changed %1; value=%2").arg(item->property()->propertyName()).arg(item->property()->valueText());


	setWindowTitle(debug);
}


void MidiJDraw::intValueChanged(QtProperty *property, int val)
{
	QString debug = QString("item changed %1; value=%2")
			.arg(property->propertyName())
			.arg(val);
// XXX
	setWindowTitle(debug);
}

void MidiJDraw::areaValueChanged(QtProperty* property, const QString &area)
{
	qDebug(__PRETTY_FUNCTION__);

	if (property->propertyName() != "area" || m_selectedControl == NULL)
		return;
	QDomElement* node = NULL;
	if (!m_selectedControl->hasOutput())
	{
		node = (QDomElement*) m_selectedControl->data(MIDICONTROL_XMLELEMENT_INPUT).value<void*>();
		//areaProperty = findSubProperty(m_inputProperties, "area");
	}
	else
	{
		node = (QDomElement*) m_selectedControl->data(MIDICONTROL_XMLELEMENT_OUTPUT).value<void*>();
		//areaProperty = findSubProperty(m_outputProperties, "area");
	}
	controlSetNodeArea(m_selectedControl, area, node, NULL, m_selectionItem);
}


void MidiJDraw::setCurrentProject(QString projectDir, QString guiXmlFile, QString midiXmlFile, QString scriptPrefix)
{
	m_projectDir = projectDir;
	m_guiXmlFile = guiXmlFile;
	m_midiXmlFile = midiXmlFile;
	m_scriptPrefix = scriptPrefix;
	m_makeBak = true;
	// find scheme header files
	QStringList fileExtList;
	fileExtList.append("*.xml");
	fileExtList.append("*.png");
	QStandardItemModel* model = (QStandardItemModel*)ui.treeView->model();
	if (model == NULL)
	{
		model = new QStandardItemModel();
	}
	model->clear();

	QIcon pngIcon("imagens/16x16/png_file.png");
	QIcon xmlIcon("imagens/16x16/coffee_cup.png");

	QStandardItem* rootItem = new QStandardItem(m_projectDir);

	QDirIterator xmlHeadersIt(m_projectDir, fileExtList, QDir::Files);
	while (xmlHeadersIt.hasNext())
	{
		xmlHeadersIt.next();
		if (xmlHeadersIt.fileInfo().suffix() == "xml")
			rootItem->appendRow(new QStandardItem(xmlIcon, xmlHeadersIt.fileName()));
		else
			rootItem->appendRow(new QStandardItem(pngIcon, xmlHeadersIt.fileName()));
	}
	model->invisibleRootItem()->appendRow(rootItem);

	ui.treeView->setModel(model);
	ui.treeView->expand(rootItem->index());


	// MidiMapping -> control inspector
	qDeleteAll(m_guiItems);
	m_guiItems.clear();
	DirContext dc(m_projectDir);
	QDomElement xml = MidiMappingGui::openXMLFile(m_guiXmlFile, QString("guiMapping"));
	m_midiMappingGui->clear();
	m_midiMappingGui->loadGraphicItems(xml, m_guiItems);
	QFile file(m_guiXmlFile);
	if(file.open(QIODevice::ReadOnly))
	{
		QTextStream ts( &file );
		ui.xmlEdit->setPlainText(ts.readAll());
		file.close();
	}
	dc.clear();

	QGraphicsScene* scene = ui.graphicsView->scene();
	foreach (QGraphicsItem *item, m_guiItems)
		scene->addItem(item);

	m_midiSysexCombo->blockSignals(true);
	m_midiSysexCombo->clear();
	m_midiSysexCombo->addItems(m_midiMappingGui->midiSysexActionMap().keys());
	m_midiSysexCombo->blockSignals(false);

	QTreeWidgetItem* parentItem = NULL;
	QTreeWidgetItem* treeRootItem = new QTreeWidgetItem(QStringList("background"));
	QList<GraphicsMidiControlItem*> controls = m_midiMappingGui->controls();
	QMap<QString, QTreeWidgetItem*> groups;

	ui.controlBrowser->clear();
	foreach(GraphicsMidiControlItem* control, controls)
	{
		QString groupName = control->groupName();

		if (!groupName.isEmpty())
		{
			parentItem = groups.value(groupName);
			if (parentItem == NULL)
			{
				parentItem = new QTreeWidgetItem(QStringList(groupName));
				groups.insert(groupName, parentItem);
				treeRootItem->addChild(parentItem);
			}
		}
		else
			parentItem = treeRootItem;

		QStringList controlItems(control->name());
		control->setData(MIDIJDRAW_CONTROL_NAME,control->name());

		if (!control->hasOutput())
		{
			if (control->isVisible())
				controlItems << "input";
			else
				controlItems << "input/multi";
		}
		else
		{
			 if(control->hasInput())
				 controlItems << "input/output";
			 else
				 controlItems << "output";
		}
		QDomElement* node = midiControlXmlElement(control);
		if (node)
		{
			QDomElement midiNo = node->firstChildElement("midino");
			controlItems << midiNo.text();
		}
		QTreeWidgetItem* item = new QTreeWidgetItem(controlItems);
		control->setFlag(QGraphicsItem::ItemIsSelectable, true);
		item->setData(0, Qt::UserRole, qVariantFromValue((void*) control));
		parentItem->addChild(item);

	}
	ui.controlBrowser->insertTopLevelItem(0, treeRootItem);
	ui.controlBrowser->expandItem(treeRootItem);


	// TODO MidiMapping -> writeXML





/*   --- From Net -----
 *
 * 1. ...
   2. FIXME m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
   3. ...
 *  connect(m_tree, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
 *
 *
 * 1. void MyWidget::showContextMenu(const QPoint &pos)
   2. {
   3. QTreeWidgetItem *item = m_tree->itemAt(pos);
   4. if (!item)
   5. 	return;
   6.
   7. QMenu menu(tr("Context menu"), this);
   8. //add needed actions
   9. menu.exec(m_tree->viewport()->mapToGlobal(pos));
  10. }
 *
 *
 *
 * --------------- From designer ---------------
 *
 *     connect(treeView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotPopupContextMenu(QPoint)));
 *
 *
 * void ObjectInspector::ObjectInspectorPrivate::slotPopupContextMenu(QWidget *, const QPoint &pos)
{
    if (m_formWindow == 0 || m_formWindow->currentTool() != 0)
        return;

    const QModelIndex index =  m_treeView->indexAt (pos);
    if (QObject *object = m_model->objectAt(m_treeView->indexAt(pos)))
        if (QMenu *menu = createTaskMenu(object, m_formWindow)) {
            menu->exec(m_treeView->viewport()->mapToGlobal(pos));
            delete menu;
        }
}
 *
 */

}

void MidiJDraw::controlSelected(GraphicsMidiControlItem *control)
{
    ui.propertyBrowser->clear();
    if (control)
	{
/*		QPainterPath path;
		path.addRect(control->sceneRect());
		ui.graphicsView->scene()->setSelectionArea(path, QTransform());*/
    	m_selectedControl = control;

    	m_selectionItem->setRect(control->sceneRect());
    	m_selectionItem->setVisible(true);
    	bool readOnly = true;

		if (!control->hasOutput())
		{
			QDomElement* inputNode = (QDomElement*) control->data(MIDICONTROL_XMLELEMENT_INPUT).value<void*>();
			readOnly = inputNode->attribute("debug", "").isEmpty();
			updatePropertiesControl(inputNode, m_controlProperties->subProperties());
			ui.propertyBrowser->addProperty(m_controlProperties);
			updatePropertiesInput(inputNode, m_inputProperties->subProperties());
			ui.propertyBrowser->addProperty(m_inputProperties);
			updateXmlEdit(inputNode);
			updateMidiStates(inputNode);
		}
		else
		{
			QDomElement* outputNode = (QDomElement*) control->data(MIDICONTROL_XMLELEMENT_OUTPUT).value<void*>();
			readOnly = outputNode->attribute("debug", "").isEmpty();
			updatePropertiesControl(outputNode, m_controlProperties->subProperties());
			ui.propertyBrowser->addProperty(m_controlProperties);
			updatePropertiesOutput(outputNode, m_outputProperties->subProperties());
			ui.propertyBrowser->addProperty(m_outputProperties);
			if(control->hasInput())
			{
				QDomElement* inputNode = (QDomElement*) control->data(MIDICONTROL_XMLELEMENT_INPUT).value<void*>();
				updatePropertiesInput(inputNode, m_inputProperties->subProperties());
				ui.propertyBrowser->addProperty(m_inputProperties);
				updateMidiStates(inputNode);
			}
			else
				updateMidiStates(NULL);
			updateXmlEdit(outputNode);
			 // TODO add output type
		}

		m_selectionItem->m_readOnly = readOnly;
	}
	else
	{
		//ui.graphicsView->scene()->clearSelection();
		m_midiState->clear();
    	m_selectedControl = NULL;
    	m_selectionItem->setVisible(false);

	}
}

void MidiJDraw::updateXmlEdit(QDomElement* node)
{
	if (!node)
		return;
	// update xml window
	QTextCursor tc(ui.xmlEdit->textCursor());
	tc.setPosition(0,QTextCursor::MoveAnchor);
	tc.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, node->lineNumber());
	ui.xmlEdit->setTextCursor(tc);
	ui.xmlEdit->centerCursor();
}

void MidiJDraw::updateMidiStates(QDomElement* node)
{
	if (!node)
	{
		m_midiState->clear();
		return;
	}

	QDomNodeList states = node->elementsByTagName("state");
	m_midiState->blockSignals(true);
	m_midiState->clear();
	for(int i = 0; i < states.count(); i++)
	{
		m_midiState->addItem(states.at(i).attributes().namedItem("value").nodeValue());
	}
	m_midiState->blockSignals(false);
}


void MidiJDraw::controlBrowserItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	qDebug(__PRETTY_FUNCTION__);

	if (current == NULL)
		return;

	qDebug(QString("controlBrowserItemChanged %1").arg(current->data(0, Qt::DisplayRole).toString()).toAscii());
//	QString debug = QString("controlBrowserItemChanged %1").arg(current->data(0, Qt::DisplayRole).toString());
//	setWindowTitle(debug);

	QVariant data = current->data(0, Qt::UserRole);
	GraphicsMidiControlItem* control = (GraphicsMidiControlItem*)(data.value<void*>());
    controlSelected(control);
    // TODO MidiControl -> property browser



}

void MidiJDraw::graphicsItemClicked(QGraphicsItem *item)
{
	qDebug(__PRETTY_FUNCTION__);

	qDebug(QString("graphicsItemClicked %1").arg(item->data(MIDIJDRAW_CONTROL_NAME).toString()).toAscii());
//	QString debug = QString("graphicsItemClicked %1").arg(item->data(MIDIJDRAW_CONTROL_NAME).toString());
//	setWindowTitle(debug);
	GraphicsMidiControlItem* control = (GraphicsMidiControlItem*)item;
    controlSelected(control);
}


void MidiJDraw::sceneRectChanged(const QRectF& rect)
{
	qDebug(__PRETTY_FUNCTION__);
	if (!m_selectedControl)
		return;
	QDomElement* node = NULL;

	QtProperty* areaProperty = NULL;
	if (!m_selectedControl->hasOutput())
	{
		node = (QDomElement*) m_selectedControl->data(MIDICONTROL_XMLELEMENT_INPUT).value<void*>();
		areaProperty = findSubProperty(m_inputProperties, "area");
	}
	else
	{
		node = (QDomElement*) m_selectedControl->data(MIDICONTROL_XMLELEMENT_OUTPUT).value<void*>();
		areaProperty = findSubProperty(m_outputProperties, "area");
	}
	QRect area = rect.toRect();
	QString areaValue = QString("%1,%2,%3,%4").arg(area.x())
					.arg(area.y())
					.arg(area.width())
					.arg(area.height());

	controlSetNodeArea(m_selectedControl, areaValue, node, areaProperty, NULL);

	if (m_selectedControl->hasOutput())
	{
		// also update the Input XML Node
		node = (QDomElement*) m_selectedControl->data(MIDICONTROL_XMLELEMENT_INPUT).value<void*>();
		if (node)
			controlSetNodeArea(NULL, areaValue, node, NULL, NULL);
	}
}

void MidiJDraw::itemContextMenu(const QPointF& pos)
{
	QMenu menu;
	QAction* stateEditor = menu.addAction("State Editor");
	connect(stateEditor, SIGNAL(triggered()), SLOT(callStateEditor()));
	menu.exec(pos.toPoint());
}


#include "MidijStateEditor.h"
void MidiJDraw::callStateEditor()
{
	if (!m_selectedControl)
		return;

	MidijStateEditor editor;

	QPixmap pix = m_selectedControl->pixmap();

	editor.setPixmap(pix);
	QDomElement* node = (QDomElement*) m_selectedControl->data(MIDICONTROL_XMLELEMENT_INPUT).value<void*>();
	if (node)
		editor.setElement(*node);
	editor.exec();
}

void MidiJDraw::zoomIn()
{
	ui.graphicsView->scale(2,2);
}

void MidiJDraw::zoomOut()
{
	ui.graphicsView->scale(0.5,0.5);
}

void MidiJDraw::midiSysexSelected(QString sysexMsg)
{
	qDebug(__PRETTY_FUNCTION__);

	QStringList values = sysexMsg.split('-');
	QByteArray sysexBytes;
	sysexBytes.append(0xF0);
	for (int i = 0; i < values.size(); i++)
	{
		QString hex = QString("0x%1").arg(values.at(i));
		sysexBytes.append((char)hex.toInt(NULL, 0));
	}
	sysexBytes.append(0xF7);
	m_midiMappingGui->midiSysexMsgIn((uchar*)sysexBytes.data(), sysexBytes.size());
}

void MidiJDraw::midiStateSelected(QString midiMsg)
{
	qDebug(__PRETTY_FUNCTION__);
	if (m_selectedControl && midiMsg.size() > 0)
	{
		bool ok = false;
		int midiByte2 = midiMsg.toInt(&ok,0);
		if (!ok)
			return;
		QList<MidiMessage> controlMidiMsgs = m_selectedControl->inputMidiMessages();
		if (controlMidiMsgs.size() > 0)
			m_midiMappingGui->midiShortMsgIn(controlMidiMsgs.at(0).getMidiStatusByte(), controlMidiMsgs.at(0).getMidiNo(), midiByte2);
	}
}

void MidiJDraw::xmlModificationChanged(bool modified)
{
	ui.actionSave->setEnabled(modified);
}


void MidiJDraw::saveXml()
{
	DirContext dc(m_projectDir);
	if (m_makeBak)
	{
		QString bakFile(m_guiXmlFile + ".bak");
		if (QFile::exists(bakFile))
			QFile::remove(bakFile);
		QFile::rename(m_guiXmlFile, bakFile);
		m_makeBak = false;
	}
	QFile f(m_guiXmlFile);
	f.open(QFile::WriteOnly);
	f.write(ui.xmlEdit->toPlainText().toAscii());
	f.close();
	ui.xmlEdit->document()->setModified(false);
}

void MidiJDraw::reload()
{
	bool makeBakTmp = m_makeBak;

	QDomElement* node = NULL;
	foreach (QGraphicsItem *item, m_guiItems)
	{
		node = (QDomElement*) item->data(MIDICONTROL_XMLELEMENT_INPUT).value<void*>();
		_delete(node);
		node = (QDomElement*) item->data(MIDICONTROL_XMLELEMENT_OUTPUT).value<void*>();
		_delete(node);
	}

	setCurrentProject(m_projectDir, m_guiXmlFile, m_midiXmlFile, m_scriptPrefix);
	m_makeBak = makeBakTmp;
}


void MidiJDraw::updatePropertiesControl(QDomElement* node, QList<QtProperty*> subProperties)
{
	foreach(QtProperty* property, subProperties)
	{
		// TODO if property->name() == "sub-groupXXX"
		// {
		//	foreach(prop, property->subPropeties())
		//	{
		//		subNode = node->element("")
		//	}
		XPX_StringAttr("name", node, property);
		XPX_IntAttr("lineNumber", node, property, true);
		//XPX_

	}
}

void MidiJDraw::updatePropertiesInput(QDomElement* node, QList<QtProperty*> subProperties)
{
	m_stringManager->blockSignals(true);
	foreach(QtProperty* property, subProperties)
	{
		XPX_StringElem("midino", node, property);
		XPX_StringElem("area", node, property);
		XPX_StringElem("state", node, property);
	}
	m_stringManager->blockSignals(false);
}

void MidiJDraw::updatePropertiesOutput(QDomElement* node, QList<QtProperty*> subProperties)
{
	m_stringManager->blockSignals(true);
	foreach(QtProperty* property, subProperties)
	{
		XPX_StringAttr("type", node, property, "button");
		XPX_StringElem("midino", node, property);
		XPX_StringElem("area", node, property);
	}
	m_stringManager->blockSignals(false);
}


void MidiJDraw::XPX_StringElem(const QString& tag, QDomElement* node, QtProperty* property, const QString& defaultValue)
{
	if (!property || (property->propertyName() != tag) || !node)
		return;

	QDomElement childNode = node->firstChildElement(tag);
	QString value = childNode.isNull()? defaultValue: childNode.text();
	m_stringManager->setValue(property, value);
}

void MidiJDraw::XPX_StringAttr(const QString& tag, QDomElement* node, QtProperty* property, const QString& defaultValue)
{
	if (!property || (property->propertyName() != tag) || !node)
		return;

	QString value = node->attribute(tag);
	if (value.isNull())
		value = defaultValue;
	m_stringManager->setValue(property, value);
}


void MidiJDraw::XPX_IntAttr(const QString& tag, QDomElement* node, QtProperty* property, bool readonly)
{
	if (!property || (property->propertyName() != tag) || !node)
		return;

	QString value = node->attribute(tag);
	int intValue = 0;
	// FIXME processar o <code>intValue</code>
	if (value.isNull())
	{
		if (tag == "lineNumber")
			intValue = node->lineNumber();
	}

	if (readonly)
		m_readIntManager->setValue(property, intValue);

}



void MidiJDraw::controlSetNodeArea(GraphicsMidiControlItem *control, const QString& areaValue, QDomElement* node, QtProperty* areaProperty, GraphicsSelectionItem* selectionItem)
{
	if (!node)
		return;

	QDomElement areaNode = node->firstChildElement("area");
	if (areaNode.isNull())
		return;

	QRect areaRect = areaToRect(areaValue);
	if (areaRect.isNull())
		return;

	QDomNode textNode = areaNode.firstChild();
	if (textNode.nodeType() == QDomNode::TextNode)
	{
		((QDomText*)&textNode)->setData(areaValue);
		if (areaProperty)
		{
			m_stringManager->blockSignals(true);
			m_stringManager->setValue(areaProperty, areaValue);
			m_stringManager->blockSignals(false);
		}

		if (control)
			control->setSceneRect(areaRect);

		if (selectionItem)
			selectionItem->setRect(areaRect);

		QTextCursor tc(ui.xmlEdit->document());
		tc.setPosition(0,QTextCursor::MoveAnchor);
		tc.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, areaNode.lineNumber() - 1);
		tc.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, areaNode.columnNumber() - 6 ); // - len("<area>")
		tc.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
		tc.removeSelectedText();
		tc.insertText(QString("<area>%1</area>").arg(areaValue));

	}

	//setWindowTitle("node area" + areaNode.nodeValue());
}


/*
#ifndef QT_NO_SCRIPTTOOLS
    if (debug) {
        if (!m_debugger) {
            m_debugger = new QScriptEngineDebugger(this);
            m_debugWindow = m_debugger->standardWindow();
            m_debugWindow->setWindowModality(Qt::ApplicationModal);
            m_debugWindow->resize(1280, 704);
        }
        m_debugger->attachTo(m_env->engine());
        m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
    } else {
        if (m_debugger)
            m_debugger->detach();
    }
#else
    Q_UNUSED(debug);
#endif

    QScriptValue ret = m_env->evaluate(contents, fileName);

#ifndef QT_NO_SCRIPTTOOLS
    if (m_debugWindow)
        m_debugWindow->hide();
#endif
*/

void MidiJDraw::runScript()
{

	bool doDebug = qApp->keyboardModifiers() & Qt::SHIFT;

	MidijMidiObject* midiObject = new MidijMidiObject(m_midiMappingGui);
	m_scriptEngine->initEngine(midiObject);
	m_midiMapping = new MidiMapping(*midiObject);

	midiObject->setMidiScriptEngine(m_scriptEngine);
	midiObject->setMidiMapping(m_midiMapping);

	if (!m_debugger)
	{
		m_debugger = new QScriptEngineDebugger(this);
		m_debugWindow = m_debugger->standardWindow();
		//m_debugWindow->setWindowModality(Qt::ApplicationModal);
		m_debugWindow->resize(1280, 704);
	}
	m_debugger->attachTo(m_scriptEngine->getEngine());
//    connect(m_debugger, SIGNAL(evaluationSuspended()), SLOT(debuggerSuspended()), Qt::DirectConnection);
	connect(midiObject, SIGNAL(receiveFinished()), SLOT(debuggerResumed()), Qt::QueuedConnection);


	QFileInfo sf(m_midiXmlFile);
	DirContext dc0(m_projectDir);
	DirContext dc(sf);

	QDomElement xml = MidiMappingGui::openXMLFile(m_midiXmlFile, QString("midi xml file"));

	m_midiMapping->loadPreset(xml, false);
	//m_scriptEngine->evaluate(sf.absoluteFilePath());

	m_midiMappingGui->connectMidiOutPort(midiObject, SLOT(emulReceive(uchar,uchar,uchar)), Qt::QueuedConnection);

	if (doDebug)
		m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();

	bool running = m_scriptEngine->execute(m_scriptPrefix + ".init", "MidijDraw");

//	if (doDebug)
//		m_debugWindow->hide();

	if (running)
	{
		// listen to SHIFT+click...
		m_scene->installEventFilter(this);
		midiObject->queueReceives(MidijMidiObject::NoQueue);
	}
	else
	{
//		disconnect(m_debugger);
		m_debugger->detach();
		m_scriptEngine->stopEngine();
	}

	dc.clear();

	ui.actionRunScript->setEnabled(!running);
	ui.actionTriggerBreakpoint->setEnabled(running);
	ui.actionStopScript->setEnabled(running);
	ui.actionReload->setEnabled(!running);

}

void MidiJDraw::stopScript()
{
	m_scene->removeEventFilter(this);
//	disconnect(m_debugger);
	if (m_debugger)
		m_debugger->detach();
	m_scriptEngine->stopEngine();

	ui.actionRunScript->setEnabled(true);
	ui.actionTriggerBreakpoint->setEnabled(false);
	ui.actionStopScript->setEnabled(false);
	ui.actionReload->setEnabled(true);
}

bool MidiJDraw::eventFilter(QObject *targetObj, QEvent *event)
{
	if (targetObj == m_scene)
	{
		if (event->type() == QEvent::GraphicsSceneMousePress
				&& ((qApp->keyboardModifiers() & Qt::SHIFT) > 0)
				&& !m_scriptEngine->isDebugging())
		{
			//m_debugger->state();
			m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
			m_scriptEngine->debugging(true);
		}
	}
	return QMainWindow::eventFilter(targetObj, event);
}

//void MidiJDraw::debuggerSuspended()
//{
//	qDebug(__PRETTY_FUNCTION__);
//
//}

void MidiJDraw::debuggerResumed()
{
	qDebug(__PRETTY_FUNCTION__);
	// FIXME evaluationResumed � trigado sempre q haja 1step...
/*  Returns the current state of the debugger.

  \sa evaluationResumed()
  \sa evaluationSuspended()

QScriptEngineDebugger::DebuggerState QScriptEngineDebugger::state() const
{
    Q_D(const QScriptEngineDebugger);
    return !d->debugger || !d->debugger->isInteractive() ? SuspendedState : RunningState;
}
*/
	if (m_debugger->state() == QScriptEngineDebugger::RunningState)
		qDebug("***********  QScriptEngineDebugger::RunningState");
	else
		qDebug("***********  QScriptEngineDebugger::SuspendedState");

	m_debugger->action(QScriptEngineDebugger::ContinueAction)->trigger();


	m_scriptEngine->debugging(false);
//	m_debugWindow->hide();
	qDebug("exit debuggerResumed");
}




/* TODO passar isto para o Graphics (ou usar o filtro)
void MidiJDraw::mousePressEvent(QMouseEvent *event)
{
	m_mousePressShift = event->modifiers() & Qt::SHIFT;
	if (m_mousePressShift)
	{
		qDebug("Shift pressed");
//		if (!m_debugger)
//		{
//			m_debugger = new QScriptEngineDebugger(m_midiControler);
//			m_debugWindow = m_debugger->standardWindow();
//			m_debugWindow->setWindowModality(Qt::ApplicationModal);
//			m_debugWindow->resize(1280, 704);
//		}
//		m_debugger->attachTo(m_midiControler->getMidiScriptEngine()->getEngine());
//		m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
	}

	QMainWindow::mousePressEvent(event);
}
*/

// HOT WHEELS SKIN
//////////////////////////////////////////////////

#define HOTWHEELS_DRAW_BG	1
#define HOTWHEELS_BG_BLUR	2
#define HOTWHEELS_DRAW_LINE	3
#define HOTWHEELS_LINE_BLUR	4
#define HOTWHEELS_SCENE_BG	5

#define HOTWHEELS_MODE_SLIDERS	0x10
#define HOTWHEELS_MODE_VINYL	0x20
#define HOTWHEELS_MODE_BUTTONS	0x30

////////////////////////
// CONFIG
////////////////////////

const Qt::GlobalColor STATE_COLOR 	= Qt::gray;
const int hotWheelsMode 			= 0;//HOTWHEELS_MODE_SLIDERS;
const int hotWheelsDrawOnly 		= 0;//HOTWHEELS_DRAW_BG;
//const int hotWheelsDrawOnly 		= HOTWHEELS_DRAW_BG;
const QString PNG_DEFAULTFILE		= "blueskin_gray_03b.png";
////////////////////////

bool hotWheelsFlag(int flag)
{
	if (flag >= HOTWHEELS_MODE_SLIDERS)
	{
		return flag == hotWheelsMode;
	}
	if (hotWheelsDrawOnly > 0)
	{
		return flag == hotWheelsDrawOnly;
	}
//	switch (flag)
//	{
////	case HOTWHEELS_DRAW_BG:
//	case HOTWHEELS_BG_BLUR:
//	case HOTWHEELS_DRAW_LINE:
//	case HOTWHEELS_LINE_BLUR:
//		return false;
//		break;
//	}
	return true;
}

QBrush hotWheelsBgBrush(Qt::GlobalColor c)
{
	if (!hotWheelsFlag(HOTWHEELS_DRAW_BG))
		return QBrush(Qt::NoBrush);

	QColor bgColor;
	switch (c) {
		case Qt::blue:
//			bgColor = QColor::fromRgb(2, 140, 220, 255);
			bgColor = QColor::fromRgb(50, 100, 240, 255); // From Pioneer Blue Edition
			break;
		case Qt::magenta:
			bgColor = QColor::fromRgb(0xc7, 1, 0xa7, 255);
			break;
		case Qt::gray:
			bgColor = QColor::fromRgb(0x70, 0x70, 0x70, 255);
			break;
		case Qt::red:
			bgColor = QColor::fromRgb(0xf0, 0xf, 0xf, 255);
			break;
		default:
			bgColor = QColor(c);
			break;
	}

	return QBrush(bgColor);
}

void hotWheelsSetBgBlur(QGraphicsItem* item)
{
	if (hotWheelsFlag(HOTWHEELS_BG_BLUR))
	{
		QGraphicsBlurEffect* bgBlur = new QGraphicsBlurEffect();
		bgBlur->setBlurHints(QGraphicsBlurEffect::QualityHint);
		bgBlur->setBlurRadius(10);
		item->setGraphicsEffect(bgBlur);
	}
}

void hotWheelsSetLineBlur(QGraphicsItem* item)
{
	if (hotWheelsFlag(HOTWHEELS_LINE_BLUR))
	{
		QGraphicsBlurEffect* bgBlur = new QGraphicsBlurEffect();
		bgBlur->setBlurHints(QGraphicsBlurEffect::QualityHint);
		bgBlur->setBlurRadius(10);
		item->setGraphicsEffect(bgBlur);
	}
}


class GraphicsRoundedRectItem : public QGraphicsRectItem
{
public:
	GraphicsRoundedRectItem(const QRectF &_rect, const QPen &_pen, const QBrush &_brush, qreal _radius)
		: QGraphicsRectItem(_rect)
	{
	    setPen(_pen);
	    setBrush(_brush);
	    m_radius = _radius;
	};

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
	    Q_UNUSED(widget);
	    painter->setPen(pen());
	    painter->setBrush(brush());
	    painter->drawRoundedRect(rect(), m_radius, m_radius);
	};

	qreal m_radius;
};



void hotWheelsButton(const QRectF& rect, QGraphicsScene* scene, Qt::GlobalColor c = Qt::blue)
{
	QGraphicsItem* bg = new GraphicsRoundedRectItem(rect, Qt::NoPen, hotWheelsBgBrush(c), 10);
//	QGraphicsBlurEffect* bgBlur = new QGraphicsBlurEffect(scene);
//	bgBlur->setBlurRadius(10);
//	bgBlur->setBlurHints(QGraphicsBlurEffect::QualityHint);
//	bg->setGraphicsEffect(bgBlur);
	hotWheelsSetBgBlur(bg);
	scene->addItem(bg);

	if (hotWheelsFlag(HOTWHEELS_DRAW_LINE))
	{
		QRectF lineRect = rect.adjusted(10, 10, -10, -10);
		QPen linePen(QBrush(Qt::white), 4, Qt::SolidLine, Qt::RoundCap);
		QGraphicsItem* lineBg = new  GraphicsRoundedRectItem(lineRect, linePen, QBrush(Qt::NoBrush), 10);
		scene->addItem(lineBg);

//	bgBlur = new QGraphicsBlurEffect(scene);
//	bgBlur->setBlurRadius(10);
//	lineBg->setGraphicsEffect(bgBlur);

		hotWheelsSetLineBlur(lineBg);

		linePen.setWidth(4);

		QGraphicsItem* line = new  GraphicsRoundedRectItem(lineRect, linePen, QBrush(Qt::NoBrush), 10);
		scene->addItem(line);
	}
}

#define DRAW_ALL	0x3
#define DRAW_BG		0x1
#define DRAW_LINE	0x2

void hotWheelsBar(const QRectF& rect, int drawType, QGraphicsScene* scene, Qt::GlobalColor c = Qt::blue)
{
	if (drawType & DRAW_BG)
	{
		QGraphicsItem* bg = new GraphicsRoundedRectItem(rect, Qt::NoPen, hotWheelsBgBrush(c), 10);
//		QGraphicsBlurEffect* bgBlur = new QGraphicsBlurEffect(scene);
//		bgBlur->setBlurHints(QGraphicsBlurEffect::QualityHint);
//		bgBlur->setBlurRadius(10);
//		bg->setGraphicsEffect(bgBlur);
		hotWheelsSetBgBlur(bg);
		scene->addItem(bg);
	}

	/*
	QRectF lineRect = rect.adjusted(10, 10, -10, -10);
	QPen linePen(QBrush(Qt::white), 6, Qt::SolidLine, Qt::RoundCap);
	QGraphicsItem* lineBg = new  GraphicsRoundedRectItem(lineRect, linePen, QBrush(Qt::NoBrush), 10);
	scene->addItem(lineBg);
	bgBlur = new QGraphicsBlurEffect(scene);
	bgBlur->setBlurRadius(10);
	lineBg->setGraphicsEffect(bgBlur);

	linePen.setWidth(4);
	QGraphicsItem* line = new  GraphicsRoundedRectItem(lineRect, linePen, QBrush(Qt::NoBrush), 10);
	scene->addItem(line);
	*/
	if (drawType & DRAW_LINE && hotWheelsFlag(HOTWHEELS_DRAW_LINE))
	{
		scene->addRect(rect.adjusted(6, 10, -6,-10), Qt::NoPen, QBrush(Qt::white));
	}
}

void hotWheelsEllipse(const QRectF& rect, int innerDraw, QGraphicsScene* scene, Qt::GlobalColor c = Qt::blue)
{
	QGraphicsEllipseItem* item = scene->addEllipse(rect, Qt::NoPen, hotWheelsBgBrush(c));
	hotWheelsSetBgBlur(item);


	if (innerDraw < 20)
		return;

	QRectF innerRect = rect.adjusted(innerDraw, innerDraw, -innerDraw, -innerDraw);
	item = scene->addEllipse(innerRect, Qt::NoPen, Qt::black);
	hotWheelsSetBgBlur(item);

	if (hotWheelsFlag(HOTWHEELS_DRAW_LINE))
	{
		QPen linePen = QPen(QBrush(Qt::white), 4, Qt::SolidLine, Qt::RoundCap);
		hotWheelsSetLineBlur(scene->addEllipse(rect.adjusted(10,10,-10,-10), linePen, Qt::NoBrush));
		scene->addEllipse(rect.adjusted(10,10,-10,-10), linePen, Qt::NoBrush);

		item = scene->addEllipse(innerRect.adjusted(-10,-10,10,10), linePen, Qt::NoBrush);
		hotWheelsSetLineBlur(item);
		scene->addEllipse(innerRect.adjusted(-10,-10,10,10), linePen, Qt::NoBrush);
	}
}

void MidiJDraw::sceneSavePng()
{

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
			PNG_DEFAULTFILE,
			tr("Images (*.png *.xpm *.jpg)"));

	if (!fileName.isEmpty())
	{
		QGraphicsScene* scene = ui.graphicsView->scene();
		QImage img(scene->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
		img.fill(0);
		QPainter painter(&img);
		painter.setRenderHint(QPainter::Antialiasing, true);
		scene->render(&painter);
		img.save(fileName);
	}
}


void MidiJDraw::drawHotWheelsSkin()
{
	ui.controlsFrame->hide();
	ui.xmlFrame->hide();
	ui.propertyFrame->hide();
	ui.projectFrame->hide();

	QAction* save = ui.navigationToolbar->addAction("Save SKin");
	connect(save, SIGNAL(triggered()), SLOT(sceneSavePng()));

	QGraphicsScene* scene = ui.graphicsView->scene();
	ui.graphicsView->setRenderHint(QPainter::Antialiasing, true);

	if (hotWheelsFlag(HOTWHEELS_SCENE_BG))
	{
		scene->setSceneRect(-4, -4, 488, 808);
	//	scene->setBackgroundBrush(QBrush(Qt::black));
		scene->addRect(QRectF(0,0,480,800), Qt::NoPen, Qt::black);
	}

	// slider VOL
	hotWheelsButton(QRectF(10,20,70,225), scene);
	hotWheelsBar(QRectF(10+70,20,26,225), DRAW_ALL, scene);


	hotWheelsButton(QRectF((480-220)/2, 60,220,70), scene, STATE_COLOR);
	hotWheelsButton(QRectF((480-220)/2,60+72,220,70), scene, STATE_COLOR);
	hotWheelsButton(QRectF((480-220)/2,60+144,220,70), scene, STATE_COLOR);

	if (true)
	{
		hotWheelsBar(QRectF((480-10)/2,    70,10,50), DRAW_LINE, scene);
		hotWheelsBar(QRectF((480-10)/2, 70+72,10,50), DRAW_LINE, scene);
		hotWheelsBar(QRectF((480-10)/2,70+144,10,50), DRAW_LINE, scene);
	}

	// slider PITCH
	hotWheelsBar(QRectF(400 - 26,20,26,225), DRAW_ALL, scene);
	hotWheelsButton(QRectF(400,20,70,225), scene);


	// DECK LETTER: A
	hotWheelsEllipse(QRectF(105, 5, 40, 40), -1, scene, STATE_COLOR);

	// DECK LETTER: B
	hotWheelsEllipse(QRectF(335, 5, 40, 40), -1, scene, STATE_COLOR);

	// B11
	hotWheelsButton(QRectF(10,255,70,70), scene, STATE_COLOR);
	//hotWheelsBar(QRectF(25, 260,40,20), DRAW_LINE, scene);
	// B12
	hotWheelsButton(QRectF(400,255,70,70), scene, STATE_COLOR);
	//hotWheelsBar(QRectF(415, 260,40,20), DRAW_LINE, scene);
	// B13
	hotWheelsButton(QRectF(10,660,70,70), scene, STATE_COLOR);
	//hotWheelsBar(QRectF(25, 640,40,20), DRAW_LINE, scene);
	// B14
	hotWheelsButton(QRectF(400,660,70,70), scene, STATE_COLOR);
	//hotWheelsBar(QRectF(415, 640,40,20), DRAW_LINE, scene);


	if (hotWheelsFlag(HOTWHEELS_MODE_SLIDERS))
	{
		// slider S3
		hotWheelsButton(QRectF(20,370,110,265), scene);
		// slider S5
		hotWheelsButton(QRectF(480-20-110,370,110,265), scene);
	}
	if (hotWheelsFlag(HOTWHEELS_MODE_VINYL))
	{
		/*<Control name="VinylCircleC1_abs" type="WheelAbsolute" group="C1" image0="darky.png" debug="red">
		    <status>0xB0</status> <midino>0x62</midino>
		    <!--  area type="Wheel" size="38">20,288,440,440</area  -->
		    <area type="Wheel" size="46">18,286,445,445</area>
		</Control> */
		hotWheelsEllipse(QRectF(18,280,445,445), 50, scene, STATE_COLOR);

	}
	if (hotWheelsFlag(HOTWHEELS_MODE_BUTTONS))
	{
		// slider S3
		hotWheelsButton(QRectF(20,370,110,120), scene, STATE_COLOR);
		hotWheelsButton(QRectF(20,515,110,120), scene, STATE_COLOR);
		// slider S5
		hotWheelsButton(QRectF(480-20-110,370,110,120), scene, STATE_COLOR);
		hotWheelsButton(QRectF(480-20-110,515,110,120), scene, STATE_COLOR);
	}

	hotWheelsBar(QRectF(30+100,370,30,265), DRAW_ALL, scene);

	// slider S4
	hotWheelsButton(QRectF((480-160)/2,360,160,285), scene, STATE_COLOR);
	hotWheelsBar(QRectF((480-30)/2,370,30,265), DRAW_LINE, scene);

	hotWheelsBar(QRectF(480-30-130,370,30,265), DRAW_ALL, scene);


	// Play-Cue- buttons
	hotWheelsButton(QRectF(10,800-60,460,60), scene, STATE_COLOR);
	if (true)
	{
		hotWheelsBar(QRectF((480-10)/4 + 5,   800-50,10,40), DRAW_LINE, scene);
		hotWheelsBar(QRectF((480-10)/2,       800-50,10,40), DRAW_LINE, scene);
		hotWheelsBar(QRectF((480-10)/4*3 - 5, 800-50,10,40), DRAW_LINE, scene);
	}

}



//#define SPOT_SIZE	14
//#define RECT_SIZE	90
//#define RECT_AJUST	20
//#define RRECT_AJUST	10
//
//void drawSpot(bool drawFlag, QPointF point, int move, int size, QGraphicsScene* scene)
//{
//	if (!drawFlag)
//		return;
//
//	QRectF oRect = QRectF(point, QSizeF(size, size)).translated(move, move);
///*	QRadialGradient rad(point, size/2, point + QPointF(size/2, size/2));
//	rad.setColorAt(1, Qt::black);
//	rad.setColorAt(0, Qt::gray);
//
//	scene->addEllipse(oRect, Qt::NoPen, QBrush(rad));
//	*/
//
//
//	scene->addEllipse(oRect, Qt::NoPen, Qt::gray);
//
//
//}
//
//void drawDado(const QRectF& rect, int spotSize, int mask, QGraphicsScene* scene)
//{
//	if (false)
//	{
//		QGraphicsItem* rr = new GraphicsRoundedRectItem(rect.adjusted(-RRECT_AJUST,-RRECT_AJUST,RRECT_AJUST,RRECT_AJUST), QPen(Qt::black), QBrush(Qt::NoBrush), 8);
//		scene->addItem(rr);
//	}
//
//	drawSpot((mask & 0x400) > 0, rect.topLeft(), 0, spotSize, scene);
//	drawSpot((mask & 0x200) > 0, QPointF(rect.left(), rect.center().y() - spotSize/2) , 0, spotSize, scene);
//	drawSpot((mask & 0x100) > 0, QPointF(rect.left(), rect.bottom() - spotSize) , 0, spotSize, scene);
//
//
//	drawSpot((mask & 0x020) > 0, rect.center(), -spotSize/2, spotSize, scene);
//
//
//	drawSpot((mask & 0x004) > 0, QPointF(rect.right(), rect.top() + spotSize), -spotSize, spotSize, scene);
//	drawSpot((mask & 0x002) > 0, QPointF(rect.right(), rect.center().y() + spotSize/2), -spotSize, spotSize, scene);
//	drawSpot((mask & 0x001) > 0, rect.bottomRight(), -spotSize, spotSize, scene);
//}
//
//
//void MidiJDraw::drawDados6()
//{
//
//	QGraphicsScene* scene = ui.graphicsView->scene();
//	ui.graphicsView->setRenderHint(QPainter::Antialiasing, true);
//
//	scene->setSceneRect(0, 0, RECT_SIZE*6, RECT_SIZE);
//
//	QRectF r1(0,0,RECT_SIZE, RECT_SIZE);
//	drawDado(r1.adjusted(RECT_AJUST, RECT_AJUST, -RECT_AJUST, -RECT_AJUST), SPOT_SIZE, 0x020, scene);
//
//	QRectF r2(RECT_SIZE,0,RECT_SIZE, RECT_SIZE);
//	drawDado(r2.adjusted(RECT_AJUST, RECT_AJUST, -RECT_AJUST, -RECT_AJUST), SPOT_SIZE, 0x401, scene);
//
//	QRectF r3(RECT_SIZE*2,0,RECT_SIZE, RECT_SIZE);
//	drawDado(r3.adjusted(RECT_AJUST, RECT_AJUST, -RECT_AJUST, -RECT_AJUST), SPOT_SIZE, 0x421, scene);
//
//	QRectF r4(RECT_SIZE*3,0,RECT_SIZE, RECT_SIZE);
//	drawDado(r4.adjusted(RECT_AJUST, RECT_AJUST, -RECT_AJUST, -RECT_AJUST), SPOT_SIZE, 0x505, scene);
//
//	QRectF r5(RECT_SIZE*4,0,RECT_SIZE, RECT_SIZE);
//	drawDado(r5.adjusted(RECT_AJUST, RECT_AJUST, -RECT_AJUST, -RECT_AJUST), SPOT_SIZE, 0x525, scene);
//
//	QRectF r6(RECT_SIZE*5,0,RECT_SIZE, RECT_SIZE);
//	drawDado(r6.adjusted(RECT_AJUST, RECT_AJUST, -RECT_AJUST, -RECT_AJUST), SPOT_SIZE, 0x707, scene);
//
//}


#define BS_MARGIN			16
#define BS_BUTTON_HEIGHT	120
#define BS_BUTTON_WIDTH		(160)
#define SCENE_WIDTH			800
#define SCENE_HEIGHT		480
#define BS_LOGO_HEIGHT		40


void MidiJDraw::drawHwBrowsingSkin()
{
	ui.controlsFrame->hide();
	ui.xmlFrame->hide();
	ui.propertyFrame->hide();
	ui.projectFrame->hide();

	QAction* save = ui.navigationToolbar->addAction("Save SKin");
	connect(save, SIGNAL(triggered()), SLOT(sceneSavePng()));

	QGraphicsScene* scene = ui.graphicsView->scene();
	ui.graphicsView->setRenderHint(QPainter::Antialiasing, true);

	if (hotWheelsFlag(HOTWHEELS_SCENE_BG))
	{
		scene->setSceneRect(-4, -4, 808, 488);
	//	scene->setBackgroundBrush(QBrush(Qt::black));
		scene->addRect(QRectF(0,0,800,480), Qt::NoPen, Qt::black);
	}

	// top: LOGO


	// left 3 buttons
	int buttonTop1 = (SCENE_HEIGHT - BS_LOGO_HEIGHT) /2 - (BS_BUTTON_HEIGHT*3 + BS_MARGIN*2)/2 + BS_LOGO_HEIGHT;
	hotWheelsButton(QRectF(BS_MARGIN, buttonTop1, BS_BUTTON_WIDTH, BS_BUTTON_HEIGHT), scene);
	hotWheelsButton(QRectF(BS_MARGIN, buttonTop1+BS_MARGIN+BS_BUTTON_HEIGHT, BS_BUTTON_WIDTH, BS_BUTTON_HEIGHT), scene);
	hotWheelsButton(QRectF(BS_MARGIN, buttonTop1+(BS_MARGIN+BS_BUTTON_HEIGHT)*2, BS_BUTTON_WIDTH, BS_BUTTON_HEIGHT), scene);

	// slider TREE browse
	int treeLeft = BS_MARGIN*2+BS_BUTTON_WIDTH;
	hotWheelsButton(QRectF(treeLeft,
			BS_MARGIN + BS_LOGO_HEIGHT,
			SCENE_WIDTH/2 - treeLeft - BS_MARGIN,
			SCENE_HEIGHT - BS_LOGO_HEIGHT - BS_MARGIN*2), scene);

	// slider FILE browse
	hotWheelsButton(QRectF(SCENE_WIDTH/2,
			BS_MARGIN + BS_LOGO_HEIGHT,
			SCENE_WIDTH/2 - BS_BUTTON_WIDTH - BS_MARGIN*2,
			SCENE_HEIGHT - BS_LOGO_HEIGHT - BS_MARGIN*2), scene);

	// right 3  buttons
	hotWheelsButton(QRectF(SCENE_WIDTH - BS_BUTTON_WIDTH - BS_MARGIN, buttonTop1, BS_BUTTON_WIDTH, BS_BUTTON_HEIGHT), scene);
	hotWheelsButton(QRectF(SCENE_WIDTH - BS_BUTTON_WIDTH - BS_MARGIN, buttonTop1+BS_MARGIN+BS_BUTTON_HEIGHT, BS_BUTTON_WIDTH, BS_BUTTON_HEIGHT), scene);
	hotWheelsButton(QRectF(SCENE_WIDTH - BS_BUTTON_WIDTH - BS_MARGIN, buttonTop1+(BS_MARGIN+BS_BUTTON_HEIGHT)*2, BS_BUTTON_WIDTH, BS_BUTTON_HEIGHT), scene);
}
