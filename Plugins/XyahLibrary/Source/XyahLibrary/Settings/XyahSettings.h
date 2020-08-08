// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XyahSettings.generated.h"

UCLASS(config = XyahSettings, DefaultConfig, Blueprintable)
class XYAHLIBRARY_API UXyahBaseSettings : public UObject
{
	GENERATED_BODY()

public:

	UXyahBaseSettings();

	// Called when a Property for the Specified Class Default Object in Project/Xyah/Core CustomSettingsClass has been Modified in Settings
	UFUNCTION(BlueprintNativeEvent, Category = "Xyah")
	bool OnSettingsModified();

};

USTRUCT(BlueprintType)
struct XYAHLIBRARY_API FXyahSettingsInfo
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = "Class Settings")
	FString SettingsName;

	UPROPERTY(Config, EditAnywhere, Category = "Class Settings")
	TSubclassOf<UXyahBaseSettings> Class;

	UPROPERTY(Config, EditAnywhere, Category = "Class Settings")
	FString Description;
};


/**
 * 
 */
UCLASS(config = XyahSettings, DefaultConfig)
class XYAHLIBRARY_API UXyahSettings : public UObject
{
	GENERATED_BODY()
	

public:
	
	//The Settings Classes to use. Must restart Editor to see the changes.
	UPROPERTY(Config, EditAnywhere, Category = "Xyah")
    TArray<FXyahSettingsInfo> Settings;

};



