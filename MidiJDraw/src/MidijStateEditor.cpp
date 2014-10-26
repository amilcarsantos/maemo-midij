#include "MidijStateEditor.h"
#include <QTextStream>
#include <QtCore/qmath.h>

QRectF areaToRectF(const QString& areaCoords)
{
	QStringList coords = areaCoords.split(",");
	if (coords.size() != 4)
	{
		//qCritical() << "invalid area size:" << areaCoords << "; line:" << areaNode.lineNumber();
		return QRectF();
	}
	return QRectF(coords.at(0).toInt(), coords.at(1).toInt(), coords.at(2).toInt(), coords.at(3).toInt());
}


GraphicsGrid::GraphicsGrid()
{
	setAcceptedMouseButtons(Qt::NoButton);
	setCacheMode(ItemCoordinateCache);
	setZValue(1);
	m_steps = 0;
	m_extraSpaceStep = -1;
}

GraphicsGrid::~GraphicsGrid()
{

}

void GraphicsGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
	qDebug(__PRETTY_FUNCTION__);

	setPen(QPen(Qt::green));
	QGraphicsRectItem::paint(painter, option, w);

	int extraSpaceStep = m_extraSpaceStep <= 0 ? m_steps : m_extraSpaceStep;

	if (m_steps > 1)
	{
		QRectF rect = QGraphicsRectItem::rect();
		qreal l, t, r, d, de;
		l = rect.left();
		t = rect.top();
		r = rect.right();
		d = qFloor(rect.size().height() / m_steps);
		de = rect.size().height() - d * (m_steps - 1);
		for(int i = 1; i <= m_steps; i++)
		{
			if (i == extraSpaceStep)
				t += de;
			else
				t += d;
			painter->drawLine(l, t, r, t);
		}
	}
}



MidijStateEditor::MidijStateEditor(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	QGraphicsScene* scene = new QGraphicsScene(this);
	ui.graphicsView->setScene(scene);

	m_selectionItem = new GraphicsSelectionItem();
	m_selectionItem->m_readOnly = false;
	m_grid = new GraphicsGrid();

	scene->addItem(m_selectionItem);
	scene->addItem(m_grid);
	connect(m_selectionItem, SIGNAL(rectChanged(const QRectF &)), SLOT(sceneRectChanged(const QRectF &)));
//	connect(m_selectionItem, SIGNAL(contextMenu(const QPointF &)), SLOT(itemContextMenu(const QPointF &)));

	QStringList gridSizes;
	gridSizes << "7" << "8" << "9" << "10";

	ui.gridSizeCombo->addItems(gridSizes);
}

MidijStateEditor::~MidijStateEditor()
{
	delete m_selectionItem;
	delete m_grid;
}


void MidijStateEditor::setPixmap(const QPixmap& pixmap)
{
	//m_maxRect.setBottomLeft(QPointF());
	m_maxRect.setSize(pixmap.size());
	ui.graphicsView->scene()->addPixmap(pixmap)->setZValue(0);
	m_grid->setRect(m_maxRect);
	ui.graphicsView->setSceneRect(m_maxRect.adjusted(-4, -4, 4, 4));

	// FIXME suporte ao MaxRect
}

void MidijStateEditor::setElement(QDomElement node)
{
//	ui.plainTextEdit->setPlainText("");
	QString xml;
	QTextStream stream(&xml);
	node.save(stream, 4);
	stream.flush();

	ui.plainTextEdit->setPlainText(xml);
	m_grid->m_steps = 0;
//	qDebug(xml.toAscii());

	// get states
	QDomElement stateNode = node.firstChildElement("state");
	int nl = node.lineNumber();
	while (!stateNode.isNull())
	{
		QString state = stateNode.text();
		QString stateValue = stateNode.attribute("value").trimmed();

		ui.statesCombo->addItem(stateValue + " - " + state);

		QRectF rect = areaToRectF(stateNode.attribute("area"));
		m_rects.append(rect);
		m_lines.append(stateNode.lineNumber() - nl);
		if (!rect.isEmpty())
			m_grid->m_steps++;

		stateNode = stateNode.nextSiblingElement("state");
	}


	int gridIndex = ui.gridSizeCombo->findText(QString("%1").arg(m_grid->m_steps));
	if (gridIndex > 0)
		ui.gridSizeCombo->setCurrentIndex(gridIndex);
}


void MidijStateEditor::stateSelect(int index)
{
	QRectF rect;
	m_selectedState = index;

	if (index < m_rects.size())
	{
		rect = m_rects.at(index);
	}

	if (rect.isEmpty())
	{
		m_selectionItem->setVisible(false);
	}
	else
	{
		m_selectionItem->setRect(rect);
		m_selectionItem->setVisible(true);
	}
}


void MidijStateEditor::zoomIn()
{
	ui.graphicsView->scale(2,2);
}

void MidijStateEditor::zoomOut()
{
	ui.graphicsView->scale(0.5,0.5);
}

void MidijStateEditor::gridVisible(bool vibible)
{
	m_grid->setVisible(vibible);
}

void MidijStateEditor::gridSizeSelect(const QString& select)
{
//	m_grid->setVisible(vibible);

	ui.gridExtraCombo->blockSignals(true);

	ui.gridExtraCombo->clear();
	ui.gridExtraCombo->addItem("Last");
	int n = select.toInt();
	for (int i = 1; i <= n; i++)
		ui.gridExtraCombo->addItem(QString("%1").arg(i));

	ui.gridExtraCombo->blockSignals(false);

	m_grid->m_steps = n;
	m_grid->m_extraSpaceStep = 0;
	m_grid->update();
}


void MidijStateEditor::gridExtraSelect(const QString& select)
{
	int n = select.toInt();
	m_grid->m_extraSpaceStep = n;
	m_grid->update();
}

void MidijStateEditor::sceneRectChanged(const QRectF rect)
{
	QString areaValue = QString("%1,%2,%3,%4")
			.arg((int)rect.x())
			.arg((int)rect.y())
			.arg((int)rect.width())
			.arg((int)rect.height());
	int line = m_lines.at(m_selectedState);

	QTextCursor tc(ui.plainTextEdit->document());
	tc.setPosition(0,QTextCursor::MoveAnchor);
	tc.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line);

//	tc.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, areaNode.columnNumber() - 6 ); // - len("<area>")
	tc.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
	QString state = tc.selectedText();

	int i1 = state.indexOf("area=\"");

	if (i1 > 0)
	{
		m_rects.replace(m_selectedState, rect);
		i1 += 6;
		int i2 = state.indexOf("\"", i1);
		state.replace(i1, i2 - i1, areaValue);
		tc.removeSelectedText();
		tc.insertText(state);
	}
}
