// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XyahUtilityLibrary.generated.h"

#define XYAH_UTILITY_LIBRARY

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
//Blueprints
public:

	static void Print(const FString& String);
	
//Unreal C++
public:

	static bool ArePropertiesTheSame(const FProperty* A, const FProperty* B, bool bCheckPropertyOffsets = false, bool bCheckPropertyNames = false);

	template<class... Args>
	static void Print(const FString& Format, Args&&... Arguments);

//Internal Use 
protected:

};

template<class... Args>
void UXyahUtilityLibrary::Print(const FString& Format, Args&&... Arguments)
{
	Print(FString::Printf(Format, Arguments));
}
