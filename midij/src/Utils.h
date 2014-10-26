/*
 * MidiJ - Remote MIDI controller for DJs
 * Copyright (C) 2010  Amilcar Santos <amilcar.santos@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef UTILS_H_
#define UTILS_H_


// delete Non-NULL variables and set NULL
#define _delete(x) if (x){delete x; x=NULL;}

#define MIDIJ_MAJORVERSION_HI 	0
#define MIDIJ_MAJORVERSION_LO	1
#define MIDIJ_MINORVERSION_HI	4
#define MIDIJ_MINORVERSION_LO	0

#include <QFileInfo>
#include <QDir>

class DirContext
{
public:
	DirContext(const QString& dir);
	DirContext(const QFileInfo& fileInfo);
	virtual ~DirContext();

	bool isValid();
	void clear();

protected:
	QString m_storedPath;
};

/////////////////////////////////////////////////
// Implementation
inline DirContext::DirContext(const QString& dir)
{
	m_storedPath = QDir::currentPath();
	if (!QDir::setCurrent(dir))
		m_storedPath.clear();
}

inline DirContext::DirContext(const QFileInfo& fileInfo)
{
	m_storedPath = QDir::currentPath();
	if (!QDir::setCurrent(fileInfo.absolutePath()))
		m_storedPath.clear();
}

inline DirContext::~DirContext()
{
	if (isValid())
		QDir::setCurrent(m_storedPath);
}


inline bool DirContext::isValid()
{
	return !m_storedPath.isEmpty();
}

inline void DirContext::clear()
{
	if (isValid())
	{
		QDir::setCurrent(m_storedPath);
		m_storedPath.clear();
	}
}

#endif /* UTILS_H_ */
