// Copyright Epic Games, Inc. All Rights Reserved.

#include "XyahLibrary.h"
#include "../Settings/XyahSettings.h"

#define LOCTEXT_NAMESPACE "FXyahLibraryModule"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#endif

DEFINE_LOG_CATEGORY(XyahLog);

void FXyahLibraryModule::StartupModule()
{
	RegisterSettings();
}

void FXyahLibraryModule::ShutdownModule()
{
	UnregisterSettings();
}

void FXyahLibraryModule::RegisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr <ISettingsModule>("Settings"))
	{
		//add a category for the set
		ISettingsSectionPtr XyahSettings = SettingsModule->RegisterSettings("Project", "Xyah", "Core"
			, FText::FromString("Core")
			, FText::FromString("Xyah Core Settings"),
			GetMutableDefault <UXyahSettings>());

		if (XyahSettings.IsValid())
		{
			//Register the auto-save processing at the time of update
			XyahSettings->OnModified().BindRaw(this, &FXyahLibraryModule::OnSettingsModified);
		}

		if (const UXyahSettings* XyahSettingsObj = GetDefault<UXyahSettings>())
		{
			if (XyahSettingsObj->CustomSettingsClass)
			{
				ISettingsSectionPtr CustomSettings = SettingsModule->RegisterSettings("Project", "Xyah", "Custom"
					, FText::FromString("Custom")
					, FText::FromString("Xyah Custom Settings"),
					GetMutableDefault<UXyahBaseSettings>(XyahSettingsObj->CustomSettingsClass));

				if (CustomSettings.IsValid())
				{
					CustomSettings->OnModified().BindUObject(GetMutableDefault<UXyahBaseSettings>(XyahSettingsObj->CustomSettingsClass), &UXyahBaseSettings::OnSettingsModified);
				}
			}
		}
		
	}
#endif
}

bool FXyahLibraryModule::OnSettingsModified()
{
	return true;
}

void FXyahLibraryModule::UnregisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr <ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Xyah", "Core");
		SettingsModule->UnregisterSettings("Project", "Xyah", "Custom");
	}
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXyahLibraryModule, XyahLibrary)