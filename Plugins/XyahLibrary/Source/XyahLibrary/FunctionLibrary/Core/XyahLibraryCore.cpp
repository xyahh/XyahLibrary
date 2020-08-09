// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.
#include "XyahLibraryCore.h"

DEFINE_LOG_CATEGORY(XyahLibLog);

uint64 ConvertXyahPropertyFlags(int32 XyahPropertyFlags)
{
#define CONVERT(PropertyFlag) \
	if(XyahPropertyFlags & static_cast<int32>(EXyahPropertyFlags::XPF_##PropertyFlag)) \
		OutPropertyFlags |= static_cast<uint64>(EPropertyFlags::CPF_##PropertyFlag);

	uint64 OutPropertyFlags = 0;

	CONVERT(DisableEditOnInstance);
	CONVERT(BlueprintVisible);
	CONVERT(BlueprintReadOnly);
	CONVERT(Deprecated);
	CONVERT(Net);
	CONVERT(SaveGame);
	CONVERT(Config);
	CONVERT(Transient);

	return OutPropertyFlags;
}
