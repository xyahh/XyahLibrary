// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XyahLibrary.h"
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
	
//Blueprint Exposed
protected:

	static void Print(const FString& String);

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH GetAllActorsOfClass", WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutActors", AutoCreateRefTerm = "ClassesToIgnore", AdvancedDisplay = "ClassesToIgnore,FilterFunctionName,FunctionOwner"))
	static bool BlueprintGetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName = NAME_None, UObject* FunctionOwner = nullptr);

	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH SetPropertyValue"))
	static bool BlueprintSetPropertyValue(UObject* OwnerObject, const FString& PropertyName, const FString& PropertyValue);

	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH GetPropertyValue"))
	static bool BlueprintGetPropertyValue(UObject* OwnerObject, const FString& PropertyName, FString& OutPropertyValue);

	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH Vector to String", CompactNodeTitle = "V3toSTR"))
	static FString BlueprintGetVectorString(const FVector& V);

	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH Rotator to String ", CompactNodeTitle = "RtoSTR"))
	static FString BlueprintGetRotatorString(const FRotator& R);

	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH Transform to String ", CompactNodeTitle = "TtoSTR"))
	static FString BlueprintGetTransformString(const FTransform& T);

	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH Vector2D to String ", CompactNodeTitle = "V2toSTR"))
	static FString BlueprintGetVector2DString(const FVector2D& V);

	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH String to Vector ", CompactNodeTitle = "STRtoV3"))
	static bool BlueprintGetVector(const FString& S, FVector& V);

	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH String to Rotator ", CompactNodeTitle = "STRtoR"))
	static bool BlueprintGetRotator(const FString& S, FRotator& R);

	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH String to Transform ", CompactNodeTitle = "STRtoT"))
	static bool BlueprintGetTransform(const FString& S, FTransform& T);

	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DisplayName = "XYAH String to Vector2D ", CompactNodeTitle = "STRtoV2"))
	static bool BlueprintGetVector2D(const FString& S, FVector2D& V);

//C++ Only
public:

	//Checks whether two properties are of the same class, type, etc. Additional checks such as Offsets, Names are also available
	static bool CheckProperties(const FProperty* A, const FProperty* B, bool bCheckPropertyOffsets = false, bool bCheckPropertyNames = false);

	template<class... Args>
	static void Print(const FString& Format, Args&&... Arguments);
	
	//Finds a Function from a Given Object
	static UFunction* FindFunction(const UObject* Object, FName FunctionName
		, const FString& ErrorString, FProperty* InnerProperty, int32 ExpectedInputParams);


//ExecFuncs
public:

	static void GenericBlueprintGetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FProperty* ArrayInnerProperty
		, TArray<AActor*>& OutActors
		, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName, UObject* FuncOwner);

public:

	DECLARE_FUNCTION(execBlueprintGetAllActorsOfClass)
	{
		P_GET_OBJECT(UObject, WorldContextObject);
		P_GET_OBJECT(UClass, ActorClass);

		P_GET_TARRAY_REF(AActor*, OutActors);
		FArrayProperty* ActorProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);

		P_GET_TSET_REF(TSubclassOf<AActor>, ClassesToIgnore);
		P_GET_PROPERTY(FNameProperty, FilterFuncName);
		P_GET_OBJECT(UObject, FuncOwner);
		P_FINISH;
		P_NATIVE_BEGIN;

		//No need to Mark property as dirty since we are getting Tarrayref
		GenericBlueprintGetAllActorsOfClass(WorldContextObject, ActorClass, ActorProperty ? ActorProperty->Inner : nullptr, OutActors, ClassesToIgnore, FilterFuncName, IsValid(FuncOwner) ? FuncOwner : XYAH_GET_OBJECT);
		P_NATIVE_END;
	}

};

template<class... Args>
void UXyahUtilityLibrary::Print(const FString& Format, Args&&... Arguments)
{
	Print(FString::Printf(Format, Arguments));
}
