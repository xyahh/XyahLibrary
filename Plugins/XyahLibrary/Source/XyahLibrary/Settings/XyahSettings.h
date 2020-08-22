// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XyahSettings.generated.h"

UCLASS(config = XyahSettings, Blueprintable)
class XYAHLIBRARY_API UXyahSettings : public UObject
{
	GENERATED_BODY()

public:

	UXyahSettings();

	// Called when a Property for the Specified Class Default Object in Project/Xyah/Core CustomSettingsClass has been Modified in Settings
	UFUNCTION(BlueprintNativeEvent, Category = "Xyah")
	bool OnSettingsModified();

	void SaveSettings();
};

UCLASS(config = XyahSettings, DefaultConfig, Blueprintable)
class XYAHLIBRARY_API UXyahDefaultSettings : public UXyahSettings
{
	GENERATED_BODY()
};


USTRUCT(BlueprintType)
struct XYAHLIBRARY_API FXyahSettingsInfo
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = "Class Settings")
	FString SettingsName;

	UPROPERTY(Config, EditAnywhere, Category = "Class Settings")
	TSubclassOf<UXyahSettings> Class;

	UPROPERTY(Config, EditAnywhere, Category = "Class Settings")
	FString Description;

	//Whether to save all the Configuration regardless if it has not been changed from its Original Default values.
	UPROPERTY(Config, EditAnywhere, Category = "Class Settings")
	bool	bSaveAllConfig;
};


/**
 * Do not use this class for additional Settings!
 * Instead, use XyahSettings & XyahDefaultSettings!
 */
UCLASS(config = XyahSettings, DefaultConfig, NotBlueprintable, HideDropdown)
class XYAHLIBRARY_API UXyahConfig final : public UObject 
{
	GENERATED_BODY()
	

public:
	
	//The Settings Classes to add when the Editor loads. Must restart Editor to see the changes.
	UPROPERTY(Config, EditAnywhere, Category = "Custom Settings")
    TArray<FXyahSettingsInfo> Settings;

};



