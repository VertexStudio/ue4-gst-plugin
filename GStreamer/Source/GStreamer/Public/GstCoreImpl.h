#pragma once

#include "Shared.h"

#define GST_INTERFACE_IMPL(Name)\
	friend class FGstCoreImpl;\
	protected:\
	Name() {}\
	virtual ~Name() {}\
	public:\
	static Name* CreateInstance(const char *ElementName);\
	virtual void Destroy() = 0;\
	private:

class FGstCoreImpl
{
public:

	static bool Init(const char* BinPath, const char* PluginPath);
	static void Deinit();
};
