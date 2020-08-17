// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahLibraryCore.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XyahMapLibrary.generated.h"

/**
 *
 */
UCLASS()
class XYAHLIBRARY_API UXyahMapLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//Blueprint Only	
private:

	/*
	Gets a Random Pair from the Map

	@param Map - The Map to get a Random Pair from
	@param OutKey - the Key from the Pair gotten at Random
	@param OutValue - the Value from the Pair gotten at Random
	*/
	UFUNCTION(BlueprintPure, CustomThunk, Category = "XyahLibrary|Map", meta = (DisplayName = "Get Random", CompactNodeTitle = "RANDOM", MapParam = "Map", MapKeyParam = "OutKey", MapValueParam = "OutValue"))
	static void BP_GetRandom(const TMap<int32, int32>& Map, int32& OutKey, int32& OutValue);

	/*
	Replaces a Given Key from a Given Map while keeping its Values.

	@param InMap - The Map to have one of its keys replaced
	@param OldKey - the current key to replace
	@param NewKey - the new key to replace the current key with (must match the OldKey type to take effect)
	@note There's currently a limitation with how UFUNCTION MapParams specifiers work, preventing for multiple Map Keys/Values to be resolved. 
	@see XyahMapLibraryNotes.txt
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Map", meta=(DisplayName = "Replace Map Key", CompactNodeTitle = "REPLACE", MapParam = "InMap", MapKeyParam = "OldKey", CustomStructureParam = "NewKey", Keywords = "switch change swap", AutoCreateRefTerm = "OldKey"))
	static bool BP_ReplaceKey(const TMap<int32, int32>& InMap, const int32& OldKey, const int32& NewKey);

	/*
	Removes the pairs from the given map that satisfy the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every pair (Key, Value). If the Function returns true, the element will be removed.

	The format of the Function should be as follows:
	[INPUT] Map Key Type, Map Value Type
	[OUTPUT] Bool Type

	@param InMap - The Map to Remove Elements from.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@return [bool] Whether RemoveIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Map", meta = (DisplayName = "Remove If", MapParam = "InMap", AdvancedDisplay = "FunctionOwner"))
	static bool BP_RemoveIf(const TMap<int32, int32>& InMap, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Finds the pairs from the given map that satisfy the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every pair. If the Function returns true, the pair's Key will be put into the OutArray of Keys.

	The format of the Function should be as follows:
	[INPUT] Map Key Type, Map Value Type
	[OUTPUT] Bool Type

	@param InMap - The Map to Find Pairs from.
	@param OutKeys - The Array of Keys that satisfied the Predicate.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@return [bool] Whether FindIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Map", meta = (DisplayName = "Find If", MapParam = "InMap", MapKeyParam = "OutKeys", AdvancedDisplay = "FunctionOwner"))
	static bool BP_FindIf(const TMap<int32, int32>& InMap, TArray<int32>& OutKeys, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);


//Internal Funcs 
private:

	static void Generic_GetRandom(void* TargetMap, const FMapProperty* MapProp, void* OutKey, void* OutValue);

	static bool Generic_ReplaceKey(const void* TargetMap, const FMapProperty* MapProperty
		, FProperty* OldKeyProp, const void* OldKeyPtr, FProperty* NewKeyProp, const void* NewKeyPtr);

	static bool Generic_RemoveIf(void* TargetMap, const FMapProperty* MapProp, UObject* FuncOwner
		, FName PredicateFunctionName);

	static bool Generic_FindIf(void* TargetMap, void* OutKeysArray, const FMapProperty* TargetMapProp
		, const FArrayProperty* OutKeysArrayProp, UObject* FuncOwner, FName PredicateFunctionName);
	
//Exec Funcs
public:

	DECLARE_FUNCTION(execBP_GetRandom);
	DECLARE_FUNCTION(execBP_ReplaceKey);
	DECLARE_FUNCTION(execBP_RemoveIf);
	DECLARE_FUNCTION(execBP_FindIf);
};

#if XYAH_MAP_LIBRARY
#include "XyahMapLibrary.inl"
#endif