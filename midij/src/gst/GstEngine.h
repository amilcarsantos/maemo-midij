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

#ifndef GSTENGINE_H_
#define GSTENGINE_H_

#include <QtCore>
#include <glib/gerror.h>
#include <gst/gst.h>

class GstEngine : public QObject
{
	Q_OBJECT
public:
	GstEngine(QObject* parent=0);
	virtual ~GstEngine();

public slots:
	void play();
	void pause();
	void stop();
	virtual void setVolume(int percent);

public:
	void init();
	bool isPlaying() const;
	bool isPaused() const;
	bool seek(uint ms);
	uint position() const;
	uint duration() const;
	virtual int volume();

	bool hasError() const;
	QString errorMessage();

protected:
	// Creates the gstreamer elements, returns error string or NULL if all elements are loaded
	virtual const char* createElements() = 0;
	void addElement(GstElement* element);
	void removeElement(GstElement* element);
	static gboolean bus_call(GstBus* bus, GstMessage* msg, void* user_data);

protected:
	GstElement* m_pipeline;
	QString m_errorMessage;
};

#endif /* GSTENGINE_H_ */
