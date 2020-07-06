// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/UnrealType.h"
#include "Net/Core/PushModel/PushModel.h"
#include "XyahArrayLibrary.generated.h"

#define XYAH_ARRAY_LIBRARY

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahArrayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
//Unreal Blueprints
public:

	/*
	Gets a Random Element from the Array

	@param Array - The Array to get a Random Element from
	@param OutRandomElement - the element gotten at random
	*/
	UFUNCTION(BlueprintPure, CustomThunk, Category = "XyahUtility|Array", meta = (DisplayName = "GetRandom", CompactNodeTitle="RANDOM", ArrayParm="Array", ArrayTypeDependentParams="OutRandomElement"))
	static void Array_GetRandom(const TArray<int32>& Array, int32& OutRandomElement);

	/*
	Sorts the given Array via a Predicate Function gotten via its Name and Function Owner.
	This Predicate Function will be called for every comparison. If the Function returns true, the swap will take place (i.e. out of order), else
	there will be no swap.
	
	@param Array - The Array to Sort.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally. 

	@retval bool - Whether Sorting actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahUtility|Array", meta = (DisplayName = "Sort", ArrayParm = "Array", AdvancedDisplay="FunctionOwner"))
	static bool Array_Sort(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);


	/*
	Removes the elements from the given array that satisfied the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every element. If the Function returns true, the element will be removed.

	@param Array - The Array to Remove Elements from. 
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@retval bool - Whether RemoveIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahUtility|Array", meta = (DisplayName = "RemoveIf", ArrayParm = "Array", AdvancedDisplay="FunctionOwner"))
	static bool Array_RemoveIf(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

//Unreal C++
public:

	

//Internal Use
protected:

	static void GenericArray_GetRandom(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem);

	static bool GenericArray_Sort(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
		, FName PredicateFunctionName);

	static bool GenericArray_RemoveIf(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
		, FName PredicateFunctionName);

	
//Internal Use - Helper Funcs
protected:

	//Checks that the Function has (Expected Input Params) num of (InnerProperty) properties
	static UFunction* FindFunction_Checked(UObject* Object, FName FunctionName
		, const FString& ErrorString, FProperty* InnerProperty, int32 ExpectedInputParams);

//A Messy Definition for Future Fast Implementation of Pred-based Funcs
public:

#define PREDICATE_BODY(GenericFunc) Stack.MostRecentProperty = nullptr; \
	Stack.StepCompiledIn<FArrayProperty>(nullptr); \
	void* ArrayAddr = Stack.MostRecentPropertyAddress; \
	FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty); \
	if (!ArrayProperty) \
	{\
		Stack.bArrayContextFailed = true;\
		return;\
	}\
	\
	P_GET_PROPERTY(FNameProperty, PredicateFunctionName);\
	P_GET_PROPERTY(FObjectProperty, FunctionOwner);\
	\
	P_FINISH;\
	P_NATIVE_BEGIN;\
	MARK_PROPERTY_DIRTY(Stack.Object, ArrayProperty);\
	*(bool*)RESULT_PARAM = GenericFunc(ArrayAddr, ArrayProperty, (FunctionOwner) ? FunctionOwner : Stack.Object, PredicateFunctionName);\
	P_NATIVE_END;

//ExecFuncs
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
		PREDICATE_BODY(GenericArray_Sort);
	}

	DECLARE_FUNCTION(execArray_RemoveIf)
	{
		PREDICATE_BODY(GenericArray_RemoveIf);
	}
};

