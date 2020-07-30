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

		//Add Core as a Reserved Name
		ReservedSettingsNames.Add("Core");

		if (XyahSettings.IsValid())
		{
			//Register the auto-save processing at the time of update
			XyahSettings->OnModified().BindRaw(this, &FXyahLibraryModule::OnSettingsModified);
		}

		if (const UXyahSettings* XyahSettingsObj = GetDefault<UXyahSettings>())
		{
			if (XyahSettingsObj->Settings.Num() > 0)
				RegisteredSettingsNames.Reserve(XyahSettingsObj->Settings.Num());

			for (auto& SettingsClass : XyahSettingsObj->Settings)
			{
				//Make sure Class is valid AND that the Setting we are assigning is NOT a Reserved (in-use) Setting Name.
				if (SettingsClass.Class && false == ReservedSettingsNames.Contains(SettingsClass.SettingsName))
				{
					UXyahBaseSettings* SettingsObj = GetMutableDefault<UXyahBaseSettings>(SettingsClass.Class);

					RegisteredSettingsNames.Add(SettingsClass.SettingsName);
					ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Xyah", *SettingsClass.SettingsName
						, FText::FromString(SettingsClass.SettingsName)
						, FText::FromString(SettingsClass.Description),
						SettingsObj);

					if (SettingsSection.IsValid())
					{
						SettingsSection->OnModified().BindUObject(SettingsObj, &UXyahBaseSettings::OnSettingsModified);
					}
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
		for (auto& SettingsName : RegisteredSettingsNames)
			SettingsModule->UnregisterSettings("Project", "Xyah", *SettingsName);
		RegisteredSettingsNames.Empty();
	}
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXyahLibraryModule, XyahLibrary)