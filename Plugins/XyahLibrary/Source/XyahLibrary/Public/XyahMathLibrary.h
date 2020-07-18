// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XyahMathLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//Converts a given Integer Number to a String expressing the Number in Binary Form
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math")
	static FString ToBinaryString(int32 Number);

};
