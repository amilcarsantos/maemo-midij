/*
 * gst.cpp
 *
 *  Created on: 4 de Dez de 2011
 *      Author: Amilcar
 */

#include <QDebug>
#include "gst.h"

void gst_init(int* c, char*** argv)
{
	qDebug("gst_init()");
}

GstElement* gst_pipeline_new(const char* name)
{
	qDebug(QString("gst_pipeline_new() - name:'%1'").arg(name).toAscii());
	static GstElement pipeline;
	::strcpy(pipeline.name, name);
	return &pipeline;
}

void g_object_get(GstElement* e, char* attr, ...)
{
	qDebug(QString("g_object_get() - element:'%1'; attr:'%2'")
			.arg(e->name).arg(attr).toAscii());
}

void g_object_set(GstElement* e, char* attr, ...)
{
	qDebug(QString("g_object_set() - element:'%1'; attr:'%2'")
			.arg(e->name).arg(attr).toAscii());
}


void gst_object_unref(GstElement* e)
{
	qDebug(QString("gst_object_unref() - element:'%1'").arg(e->name).toAscii());
}

void gst_bin_add(GstElement* bin, GstElement* e)
{
	qDebug(QString("gst_bin_add() - bin:'%1'; element:'%2'")
			.arg(bin->name).arg(e->name).toAscii());
}


void gst_bin_remove(GstElement* bin, GstElement* e)
{
	qDebug(QString("gst_bin_remove() - bin:'%1'; element:'%2'")
			.arg(bin->name).arg(e->name).toAscii());
}


void gst_element_set_state(GstElement* e, int state)
{
	qDebug("gst_element_set_state()");
}


void gst_element_get_state(GstElement* e, GstState* state, int x, int y)
{
	qDebug("gst_element_get_state()");
}

bool gst_element_seek (GstElement* e, double a, int format, int flag1, int flag2, int time, int flag3, int flag4)
{
	qDebug("gst_element_seek()");
	return false;
}

void gst_element_query_position(GstElement* e, GstFormat* fmt, qint64* value)
{
	qDebug("gst_element_query_position()");
}

void gst_element_query_duration(GstElement* e, GstFormat* fmt, qint64* value)
{
	qDebug("gst_element_query_duration()");
}

GstElement* gst_element_factory_make(const char* type, const char* name)
{
	qDebug(QString("gst_element_factory_make() - type:'%1'; name:'%2'")
			.arg(type).arg(name).toAscii());

	static QList<GstElement> elements;
	GstElement element;
	::strcpy(element.name, name);
	elements.append(element);
	return &elements.last();
}

GstBus* gst_pipeline_get_bus(GstElement* e)
{
	qDebug(QString("gst_pipeline_get_bus() - type:'%1'").arg(e->name).toAscii());
	static GstBus bus;
	strcpy(bus.name, "bus");
	return &bus;
}

void gst_bus_add_watch(GstBus * bus, GstBusFunc func, void* user_data)
{
	qDebug("gst_bus_add_watch()");
}

void gst_message_parse_error(GstMessage* msg, GError** error, void* debug)
{
	qDebug("gst_message_parse_error()");
	static GError gerror;
	::strcpy(gerror.message, "DUMMY_ERROR");
	*error = &gerror;
}


void gst_element_link_many(GstElement* e, ...)
{
	qDebug("gst_element_link_many()");
}
