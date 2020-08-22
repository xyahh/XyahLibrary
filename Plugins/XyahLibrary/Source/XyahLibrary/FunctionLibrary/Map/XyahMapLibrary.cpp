// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#include "XyahMapLibrary.h"
#include "XyahUtilityLibrary.h"

void UXyahMapLibrary::BP_GetRandom(const TMap<int32, int32>& Map, int32& OutKey, int32& OutValue)
{
	XYAH_SHOULD_NEVER_HIT_THIS();
}

void UXyahMapLibrary::BP_GetFromIndex(const TMap<int32, int32>& Map, int32 Index, int32& OutKey, int32& OutValue)
{
	XYAH_SHOULD_NEVER_HIT_THIS();
}

bool UXyahMapLibrary::BP_ReplaceKey(const TMap<int32, int32>& InMap, const int32& OldKey, const int32& NewKey)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

bool UXyahMapLibrary::BP_RemoveIf(const TMap<int32, int32>& InMap, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

bool UXyahMapLibrary::BP_FindIf(const TMap<int32, int32>& InMap, TArray<int32>& OutKeys, FName PredicateFunctionName
	, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

bool UXyahMapLibrary::BP_SafeAdd(const TMap<int32, int32>& InMap, const int32& Key, const int32& Value)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

int32 UXyahMapLibrary::GetNextIndex(int32 CurrentIndex, int32 DeltaIndex, const TMap<int32, int32>& InMap, bool bClampAtEnds /*= false*/)
{
	const int32 Sum = CurrentIndex + DeltaIndex;
	const int32 Result = Sum % InMap.Num();

	if (bClampAtEnds)
	{
		if (Sum >= InMap.Num())
			return InMap.Num() - 1;
		else if (Sum < 0)
			return 0;
	}

	if (Result < 0)
		return InMap.Num() + Result;
	return Result;
}

void UXyahMapLibrary::Generic_GetRandom(void* TargetMap, const FMapProperty* MapProp, void* OutKey, void* OutValue)
{
	if (TargetMap)
	{
		FScriptMapHelper MapHelper(MapProp, TargetMap);
		if (MapHelper.Num() > 0)
		{
			const int32 RandIndex = FMath::RandHelper(MapHelper.Num());
			MapHelper.KeyProp->CopyCompleteValueFromScriptVM(OutKey, MapHelper.GetKeyPtr(MapHelper.FindInternalIndex(RandIndex)));
			MapHelper.ValueProp->CopyCompleteValueFromScriptVM(OutValue, MapHelper.GetValuePtr(MapHelper.FindInternalIndex(RandIndex)));
		}
	}
}

void UXyahMapLibrary::Generic_GetFromIndex(void* TargetMap, const FMapProperty* MapProp, int32 Index, void* OutKey, void* OutValue)
{
	if (TargetMap)
	{
		FScriptMapHelper MapHelper(MapProp, TargetMap);
		if (MapHelper.Num() > 0 && Index >= 0 && Index < MapHelper.Num())
		{
			MapHelper.KeyProp->CopyCompleteValueFromScriptVM(OutKey, MapHelper.GetKeyPtr(MapHelper.FindInternalIndex(Index)));
			MapHelper.ValueProp->CopyCompleteValueFromScriptVM(OutValue, MapHelper.GetValuePtr(MapHelper.FindInternalIndex(Index)));
		}
	}
}

bool UXyahMapLibrary::Generic_ReplaceKey(const void* TargetMap, const FMapProperty* MapProperty
	, FProperty* OldKeyProp, const void* OldKeyPtr, FProperty* NewKeyProp, const void* NewKeyPtr)
{
	if (TargetMap)
	{
		//Make sure that the Properties are of the Same Time
		if (XYAH(Utility) CheckProperties(NewKeyProp, OldKeyProp))
		{
			FScriptMapHelper MapHelper(MapProperty, TargetMap);

			//Check if the NewKey is already in the Map. 
			if (MapHelper.FindValueFromHash(NewKeyPtr))
			{
				XYAH_LIB_LOG(Log, TEXT("Map ReplaceKey Error! Trying to Replace a Key with a Key that already exists in the Map!"));
				return false;
			}

			if (uint8* MapPair = MapHelper.FindMapPairPtrFromHash(OldKeyPtr))
			{
				int32 FoundIndex = MapHelper.FindMapIndexWithKey(MapPair);

				//"Rename" the OldKey to the NewKey and Rehash.
				if (FoundIndex != INDEX_NONE)
				{
					//MapHelper.AddPair(NewKeyPtr, MapHelper.GetValuePtr(FoundIndex));
					OldKeyProp->CopyCompleteValueToScriptVM(MapHelper.GetKeyPtr(FoundIndex), NewKeyPtr);
					MapHelper.Rehash();
					return true;
				}
			}
			else
			{
				XYAH_LIB_LOG(Log, TEXT("Map Error! ReplaceKey Failed. OldKey was not an element in the Map."));
			}
			
		}
		else
		{
			XYAH_LIB_LOG(Warning, TEXT("Map Error! ReplaceKey Failed. NewKey and OldKey properties do not match."));
		}
	}
	return false;
}

bool UXyahMapLibrary::Generic_RemoveIf(void* TargetMap, const FMapProperty* MapProp, UObject* FuncOwner, FName PredicateFunctionName)
{
	if (TargetMap && IsValid(FuncOwner))
	{
		FScriptMapHelper MapHelper(MapProp, TargetMap);

		if (MapHelper.Num() > 0)
		{
			if (UFunction* Predicate = XYAH(Utility) ValidateFunction(FuncOwner, PredicateFunctionName, TEXT("RemoveIf Failed")
				, { MapHelper.KeyProp, MapHelper.ValueProp }, { FBoolProperty::StaticClass() }))
			{
				//Allocations Done Here
				uint8* Params = (uint8*)FMemory_Alloca(Predicate->ParmsSize);

				//RemoveIf
				for (int32 i = 0; i < MapHelper.Num(); ++i)
				{
					FMemory::Memzero(Params, Predicate->ParmsSize);
					int32 ParamsProcessed = 0;
					uint8* ReturnParam = 0;

					for (TFieldIterator<FProperty> It(Predicate); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It)
					{
						switch (ParamsProcessed)
						{
						case 0: It->CopyCompleteValueFromScriptVM(It->ContainerPtrToValuePtr<uint8>(Params), MapHelper.GetKeyPtr(MapHelper.FindInternalIndex(i))); break;
						case 1: It->CopyCompleteValueFromScriptVM(It->ContainerPtrToValuePtr<uint8>(Params), MapHelper.GetValuePtr(MapHelper.FindInternalIndex(i))); break;
						default: ReturnParam = It->ContainerPtrToValuePtr<uint8>(Params); break;
						}

						++ParamsProcessed;
						if (ParamsProcessed >= 3)
							break;
					}
					FuncOwner->ProcessEvent(Predicate, Params);
					if (bool* ReturnBool = (bool*)ReturnParam)
					{
						if (*ReturnBool)
						{
							MapHelper.RemoveAt(MapHelper.FindInternalIndex(i));
							--i;
						}
					}
				}

				//Destroy Allocations
				for (TFieldIterator<FProperty> It(Predicate); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It)
				{
					It->DestroyValue_InContainer(Params);
				}
				return true;
			}


		}
		else
		{
			XYAH_LIB_LOG(Log, TEXT("RemoveIf did not take place! Map Pair count is 0!"));
		}
	}
	return false;
}

bool UXyahMapLibrary::Generic_FindIf(void* TargetMap, void* OutKeysArray, const FMapProperty* TargetMapProp
	, const FArrayProperty* OutKeysArrayProp, UObject* FuncOwner, FName PredicateFunctionName)
{
	if (TargetMap && OutKeysArray && IsValid(FuncOwner))
	{
		FScriptMapHelper MapHelper(TargetMapProp, TargetMap);
		FScriptArrayHelper OutKeysHelper(OutKeysArrayProp, OutKeysArray);

		if (MapHelper.Num() > 0)
		{
			if (UFunction* Predicate = XYAH(Utility) ValidateFunction(FuncOwner, PredicateFunctionName, TEXT("FindIf Failed")
				, { MapHelper.KeyProp, MapHelper.ValueProp }, { FBoolProperty::StaticClass() }))
			{
				//Allocations Done Here
				uint8* Params = (uint8*)FMemory_Alloca(Predicate->ParmsSize);

				//FindIf
				for (int32 i = 0; i < MapHelper.Num(); ++i)
				{
					FMemory::Memzero(Params, Predicate->ParmsSize);
					int32 ParamsProcessed = 0;
					uint8* ReturnParam = 0;

					for (TFieldIterator<FProperty> It(Predicate); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It)
					{
						switch (ParamsProcessed)
						{
						case 0: It->CopyCompleteValueFromScriptVM(It->ContainerPtrToValuePtr<uint8>(Params), MapHelper.GetKeyPtr(MapHelper.FindInternalIndex(i))); break;
						case 1: It->CopyCompleteValueFromScriptVM(It->ContainerPtrToValuePtr<uint8>(Params), MapHelper.GetValuePtr(MapHelper.FindInternalIndex(i))); break;
						default: ReturnParam = It->ContainerPtrToValuePtr<uint8>(Params); break;
						}

						++ParamsProcessed;
						if (ParamsProcessed >= 3)
							break;
					}
					FuncOwner->ProcessEvent(Predicate, Params);
					if (bool* ReturnBool = (bool*)ReturnParam)
					{
						if (*ReturnBool)
						{
							int32 Index = OutKeysHelper.AddValue();
							MapHelper.KeyProp->CopyCompleteValueFromScriptVM(OutKeysHelper.GetRawPtr(Index), MapHelper.GetKeyPtr(MapHelper.FindInternalIndex(i)));
						}
					}
				}

				//Destroy Allocations
				for (TFieldIterator<FProperty> It(Predicate); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It)
				{
					It->DestroyValue_InContainer(Params);
				}
				return true;
			}


		}
		else
		{
			XYAH_LIB_LOG(Log, TEXT("FindIf did not take place! Map Pair count is 0!"));
		}
	}
	return false;
}

bool UXyahMapLibrary::Generic_SafeAdd(void* TargetMap, const FMapProperty* MapProp, const void* Key, const void* Value)
{
	if (TargetMap &&  Key && Value)
	{
		FScriptMapHelper MapHelper(MapProp, TargetMap);
		if (!MapHelper.FindValueFromHash(Key))
		{
			MapHelper.AddPair(Key, Value);
			return true;
		}
	}
	return false;
}
