// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XyahSettings.generated.h"

UCLASS(config = Game, DefaultConfig, Blueprintable)
class XYAHLIBRARY_API UXyahBaseSettings : public UObject
{
	GENERATED_BODY()

public:

	// Called when a Property for the Specified Class Default Object in Project/Xyah/Core CustomSettingsClass has been Modified in Settings
	UFUNCTION(BlueprintNativeEvent, Category = "Xyah")
	bool OnSettingsModified();

};

/**
 * 
 */
UCLASS(config = Game, DefaultConfig)
class XYAHLIBRARY_API UXyahSettings : public UObject
{
	GENERATED_BODY()
	

public:

	UPROPERTY(Config, EditAnywhere, Category = "Xyah")
    TSubclassOf<UXyahBaseSettings> CustomSettingsClass;

};



