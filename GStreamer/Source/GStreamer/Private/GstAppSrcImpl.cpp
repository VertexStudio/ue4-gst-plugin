#include "GstAppSrcImpl.h"
#include "GstPipelineImpl.h"
#include "GstSampleImpl.h"

extern "C"
{
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
}

#include <vector>
#include <mutex>

GstClockTime SecsToNano(float secs)
{
	return (GstClockTime)(secs * 1000000000);
}

class FGstAppSrcImpl : public IGstAppSrc
{
public:
	FGstAppSrcImpl() {}
	~FGstAppSrcImpl() { Disconnect(); }
	virtual void Destroy();

	virtual bool Connect(IGstPipeline *Pipeline, const char *ElementName, IGstAppSrcCallback *Callback);
	virtual void Disconnect();

	virtual void PushTexture(const uint8_t *TextureData, size_t TextureSize);

	virtual int GetTextureWidth()
	{
		return m_Width;
	}
	virtual int GetTextureHeight()
	{
		return m_Height;
	}
	virtual EGstTextureFormat GetTextureFormat()
	{
		return m_Format;
	}

	void OnNeedData(GstElement *Sink, guint size);

private:
	std::string m_Name;
	IGstAppSrcCallback *m_Callback = nullptr;
	GstElement *m_AppSrc = nullptr;

	gint m_Width = 0;
	gint m_Height = 0;
	EGstTextureFormat m_Format;
	gint m_Framerate = 1;

	GstClockTime m_Timestamp = 0;
};

IGstAppSrc *IGstAppSrc::CreateInstance(const char *ElementName)
{
	auto Obj = new FGstAppSrcImpl();
	GST_LOG_DBG_A("GstAppSrc: CreateInstance %p %s", Obj, ElementName);
	return Obj;
}

void FGstAppSrcImpl::Destroy()
{
	GST_LOG_DBG_A("GstAppSrc: Destroy %p %s", this, m_Name.c_str());
	delete this;
}

static void NeedDataFunc(GstElement *Sink, guint Size, FGstAppSrcImpl *Context) { Context->OnNeedData(Sink, Size); }

bool FGstAppSrcImpl::Connect(IGstPipeline *Pipeline, const char *ElementName, IGstAppSrcCallback *Callback)
{
	GST_LOG_DBG_A("GstAppSrc: Connect <%s>", ElementName);

	if (m_AppSrc)
	{
		GST_LOG_ERR_A("GstAppSrc: Already connected");
		return false;
	}

	for (;;)
	{
		m_Name = ElementName;
		m_Callback = Callback;

		m_AppSrc = gst_bin_get_by_name(GST_BIN(Pipeline->GetGPipeline()), ElementName);
		if (!m_AppSrc)
		{
			GST_LOG_ERR_A("gst_bin_get_by_name failed");
			break;
		}

		GstCaps *caps = gst_app_src_get_caps(GST_APP_SRC(m_AppSrc));
		guint num_caps = gst_caps_get_size(caps);
		if (num_caps > 0)
		{
			gchar *format;
			gint fps_n = 0, fps_d;
			GstStructure *st = gst_caps_get_structure(caps, 0);
			if (gst_structure_get(st,
								  "width", G_TYPE_INT, &m_Width,
								  "height", G_TYPE_INT, &m_Height,
								  "format", G_TYPE_STRING, &format,
								  "framerate", GST_TYPE_FRACTION, &fps_n, &fps_d,
								  NULL))
			{
				m_Framerate = fps_n / fps_d;
				GST_LOG_DBG_A("GstAppSrc: Found CAPS width:%i height:%i format:%s fps:%i", m_Width, m_Height, format, m_Framerate);
				if (strncmp(format, "BGRA", 4) == 0)
				{
					m_Format = EGstTextureFormat::GST_VIDEO_FORMAT_BGRA;
				}
				g_free(format);
			}
		}
		gst_caps_unref(caps);

		g_object_set(m_AppSrc,
					 "emit-signals", TRUE,
					 "do-timestamp", TRUE,
					 "format", GST_FORMAT_TIME,
					 nullptr);

		g_signal_connect(m_AppSrc, "need-data", G_CALLBACK(NeedDataFunc), this);

		GST_LOG_DBG_A("GstAppSrc: Connect SUCCESS");
		return true;
	}

	GST_LOG_ERR_A("GstAppSrc: Connect FAILED");
	Disconnect();
	return false;
}

void FGstAppSrcImpl::Disconnect()
{
	if (m_AppSrc)
	{
		GST_LOG_DBG_A("GstAppSrc: Disconnect <%s>", m_Name.c_str());
		gst_app_src_end_of_stream(GST_APP_SRC(m_AppSrc));
		g_object_set(m_AppSrc, "emit-signals", FALSE, nullptr);
		gst_object_unref(m_AppSrc);
		m_AppSrc = nullptr;
	}

	m_Width = 0;
	m_Height = 0;
	m_Timestamp = 0;
}

void FGstAppSrcImpl::PushTexture(const uint8_t *TextureData, size_t TextureSize)
{
	GstBuffer *buffer = gst_buffer_new_allocate(nullptr, TextureSize, nullptr);
	gst_buffer_fill(buffer, 0, TextureData, TextureSize);

	GST_BUFFER_PTS(buffer) = m_Timestamp;
	GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_SECOND, m_Framerate);
	m_Timestamp += GST_BUFFER_DURATION(buffer);

	gst_app_src_push_buffer(GST_APP_SRC(m_AppSrc), buffer);
	// const GstFlowReturn result = gst_app_src_push_buffer(GST_APP_SRC(m_AppSrc), buffer);
	// GST_LOG_DBG_A("GstAppSrc: GstFlowReturn <%s> TextureSize <%i>", gst_flow_get_name(result), TextureSize);
}

void FGstAppSrcImpl::OnNeedData(GstElement *Sink, guint Size)
{
	m_Callback->CbGstPushTexture();
}