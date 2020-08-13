// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/* XYAH LOGS */
DECLARE_LOG_CATEGORY_EXTERN(XyahComponentsLog, Log, All);
#define XYAH_CMP_LOG(Verbosity, Format, ...) UE_LOG(XyahComponentsLog, Verbosity, Format, __VA_ARGS__)