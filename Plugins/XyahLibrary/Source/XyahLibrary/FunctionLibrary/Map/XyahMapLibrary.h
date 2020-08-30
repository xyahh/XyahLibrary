// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahLibraryCore.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XyahMapLibrary.generated.h"

#define XYAH_MAP_LIBRARY 1

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
	Gets a Copy of the Pair at the Logical Index in the Given Map

	@param Map - The Map to get the Pair from
	@param Index - The Index of the Map to get the pair from (this is the "logical" index, not the physical index as the physical might be empty)
	@param OutKey - the Key from the Pair gotten
	@param OutValue - the Value from the Pair gotten
	*/
	UFUNCTION(BlueprintPure, CustomThunk, Category = "XyahLibrary|Map", meta = (DisplayName = "Get", CompactNodeTitle = "GET", MapParam = "Map", MapKeyParam = "OutKey", MapValueParam = "OutValue"))
	static void BP_GetFromIndex(const TMap<int32, int32>& Map, int32 Index, int32& OutKey, int32& OutValue);

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
	[INPUT] Map Key Type
	[INPUT] Map Value Type
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
	[INPUT] Map Key Type
	[INPUT] Map Value Type
	[OUTPUT] Bool Type

	@param InMap - The Map to Find Pairs from.
	@param OutKeys - The Array of Keys that satisfied the Predicate.
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@return [bool] Whether FindIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Map", meta = (DisplayName = "Find If", MapParam = "InMap", MapKeyParam = "OutKeys", AdvancedDisplay = "FunctionOwner"))
	static bool BP_FindIf(const TMap<int32, int32>& InMap, TArray<int32>& OutKeys, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Returns true if ALL given map pairs satisfy the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every pair in the map. 

	The format of the Function should be as follows:
	[INPUT] Map Key Type
	[INPUT] Map Value Type
	[OUTPUT] Bool Type

	@param InMap - The Map to check
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@param bEvaluation - The return value of the AllIf process
	@return [bool] Whether AllIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Map", meta = (DisplayName = "All If", ReturnDisplayName = "Valid Predicate", MapParam = "InMap", AdvancedDisplay = "FunctionOwner"))
	static bool BP_AllIf(const TMap<int32, int32>& InMap, bool& bEvaluation, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Returns true if any pair in the given map satisfies the Predicate Function set via its Name and Function Owner.
	This Predicate Function will be called for every pair in the map until a pair that returns true is found.

	The format of the Function should be as follows:
	[INPUT] Map Key Type
	[INPUT] Map Value Type
	[OUTPUT] Bool Type

	@param InMap - The Map to check
	@param PredicateFunctionName - The Name of the Function to call. This Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@param bEvaluation - The return value of the AnyIf process
	@return [bool] Whether AnyIf actually took place or there was an error processing it.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Map", meta = (DisplayName = "Any If", ReturnDisplayName = "Valid Predicate", MapParam = "InMap", AdvancedDisplay = "FunctionOwner"))
	static bool BP_AnyIf(const TMap<int32, int32>& InMap, bool& bEvaluation, FName PredicateFunctionName, UObject* FunctionOwner = nullptr);

	/*
	Adds the Key and Value to the Map if the Key is not in present in the map
	@return [bool] Whether the Addition of the given Pair actually took place
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Map", meta = (DisplayName = "Add New", CompactNodeTitle = "ADD NEW", MapParam = "InMap", MapKeyParam = "Key", MapValueParam = "Value", Keywords = "insert safe check", AutoCreateRefTerm = "Key,Value"))
	static bool BP_AddNew(const TMap<int32, int32>& InMap, const int32& Key, const int32& Value);

//C++ & Blueprints
public:

	//Returns the Next Valid Index after CurrentIndex has been added to DeltaIndex (DeltaIndex can be negative)
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Map", meta = (MapParam = "InMap"))
	static int32 GetNextIndex(int32 CurrentIndex, int32 DeltaIndex, const TMap<int32, int32>& InMap, bool bClampAtEnds = false);

//C++ Only
public:

	//Gets a random pair of Key/Value from the Given Map
	template<typename KeyType, typename ValueType>
	static bool GetRandom(const TMap<KeyType, ValueType>& InMap, KeyType* OutKey, ValueType* OutValue = nullptr);

	//Gets the pair of Key/Value at a Logical Index from the Given Map
	template<typename KeyType, typename ValueType>
	static bool GetFromIndex(const TMap<KeyType, ValueType>& InMap, int32 Index, KeyType* OutKey, ValueType* OutValue = nullptr);

	//Replaces the Key from the given map while retaining its Value. The New Key must not be in the Map!
	template<typename KeyType, typename ValueType>
	static bool ReplaceKey(TMap<KeyType, ValueType>& InMap, const KeyType& OldKey, const KeyType& NewKey);

	/*
	Removes all Pairs that Satisfy the Predicate Function.

	The format of the Predicate Function should be as follows:
	[INPUT] Map Key Type, Map Value Type
	[OUTPUT] Bool Type
	*/
	template<typename KeyType, typename ValueType, typename PredicateFunc>
	static void RemoveIf(TMap<KeyType, ValueType>& InMap, PredicateFunc Predicate);

	/*
	Finds all Pairs that Satisfy the Predicate Function.

	The format of the Predicate Function should be as follows:
	[INPUT] Map Key Type, Map Value Type
	[OUTPUT] Bool Type
	*/
	template<typename KeyType, typename ValueType, typename PredicateFunc>
	static void FindIf(TMap<KeyType, ValueType>& InMap, TArray<KeyType>& OutKeys, PredicateFunc Predicate);

	//Adds a Pair to the Map if it the Key is not already in the map
	template<typename KeyType, typename ValueType>
	static bool SafeAdd(TMap<KeyType, ValueType>& InMap, const KeyType& OutKey, const ValueType& OutValue);

//Internal Funcs 
private:

	static void Generic_GetRandom(void* TargetMap, const FMapProperty* MapProp, void* OutKey, void* OutValue);

	static void Generic_GetFromIndex(void* TargetMap, const FMapProperty* MapProp, int32 Index, void* OutKey, void* OutValue);

	static bool Generic_ReplaceKey(const void* TargetMap, const FMapProperty* MapProperty
		, FProperty* OldKeyProp, const void* OldKeyPtr, FProperty* NewKeyProp, const void* NewKeyPtr);

	static bool Generic_RemoveIf(void* TargetMap, const FMapProperty* MapProp, UObject* FuncOwner
		, FName PredicateFunctionName);

	static bool Generic_FindIf(void* TargetMap, void* OutKeysArray, const FMapProperty* TargetMapProp
		, const FArrayProperty* OutKeysArrayProp, UObject* FuncOwner, FName PredicateFunctionName);

	static bool Generic_AllIf(void* TargetMap, const FMapProperty* MapProp, bool& OutAllIfRetVal
		, UObject* FuncOwner, FName PredicateFunctionName);

	static bool Generic_AnyIf(void* TargetMap, const FMapProperty* MapProp, bool& OutAnyIfRetVal
		, UObject* FuncOwner, FName PredicateFunctionName);

	static bool Generic_AddNew(void* TargetMap, const FMapProperty* MapProp, const void* Key, const void* Value);
	
//Exec Funcs
public:

	DECLARE_FUNCTION(execBP_GetRandom);
	DECLARE_FUNCTION(execBP_GetFromIndex);
	DECLARE_FUNCTION(execBP_ReplaceKey);
	DECLARE_FUNCTION(execBP_RemoveIf);
	DECLARE_FUNCTION(execBP_FindIf);
	DECLARE_FUNCTION(execBP_AnyIf);
	DECLARE_FUNCTION(execBP_AllIf);
	DECLARE_FUNCTION(execBP_AddNew);
};


#if XYAH_MAP_LIBRARY
#include "XyahMapLibrary.inl"
#endif