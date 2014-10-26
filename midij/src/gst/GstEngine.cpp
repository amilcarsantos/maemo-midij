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

#include "GstEngine.h"

GstEngine::GstEngine(QObject *parent)
	: QObject(parent)
{
	gst_init(NULL, NULL);
	m_pipeline = gst_pipeline_new("pipeline");
}

GstEngine::~GstEngine()
{
	gst_object_unref(GST_OBJECT(m_pipeline));
}

void GstEngine::init()
{
	const char* errorMessage = createElements();
	if (errorMessage != NULL)
		m_errorMessage = QString(errorMessage);
	GstBus* bus = gst_pipeline_get_bus (GST_PIPELINE (m_pipeline));
	gst_bus_add_watch(bus, GstEngine::bus_call, this);
	gst_object_unref(bus);
}

bool GstEngine::hasError() const
{
	return !m_errorMessage.isEmpty();
}

QString GstEngine::errorMessage()
{
	QString errorMessage = m_errorMessage;
	m_errorMessage.clear();
	return errorMessage;
}

gboolean GstEngine::bus_call(GstBus* bus, GstMessage* msg, void* user_data)
{
	Q_UNUSED(bus);
	Q_UNUSED(user_data);
	if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR)
	{
		GError* error;
		gst_message_parse_error(msg, &error, NULL);
//		g_error("%s", error->message);
		if (user_data != NULL)
		{
			GstEngine* pThis = (GstEngine*)user_data;
			pThis->m_errorMessage = QString("Failure in stream: %1").arg(error->message);
		}
		g_error_free(error);
	}
	return true;
}

void GstEngine::addElement(GstElement* element)
{
	gst_bin_add(GST_BIN(m_pipeline), element);
}

void GstEngine::removeElement(GstElement* element)
{
	gst_bin_remove(GST_BIN(m_pipeline), element);
}


void GstEngine::play()
{
	gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_PLAYING);
}

void GstEngine::stop()
{
	gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_NULL);
}

void GstEngine::pause()
{
	gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_PAUSED);
}

bool GstEngine::isPlaying() const
{
	GstState state;
	gst_element_get_state(GST_ELEMENT(m_pipeline), &state, NULL, -1);
	return state == GST_STATE_PLAYING;
}

bool GstEngine::isPaused() const
{
	GstState state;
	gst_element_get_state(GST_ELEMENT(m_pipeline), &state, NULL, -1);
	return state == GST_STATE_PAUSED;
}

bool GstEngine::seek(uint ms)
{
	gst_element_get_state(GST_ELEMENT(m_pipeline), NULL, NULL, 100*GST_MSECOND);
	if (!gst_element_seek (GST_ELEMENT(m_pipeline), 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, ms*GST_MSECOND, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE))
	{
		qWarning("Can't seek");
		return false;
	}
	return true;
}

uint GstEngine::position() const
{
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 value = 0;
	gst_element_query_position(GST_ELEMENT(m_pipeline), &fmt, &value);
	return static_cast<uint>((value / GST_MSECOND));
}

uint GstEngine::duration() const
{
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 value = 0;
	gst_element_query_duration(GST_ELEMENT(m_pipeline), &fmt, &value);
	return static_cast<uint>((value / GST_MSECOND));
}

void GstEngine::setVolume(int percent)
{
	Q_UNUSED(percent);
    // g_object_set(G_OBJECT(mPlayer), "volume", percent*0.01, NULL );
}

int GstEngine::volume()
{
    // double volume;
    // g_object_get(G_OBJECT(mPlayer), "volume", &volume, NULL );
    // return volume * 100;
	return -1;
}

