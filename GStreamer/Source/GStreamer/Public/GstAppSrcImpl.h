#pragma once

#include "GstCoreImpl.h"

enum class EGstTextureFormat : unsigned char
{
  GST_VIDEO_FORMAT_UNKNOWN,
  GST_VIDEO_FORMAT_ENCODED,
  GST_VIDEO_FORMAT_I420,
  GST_VIDEO_FORMAT_YV12,
  GST_VIDEO_FORMAT_YUY2,
  GST_VIDEO_FORMAT_UYVY,
  GST_VIDEO_FORMAT_AYUV,
  GST_VIDEO_FORMAT_RGBx,
  GST_VIDEO_FORMAT_BGRx,
  GST_VIDEO_FORMAT_xRGB,
  GST_VIDEO_FORMAT_xBGR,
  GST_VIDEO_FORMAT_RGBA,
  GST_VIDEO_FORMAT_BGRA,
};

class IGstAppSrcCallback
{
public:
	virtual void CbGstPushTexture() = 0;
};

class IGstAppSrc
{
	GST_INTERFACE_IMPL(IGstAppSrc)

  public:
	virtual bool Connect(class IGstPipeline *Pipeline, const char *ElementName, IGstAppSrcCallback* Callback) = 0;
	virtual void Disconnect() = 0;
	virtual void PushTexture(const uint8_t *TextureData, size_t TextureSize) = 0;

	virtual int GetTextureWidth() = 0;
	virtual int GetTextureHeight() = 0;
	virtual EGstTextureFormat GetTextureFormat() = 0;
};
