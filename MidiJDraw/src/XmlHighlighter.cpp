/*
 * XmlHighlighter.cpp
 *
 *  Created on: 28 de Jun de 2011
 *      Author: pt101821
 */

#include "XmlHighlighter.h"

XmlHighlighter::XmlHighlighter(QTextDocument *doc, QObject* parent)
	: QSyntaxHighlighter(parent)
{
	setDocument(doc);

	QTextCharFormat entityFormat;
	entityFormat.setForeground(QColor(0, 128, 0));
	//entityFormat.setFontWeight(QFont::Bold);
	setFormatFor(Entity, entityFormat);

	QTextCharFormat tagFormat;
	tagFormat.setForeground(QColor(63, 127, 127));
	//tagFormat.setFontWeight(QFont::Bold);
	setFormatFor(Tag, tagFormat);

	QTextCharFormat attributeFormat;
	attributeFormat.setForeground(QColor(42, 0, 255));
	//attributeFormat.setFontWeight(QFont::Bold);
	setFormatFor(Attribute, attributeFormat);

	QTextCharFormat commentFormat;
	commentFormat.setForeground(QColor(63, 95, 191));
	commentFormat.setFontItalic(true);
	setFormatFor(Comment, commentFormat);
}

XmlHighlighter::~XmlHighlighter()
{
}


void XmlHighlighter::highlightBlock(const QString &text)
{
	int state = previousBlockState();
	int len = text.length();
	int start = 0;
	int pos = 0;
	while (pos < len)
	{
		switch (state)
		{
		case NormalState:
		default:
			while (pos < len)
			{
				QChar ch = text.at(pos);
				if (ch == '<')
				{
					if (text.mid(pos, 4) == "<!--")
						state = InComment;
					else
						state = InTag;
					break;
				}
				else if (ch == '&')
				{
					start = pos;
					while (pos < len && text.at(pos++) != ';')
						;
					setFormat(start, pos - start, m_formats[Entity]);
				}
					else ++pos;
			}
			break;
		case InComment:
			start = pos;
			while (pos < len)
			{
				if (text.mid(pos, 3) == "-->")
				{
					pos += 3;
					state = NormalState;
					break;
				}
				else
				{
					++pos;
				}
			}
			setFormat(start, pos - start, m_formats[Comment]);
			break;
		case InTag:
			QChar quote = QChar::Null;
			int quoteStart = 0;
			start = pos;
			while (pos < len)
			{
				QChar ch = text.at(pos);
				if (quote.isNull())
				{
					if (ch == '\'' || ch == '"')
					{
						quote = ch;
						quoteStart = pos;
					}
					else if (ch == '>')
					{
						++pos;
		                state = NormalState;
		                break;
					}
				}
				else if (ch == quote)
				{
					quote = QChar::Null;
					setFormat(start, quoteStart - start, m_formats[Tag]);
					setFormat(quoteStart, pos - quoteStart + 1, m_formats[Attribute]);
					start = pos+1;
				}
		        ++pos;
			}
			setFormat(start, pos - start, m_formats[Tag]);
		}
	}
	setCurrentBlockState(state);
}


void XmlHighlighter::setFormatFor(Construct construct, const QTextCharFormat &format)
{
	m_formats[construct] = format;
//	rehighlight();
}
