// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

FORCEINLINE DEFINE_FUNCTION(UXyahMapLibrary::execBP_GetRandom)
{
	XYAH_GET_PROPERTY_AND_ADDRESS(FMapProperty, MapAddr, MapProperty);
	XYAH_CHECK_ARRAY_PROPERTY(MapProperty);

	//Retrieve OutKey
	const FProperty* KeyProp = MapProperty->KeyProp;
	const int32 KeyPropertySize = KeyProp->ElementSize * KeyProp->ArrayDim;

	void* KeyStorageSpace = FMemory_Alloca(KeyPropertySize);
	MapProperty->KeyProp->InitializeValue(KeyStorageSpace);
	Stack.MostRecentPropertyAddress = NULL;
	Stack.StepCompiledIn<FProperty>(KeyStorageSpace);
	void* KeyPtr = (Stack.MostRecentPropertyAddress != NULL && Stack.MostRecentProperty->GetClass() == MapProperty->KeyProp->GetClass()) ? Stack.MostRecentPropertyAddress : KeyStorageSpace;

	//Retrieve OutValue
	const FProperty* ValueProp = MapProperty->ValueProp;
	const int32 ValuePropertySize = ValueProp->ElementSize * ValueProp->ArrayDim;

	void* ValueStorageSpace = FMemory_Alloca(ValuePropertySize);
	MapProperty->ValueProp->InitializeValue(ValueStorageSpace);
	Stack.MostRecentPropertyAddress = NULL;
	Stack.StepCompiledIn<FProperty>(ValueStorageSpace);
	void* ValuePtr = (Stack.MostRecentPropertyAddress != NULL && Stack.MostRecentProperty->GetClass() == MapProperty->ValueProp->GetClass()) ? Stack.MostRecentPropertyAddress : ValueStorageSpace;


	P_FINISH;
	P_NATIVE_BEGIN;
	Generic_GetRandom(MapAddr, MapProperty, KeyPtr, ValuePtr);
	P_NATIVE_END;
	KeyProp->DestroyValue(KeyStorageSpace);
	ValueProp->DestroyValue(ValueStorageSpace);
}

FORCEINLINE DEFINE_FUNCTION(UXyahMapLibrary::execBP_ReplaceKey)
{
	XYAH_GET_PROPERTY_AND_ADDRESS(FMapProperty, MapAddr, MapProperty);
	XYAH_CHECK_ARRAY_PROPERTY(MapProperty);

	FProperty* OldKeyProperty = MapProperty->KeyProp;
	void* OldKeyData = FMemory_Alloca(OldKeyProperty->ElementSize * OldKeyProperty->ArrayDim);
	OldKeyProperty->InitializeValue(OldKeyData);

	Stack.MostRecentPropertyAddress = NULL;
	Stack.StepCompiledIn<FProperty>(OldKeyData);

	Stack.Step(Stack.Object, NULL);
	FProperty* NewKeyProperty = Stack.MostRecentProperty;
	void* NewKeyData = Stack.MostRecentPropertyAddress;

	P_FINISH;

	P_NATIVE_BEGIN;
	XYAH_RETURN(bool, Generic_ReplaceKey(MapAddr, MapProperty, OldKeyProperty, OldKeyData, NewKeyProperty, NewKeyData));
	XYAH_MARK_PROPERTY_DIRTY(MapProperty);
	P_NATIVE_END;

	OldKeyProperty->DestroyValue(OldKeyData);
}

FORCEINLINE DEFINE_FUNCTION(UXyahMapLibrary::execBP_RemoveIf)
{
	XYAH_GET_PROPERTY_AND_ADDRESS(FMapProperty, MapAddr, MapProperty);
	XYAH_CHECK_ARRAY_PROPERTY(MapProperty);

	P_GET_PROPERTY(FNameProperty, PredicateFunctionName);
	P_GET_PROPERTY(FObjectProperty, FunctionOwner);

	P_FINISH;
	P_NATIVE_BEGIN;

	XYAH_MARK_PROPERTY_DIRTY(ArrayProperty);
	XYAH_RETURN(bool, Generic_RemoveIf(MapAddr, MapProperty, (FunctionOwner) ? FunctionOwner : XYAH_GET_OBJECT, PredicateFunctionName));
	P_NATIVE_END;
}

FORCEINLINE DEFINE_FUNCTION(UXyahMapLibrary::execBP_FindIf)
{
	XYAH_GET_PROPERTY_AND_ADDRESS(FMapProperty, MapAddr, MapProperty);
	XYAH_CHECK_ARRAY_PROPERTY(MapProperty);

	XYAH_GET_PROPERTY_AND_ADDRESS(FArrayProperty, OutKeysAddr, OutKeysProperty);
	XYAH_CHECK_ARRAY_PROPERTY(OutKeysProperty);

	P_GET_PROPERTY(FNameProperty, PredicateFunctionName);
	P_GET_PROPERTY(FObjectProperty, FunctionOwner);

	P_FINISH;
	P_NATIVE_BEGIN;

	XYAH_MARK_PROPERTY_DIRTY(ArrayProperty);
	XYAH_RETURN(bool, Generic_FindIf(MapAddr, OutKeysAddr, MapProperty, OutKeysProperty, (FunctionOwner) ? FunctionOwner : XYAH_GET_OBJECT, PredicateFunctionName));
	P_NATIVE_END;
	
}