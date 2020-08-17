// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.


#include "XyahMapLibrary.h"
#include "Utility/XyahUtilityLibrary.h"

bool UXyahMapLibrary::BP_ReplaceKey(const TMap<int32, int32>& InMap, const int32& OldKey, const int32& NewKey)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
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
