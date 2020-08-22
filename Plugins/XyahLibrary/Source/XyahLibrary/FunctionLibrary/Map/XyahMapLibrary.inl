// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

template<typename KeyType, typename ValueType>
bool UXyahMapLibrary::GetRandom(const TMap<KeyType, ValueType>& InMap, KeyType* OutKey, ValueType* OutValue /*= nullptr*/)
{
	if (InMap.Num() > 0 && (OutKey||OutValue))
	{
		const int32 RandomCount = FMath::Rand() % InMap.Num();
		return GetFromIndex(InMap, RandomCount, OutKey, OutValue);
		
	}
	return false;
}

template<typename KeyType, typename ValueType>
bool UXyahMapLibrary::GetFromIndex(const TMap<KeyType, ValueType>& InMap, int32 Index, KeyType* OutKey, ValueType* OutValue /*= nullptr*/)
{
	if (InMap.Num() > 0 && (OutKey || OutValue) && Index >= 0 && Index < InMap.Num())
	{
		auto MapIter = InMap.CreateConstIterator();

		for (int32 i = 0; i < Index; ++i)
			++MapIter;

		if (OutKey)
			*OutKey = MapIter->Key;

		if (OutValue)
			*OutValue = MapIter->Value;

		return true;

	}
	return false;
}

template<typename KeyType, typename ValueType>
bool UXyahMapLibrary::ReplaceKey(TMap<KeyType, ValueType>& InMap, const KeyType& OldKey, const KeyType& NewKey)
{
	if (!InMap.Contains(NewKey))
	{
		ValueType Value;
		if (InMap.RemoveAndCopyValue(OldKey, Value))
		{
			InMap.Add(NewKey, MoveTempIfPossible(Value));
			return true;
		}
	}
	return false;
}

template<typename KeyType, typename ValueType, typename PredicateFunc>
void UXyahMapLibrary::RemoveIf(TMap<KeyType, ValueType>& InMap, PredicateFunc Predicate)
{
	for (auto MapIter = InMap.CreateIterator(); MapIter; ++MapIter)
	{
		if (Predicate(MapIter->Key, MapIter->Value))
			MapIter.RemoveCurrent();
	}
}

template<typename KeyType, typename ValueType, typename PredicateFunc>
void UXyahMapLibrary::FindIf(TMap<KeyType, ValueType>& InMap, TArray<KeyType>& OutKeys, PredicateFunc Predicate)
{
	OutKeys.Empty();
	for (auto MapIter = InMap.CreateIterator(); MapIter; ++MapIter)
	{
		if (Predicate(MapIter->Key, MapIter->Value))
			OutKeys.Emplace(MapIter->Key);
	}
}

template<typename KeyType, typename ValueType>
bool UXyahMapLibrary::SafeAdd(TMap<KeyType, ValueType>& InMap, const KeyType& Key, const ValueType& Value)
{
	if (!InMap.Contains(Key))
	{
		InMap.Emplace(Key, Value);
		return true;
	}
	return false;
}

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

FORCEINLINE DEFINE_FUNCTION(UXyahMapLibrary::execBP_GetFromIndex)
{
	XYAH_GET_PROPERTY_AND_ADDRESS(FMapProperty, MapAddr, MapProperty);
	XYAH_CHECK_ARRAY_PROPERTY(MapProperty);

	//Retrieve Index
	PARAM_PASSED_BY_VAL(Index, FIntProperty, int32);

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
	Generic_GetFromIndex(MapAddr, MapProperty, Index, KeyPtr, ValuePtr);
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

FORCEINLINE DEFINE_FUNCTION(UXyahMapLibrary::execBP_SafeAdd)
{
	XYAH_GET_PROPERTY_AND_ADDRESS(FMapProperty, MapAddr, MapProperty);
	XYAH_CHECK_ARRAY_PROPERTY(MapProperty);

	Stack.Step(Stack.Object, NULL);
	void* KeyData = Stack.MostRecentPropertyAddress;

	Stack.Step(Stack.Object, NULL);
	void* ValueData = Stack.MostRecentPropertyAddress;

	P_FINISH;

	P_NATIVE_BEGIN;
	XYAH_RETURN(bool, Generic_SafeAdd(MapAddr, MapProperty, KeyData, ValueData));
	XYAH_MARK_PROPERTY_DIRTY(MapProperty);
	P_NATIVE_END;
}
