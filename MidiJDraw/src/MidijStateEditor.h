#ifndef MIDIJSTATEEDITOR_H
#define MIDIJSTATEEDITOR_H

#include <QtGui/QDialog>
#include "ui_MidijStateEditor.h"
#include <QDomElement>
#include "MidijDrawGraphicsScene.h"


class GraphicsGrid : public QGraphicsRectItem
{
public:
	GraphicsGrid();
	~GraphicsGrid();

public:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w);

	int m_steps;
	int m_extraSpaceStep;
};



class MidijStateEditor : public QDialog
{
    Q_OBJECT

public:
    MidijStateEditor(QWidget *parent = 0);
    ~MidijStateEditor();

    void setPixmap(const QPixmap& pixmap);
    void setElement(QDomElement node);

public slots:
	void stateSelect(int);
	void zoomIn();
	void zoomOut();
	void gridVisible(bool);
	void gridSizeSelect(const QString &);
	void gridExtraSelect(const QString &);
	void sceneRectChanged(const QRectF);

private:
    Ui::MidijStateEditorClass ui;

    int m_selectedState;
	GraphicsGrid* m_grid;
	GraphicsSelectionItem* m_selectionItem;
	QRectF m_maxRect;

	QVector<QRectF> m_rects;
	QVector<int> m_lines;
};

#endif // MIDIJSTATEEDITOR_H
