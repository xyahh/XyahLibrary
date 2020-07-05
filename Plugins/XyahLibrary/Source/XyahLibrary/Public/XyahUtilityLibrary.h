// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/UnrealType.h"
#include "Net/Core/PushModel/PushModel.h"
#include "XyahUtilityLibrary.generated.h"

#define XYAH_UTILITY

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
//Unreal Blueprints
public:

	//Gets a Random Element and its Index from a given Array
	UFUNCTION(BlueprintPure, CustomThunk, Category = "XyahUtility|Array", meta = (DisplayName = "GetRandom", CompactNodeTitle="RANDOM", ArrayParm="Array", ArrayTypeDependentParams="OutRandomElement"))
	static void Array_GetRandom(const TArray<int32>& Array, int32& OutRandomElement);

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahUtility|Array", meta = (DisplayName = "Sort", ArrayParm = "Array"))
	static bool Array_Sort(const TArray<int32>& Array, FName PredicateFunctionName);


//Unreal C++
public:


//Native C++
public:

	static void GenericArray_GetRandom(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem);

	static bool GenericArray_Sort(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
		, FName PredicateFunctionName);

public:

	DECLARE_FUNCTION(execArray_GetRandom)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		
		const FProperty* InnerProp = ArrayProperty->Inner;
		const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;
		void* StorageSpace = FMemory_Alloca(PropertySize);
		InnerProp->InitializeValue(StorageSpace);

		Stack.MostRecentPropertyAddress = NULL;
		Stack.StepCompiledIn<FProperty>(StorageSpace);
		void* ItemPtr = (Stack.MostRecentPropertyAddress != NULL && Stack.MostRecentProperty->GetClass() == InnerProp->GetClass()) ? Stack.MostRecentPropertyAddress : StorageSpace;

		P_FINISH;
		P_NATIVE_BEGIN;
		GenericArray_GetRandom(ArrayAddr, ArrayProperty, ItemPtr);
		P_NATIVE_END;
		InnerProp->DestroyValue(StorageSpace);
	}

	DECLARE_FUNCTION(execArray_Sort)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(nullptr);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_GET_PROPERTY(FNameProperty, PredicateFunctionName);
		P_FINISH;
		P_NATIVE_BEGIN;
		MARK_PROPERTY_DIRTY(Stack.Object, ArrayProperty);
		*(bool*)RESULT_PARAM = GenericArray_Sort(ArrayAddr, ArrayProperty, Stack.Object, PredicateFunctionName);
		P_NATIVE_END;;
	}
};

