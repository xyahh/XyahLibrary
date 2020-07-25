// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "XyahLibraryHub.h"


class FXyahLibraryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


protected:

	void RegisterSettings();
	bool OnSettingsModified();
	void UnregisterSettings();
};



