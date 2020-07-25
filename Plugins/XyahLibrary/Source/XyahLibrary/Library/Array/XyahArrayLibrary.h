// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../XyahLibrary.h"
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
	UFUNCTION(BlueprintPure, CustomThunk, Category = "XyahLibrary|Array", meta = (CompactNodeTitle="RANDOM", ArrayParm="Array", ArrayTypeDependentParams="OutRandomElement"))
	static void GetRandom(const TArray<int32>& Array, int32& OutRandomElement);

	/*
	Sorts the given Array via a Predicate Function gotten via its Name and Function Owner.
	This Predicate Function will be called for every comparison. If the Function returns true, the swap will take place (i.e. out of order), else
	there will be no swap.
	
	@param Array - The Array to Sort.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally. 

	@return [bool] Whether Sorting actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (ArrayParm = "Array", AdvancedDisplay="FunctionOwner"))
	static bool Sort(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);


	/*
	Removes the elements from the given array that satisfy the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every element. If the Function returns true, the element will be removed.

	@param Array - The Array to Remove Elements from. 
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@return [bool] Whether RemoveIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (ArrayParm = "Array", AdvancedDisplay="FunctionOwner"))
	static bool RemoveIf(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Finds the elements from the given array that satisfy the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every element. If the Function returns true, the element will be put into the OutArray of Found Elements.

	@param Array - The Array to Remove Elements from.
	@param OutArray - The Array of Found Elements.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@return [bool] Whether FindIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (ArrayParm = "Array", ArrayTypeDependentParams = "OutArray", AdvancedDisplay = "FunctionOwner"))
	static bool FindIf(const TArray<int32>& Array, TArray<int32>& OutArray, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Returns true if ANY given array element satisfies the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every element until one that returns true is found.

	@param Array - The Array to Remove Elements from.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@param bOutAnyIfReturn - The return value of the AnyIf process
	@return [bool] Whether AnyIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (ArrayParm = "Array", AdvancedDisplay = "FunctionOwner"))
	static bool AnyIf(const TArray<int32>& Array, bool& bOutAnyIfReturn, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Returns true if ALL given array elements satisfy the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every element. 

	@param Array - The Array to Remove Elements from.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@param bOutAllIfReturn - The return value of the AllIf process
	@return [bool] Whether AllIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Array", meta = (ArrayParm = "Array", AdvancedDisplay = "FunctionOwner"))
	static bool AllIf(const TArray<int32>& Array, bool& bOutAllIfReturn, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

//Blueprint & C++
public:

	
//C++ Only
public:

//Internal Funcs 
protected:

	static void Generic_GetRandom(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem);

	static bool Generic_Sort(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
		, FName PredicateFunctionName);

	static bool Generic_RemoveIf(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
		, FName PredicateFunctionName);

	static bool Generic_FindIf(void* TargetArray, void* OutTargetArray, const FArrayProperty* ArrayProp, const FArrayProperty* OutArrayProp, UObject* FuncOwner
		, FName PredicateFunctionName);

	static bool Generic_AllIf(void* TargetArray, const FArrayProperty* ArrayProp, bool& OutAllIfRetVal, UObject* FuncOwner
		, FName PredicateFunctionName);

	static bool Generic_AnyIf(void* TargetArray, const FArrayProperty* ArrayProp, bool& OutAnyIfRetVal, UObject* FuncOwner
		, FName PredicateFunctionName);

//ExecFuncs
public:

	DECLARE_FUNCTION(execGetRandom)
	{
		XYAH_GET_PROPERTY_AND_ADDRESS(FArrayProperty, ArrayAddr, ArrayProperty);
		XYAH_CHECK_ARRAY_PROPERTY(ArrayProperty);

		const FProperty* InnerProp = ArrayProperty->Inner;
		const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;

		void* StorageSpace = FMemory_Alloca(PropertySize);
		InnerProp->InitializeValue(StorageSpace);
		Stack.MostRecentPropertyAddress = NULL;
		Stack.StepCompiledIn<FProperty>(StorageSpace);
		void* ItemPtr = (Stack.MostRecentPropertyAddress != NULL && Stack.MostRecentProperty->GetClass() == InnerProp->GetClass()) ? Stack.MostRecentPropertyAddress : StorageSpace;

		P_FINISH;
		P_NATIVE_BEGIN;
		Generic_GetRandom(ArrayAddr, ArrayProperty, ItemPtr);
		P_NATIVE_END;
		InnerProp->DestroyValue(StorageSpace);
	}

	DECLARE_FUNCTION(execSort)
	{
		XYAH_GET_PROPERTY_AND_ADDRESS(FArrayProperty, ArrayAddr, ArrayProperty);
		XYAH_CHECK_ARRAY_PROPERTY(ArrayProperty);

		P_GET_PROPERTY(FNameProperty, PredicateFunctionName); 
		P_GET_PROPERTY(FObjectProperty, FunctionOwner);

		P_FINISH;
		P_NATIVE_BEGIN;

		XYAH_MARK_PROPERTY_DIRTY(ArrayProperty);
		XYAH_RETURN(bool, Generic_Sort(ArrayAddr, ArrayProperty, (FunctionOwner) ? FunctionOwner : XYAH_GET_OBJECT, PredicateFunctionName));
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execRemoveIf)
	{
		XYAH_GET_PROPERTY_AND_ADDRESS(FArrayProperty, ArrayAddr, ArrayProperty);
		XYAH_CHECK_ARRAY_PROPERTY(ArrayProperty);

		P_GET_PROPERTY(FNameProperty, PredicateFunctionName);
		P_GET_PROPERTY(FObjectProperty, FunctionOwner);

		P_FINISH;
		P_NATIVE_BEGIN;

		XYAH_MARK_PROPERTY_DIRTY(ArrayProperty);
		XYAH_RETURN(bool, Generic_RemoveIf(ArrayAddr, ArrayProperty, (FunctionOwner) ? FunctionOwner : XYAH_GET_OBJECT, PredicateFunctionName));
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execFindIf)
	{
		XYAH_GET_PROPERTY_AND_ADDRESS(FArrayProperty, ArrayAddr, ArrayProperty);
		XYAH_CHECK_ARRAY_PROPERTY(ArrayProperty);

		XYAH_GET_PROPERTY_AND_ADDRESS(FArrayProperty, OutArrayAddr, OutArrayProperty);
		XYAH_CHECK_ARRAY_PROPERTY(OutArrayProperty);

		P_GET_PROPERTY(FNameProperty, PredicateFunctionName);
		P_GET_PROPERTY(FObjectProperty, FunctionOwner);

		P_FINISH;
		P_NATIVE_BEGIN;

		//XYAH_MARK_PROPERTY_DIRTY(ArrayProperty);
		XYAH_MARK_PROPERTY_DIRTY(OutArrayProperty);

		XYAH_RETURN(bool, Generic_FindIf(ArrayAddr, OutArrayAddr, ArrayProperty, OutArrayProperty, (FunctionOwner) ? FunctionOwner : XYAH_GET_OBJECT, PredicateFunctionName));
		P_NATIVE_END;
	}
	
	DECLARE_FUNCTION(execAnyIf)
	{
		XYAH_GET_PROPERTY_AND_ADDRESS(FArrayProperty, ArrayAddr, ArrayProperty);
		XYAH_CHECK_ARRAY_PROPERTY(ArrayProperty);

		P_GET_PROPERTY_REF(FBoolProperty, OutRetVal);
		P_GET_PROPERTY(FNameProperty, PredicateFunctionName);
		P_GET_PROPERTY(FObjectProperty, FunctionOwner);

		P_FINISH;
		P_NATIVE_BEGIN;
		XYAH_RETURN(bool, Generic_AnyIf(ArrayAddr, ArrayProperty, OutRetVal, (FunctionOwner) ? FunctionOwner : XYAH_GET_OBJECT, PredicateFunctionName));
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execAllIf)
	{
		XYAH_GET_PROPERTY_AND_ADDRESS(FArrayProperty, ArrayAddr, ArrayProperty);
		XYAH_CHECK_ARRAY_PROPERTY(ArrayProperty);

		P_GET_PROPERTY_REF(FBoolProperty, OutRetVal);
		P_GET_PROPERTY(FNameProperty, PredicateFunctionName);
		P_GET_PROPERTY(FObjectProperty, FunctionOwner);

		P_FINISH;
		P_NATIVE_BEGIN;
		XYAH_RETURN(bool, Generic_AllIf(ArrayAddr, ArrayProperty, OutRetVal, (FunctionOwner) ? FunctionOwner : XYAH_GET_OBJECT, PredicateFunctionName));
		P_NATIVE_END;
	}
};

