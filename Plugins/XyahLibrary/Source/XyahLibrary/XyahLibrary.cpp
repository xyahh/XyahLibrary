// Copyright Epic Games, Inc. All Rights Reserved.

#include "XyahLibrary.h"
#include "XyahSettings.h"

#define LOCTEXT_NAMESPACE "FXyahLibraryModule"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#endif

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
		ISettingsSectionPtr XyahSettings = SettingsModule->RegisterSettings("Project", "Xyah", "Config"
			, FText::FromString("Config")
			, FText::FromString("Xyah Library Plugin Core Settings"),
			GetMutableDefault <UXyahConfig>());

		//Add Config as a Reserved Name
		ReservedSettingsNames.Add("Config");

		if (XyahSettings.IsValid())
		{
			//Register the auto-save processing at the time of update
			XyahSettings->OnModified().BindRaw(this, &FXyahLibraryModule::OnSettingsModified);
		}

		if (const UXyahConfig* XyahCore = GetDefault<UXyahConfig>())
		{
			if (XyahCore->Settings.Num() > 0)
				RegisteredSettingsNames.Reserve(XyahCore->Settings.Num());

			for (auto& SettingsClass : XyahCore->Settings)
			{
				//Make sure Class is valid AND that the Setting we are assigning is NOT a Reserved (in-use) Setting Name.
				if (SettingsClass.Class && false == ReservedSettingsNames.Contains(SettingsClass.SettingsName))
				{
					UXyahSettings* SettingsObject = GetMutableDefault<UXyahSettings>(SettingsClass.Class);
					
					if(SettingsClass.bSaveAllConfig)
						SettingsObject->SaveSettings();

					RegisteredSettingsNames.Add(SettingsClass.SettingsName);
					ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Xyah", *SettingsClass.SettingsName
						, FText::FromString(SettingsClass.SettingsName)
						, FText::FromString(SettingsClass.Description),
						SettingsObject);

					if (SettingsSection.IsValid())
					{
						SettingsSection->OnModified().BindUObject(SettingsObject, &UXyahSettings::OnSettingsModified);
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