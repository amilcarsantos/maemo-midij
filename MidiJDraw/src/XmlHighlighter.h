/*
 * XmlHighlighter.h
 *
 *  Created on: 28 de Jun de 2011
 *      Author: pt101821
 */

#ifndef XMLHIGHLIGHTER_H_
#define XMLHIGHLIGHTER_H_

#include <QSyntaxHighlighter>

class XmlHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	enum Construct
	{
		Entity,
		Tag,
		Attribute,
		Comment,
		LastConstruct = Comment
	};

	XmlHighlighter(QTextDocument *doc, QObject* parent=0);
	virtual ~XmlHighlighter();

	void setFormatFor(Construct construct, const QTextCharFormat &format);
protected:
	enum State
	{
		NormalState = -1,
		InComment,
		InTag
	};

	void highlightBlock(const QString &text);

private:
	QTextCharFormat m_formats[LastConstruct + 1];};

#endif /* XMLHIGHLIGHTER_H_ */
