
#ifndef GST_H
#define GST_H


#define	G_BEGIN_DECLS
#define G_END_DECLS

typedef int GType;

struct GstElement
{
	char name[128];
};

struct GstBus : public GstElement
{
	int bus;
};

struct GstMessage
{
	int type;
};

struct GError
{
	char message[128];
};
typedef int GstState;
typedef qint64 gint64;
typedef qint64 guint64;
typedef bool gboolean;
typedef gboolean (*GstBusFunc) (GstBus * bus, GstMessage * message, void* data);


enum GstFormat
{
	GST_FORMAT_TIME
};

#define GST_STATE_NULL		0
#define GST_STATE_PLAYING	1
#define GST_STATE_PAUSED	2

#define GST_MSECOND			1
#define GST_SEEK_FLAG_FLUSH	1
#define GST_SEEK_TYPE_SET	1
#define GST_SEEK_TYPE_NONE 	1
#define GST_CLOCK_TIME_NONE	1
#define GST_MESSAGE_ERROR 	1


#define GST_OBJECT(e)	e
#define GST_BIN(e)		e
#define GST_ELEMENT(e)	e
#define GST_PIPELINE(e)	e
#define G_OBJECT(e)		e


#define GST_MESSAGE_TYPE(msg)	msg->type

#define g_error		printf
#define g_error_free(x)	(void)0


void gst_init(int* c, char*** argv);
GstElement* gst_pipeline_new(const char* name);
GstBus* gst_pipeline_get_bus(GstElement* e);
void gst_bus_add_watch(GstBus * bus, GstBusFunc func, void* user_data);

GstElement* gst_element_factory_make(const char* type, const char* name);
void g_object_get(GstElement* e, char* attr, ...);
void g_object_set(GstElement* e, char* attr, ...);


void gst_object_unref(GstElement* e);

void gst_bin_add(GstElement* bin, GstElement* e);
void gst_bin_remove(GstElement* bin, GstElement* e);
void gst_element_set_state(GstElement* e, int state);
void gst_element_get_state(GstElement* e, GstState* state, int x, int y);
bool gst_element_seek (GstElement* e, double a, int format, int flag1, int flag2, int time, int flag3, int flag4);
void gst_element_query_position(GstElement* e, GstFormat* fmt, qint64* value);
void gst_element_query_duration(GstElement* e, GstFormat* fmt, qint64* value);
void gst_element_link_many(GstElement* e, ...);

void gst_message_parse_error(GstMessage* msg, GError** error, void* debug);

#endif
