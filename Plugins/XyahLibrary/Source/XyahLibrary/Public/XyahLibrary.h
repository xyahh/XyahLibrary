// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define XYAH(Lib) UXyah##Lib##Library::

DECLARE_LOG_CATEGORY_EXTERN(XyahLog, Log, All);

#define XYAH_LOG(Verbosity, Format, ...) UE_LOG(XyahLog, Verbosity, Format, __VA_ARGS__)

class FXyahLibraryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
