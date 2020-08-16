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
	Replaces a Given Key from a Given Map while keeping its Values.

	@param InMap - The Map to have one of its keys replaced
	@param OldKey - the current key to replace
	@param NewKey - the new key to replace the current key with (must match the OldKey type to take effect)
	@note There's currently a limitation with how UFUNCTION MapParams specifiers work, preventing for multiple Map Keys/Values to be resolved. 
	@see XyahMapLibraryNotes.txt
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Map", meta=(DisplayName = "Replace Map Key", CompactNodeTitle = "REPLACE", MapParam = "InMap", MapKeyParam = "OldKey", CustomStructureParam = "NewKey", Keywords = "switch change swap", AutoCreateRefTerm = "OldKey"))
	static bool BP_ReplaceKey(const TMap<int32, int32>& InMap, const int32& OldKey, const int32& NewKey);

//Internal Funcs 
private:

	static bool Generic_ReplaceKey(const void* TargetMap, const FMapProperty* MapProperty
		, FProperty* OldKeyProp, const void* OldKeyPtr, FProperty* NewKeyProp, const void* NewKeyPtr);

//Exec Funcs
public:

	DECLARE_FUNCTION(execBP_ReplaceKey);
};

#if XYAH_MAP_LIBRARY
#include "XyahMapLibrary.inl"
#endif