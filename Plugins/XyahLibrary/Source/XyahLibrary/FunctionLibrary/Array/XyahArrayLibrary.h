// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahLibraryCore.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Net/Core/PushModel/PushModel.h"
#include "UObject/UnrealType.h"
#include "XyahArrayLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahArrayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
//Blueprint Only
protected:

	/*
	Gets a Random Element from the Array

	@param Array - The Array to get a Random Element from
	@param OutRandomElement - the element gotten at random
	*/
	UFUNCTION(BlueprintPure, CustomThunk, Category = "XyahLibrary|Array", meta = (DisplayName = "Get Random", CompactNodeTitle="RANDOM", ArrayParm="Array", ArrayTypeDependentParams="OutRandomElement"))
	static void BP_GetRandom(const TArray<int32>& Array, int32& OutRandomElement);

	/*
	Sorts the given Array via a Predicate Function gotten via its Name and Function Owner.
	This Predicate Function will be called for every comparison. If the Function returns true, the swap will take place (i.e. out of order), else
	there will be no swap.
	
	@param Array - The Array to Sort.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally. 

	@return [bool] Whether Sorting actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (DisplayName = "Sort", ArrayParm = "Array", AdvancedDisplay="FunctionOwner"))
	static bool BP_Sort(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);


	/*
	Removes the elements from the given array that satisfy the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every element. If the Function returns true, the element will be removed.

	@param Array - The Array to Remove Elements from. 
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@return [bool] Whether RemoveIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (DisplayName = "Remove If", ArrayParm = "Array", AdvancedDisplay="FunctionOwner"))
	static bool BP_RemoveIf(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Finds the elements from the given array that satisfy the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every element. If the Function returns true, the element will be put into the OutArray of Found Elements.

	@param Array - The Array to Remove Elements from.
	@param OutArray - The Array of Found Elements.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@return [bool] Whether FindIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (DisplayName = "Find If", ArrayParm = "Array", ArrayTypeDependentParams = "OutArray", AdvancedDisplay = "FunctionOwner"))
	static bool BP_FindIf(const TArray<int32>& Array, TArray<int32>& OutArray, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Returns true if ANY given array element satisfies the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every element until one that returns true is found.

	@param Array - The Array to Remove Elements from.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@param bOutAnyIfReturn - The return value of the AnyIf process
	@return [bool] Whether AnyIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (DisplayName = "Any If", ArrayParm = "Array", AdvancedDisplay = "FunctionOwner"))
	static bool BP_AnyIf(const TArray<int32>& Array, bool& bOutAnyIfReturn, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Returns true if ALL given array elements satisfy the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every element. 

	@param Array - The Array to Remove Elements from.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@param bOutAllIfReturn - The return value of the AllIf process
	@return [bool] Whether AllIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (DisplayName = "All If", ArrayParm = "Array", AdvancedDisplay = "FunctionOwner"))
	static bool BP_AllIf(const TArray<int32>& Array, bool& bOutAllIfReturn, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);
	
//C++ & Blueprint
public:

	//Returns the Next Valid Index after CurrentIndex has been added to DeltaIndex (DeltaIndex can be negative)
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Array", meta = (ArrayParm = "InArray"))
	static int32 GetNextIndex(int32 CurrentIndex, int32 DeltaIndex, const TArray<int32>& InArray, bool bClampAtEnds = false);

//C++ Only
public: 

	template<typename ArrayElement>
	static ArrayElement GetRandom(const TArray<ArrayElement>& InArray);

	template<typename ArrayElement, typename Predicate>
	static bool AnyIf(const TArray<ArrayElement>& InArray, Predicate Pred);

	template<typename ArrayElement, typename Predicate>
	static bool AllIf(const TArray<ArrayElement>& InArray, Predicate Pred);

//Internal Funcs 
protected:

	static void Generic_GetRandom(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem);

	static bool Generic_Sort(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
		, FName PredicateFunctionName);

	static bool Generic_RemoveIf(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
		, FName PredicateFunctionName);

	static bool Generic_FindIf(void* TargetArray, void* OutTargetArray, const FArrayProperty* ArrayProp
		, const FArrayProperty* OutArrayProp, UObject* FuncOwner , FName PredicateFunctionName);

	static bool Generic_AllIf(void* TargetArray, const FArrayProperty* ArrayProp, bool& OutAllIfRetVal
		, UObject* FuncOwner, FName PredicateFunctionName);

	static bool Generic_AnyIf(void* TargetArray, const FArrayProperty* ArrayProp, bool& OutAnyIfRetVal
		, UObject* FuncOwner, FName PredicateFunctionName);

//ExecFuncs
public:

	DECLARE_FUNCTION(execBP_GetRandom);
	DECLARE_FUNCTION(execBP_Sort);
	DECLARE_FUNCTION(execBP_RemoveIf);
	DECLARE_FUNCTION(execBP_FindIf);
	DECLARE_FUNCTION(execBP_AnyIf);
	DECLARE_FUNCTION(execBP_AllIf);
};


#if XYAH_ARRAY_LIBRARY
#include "XyahArrayLibrary.inl"
#endif
