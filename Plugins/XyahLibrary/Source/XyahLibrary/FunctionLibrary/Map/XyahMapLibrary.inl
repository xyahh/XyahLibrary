// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

FORCEINLINE DEFINE_FUNCTION(UXyahMapLibrary::execBP_ReplaceKey)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FMapProperty>(NULL);
	void* MapAddr = Stack.MostRecentPropertyAddress;
	FMapProperty* MapProperty = CastField<FMapProperty>(Stack.MostRecentProperty);
	if (!MapProperty)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

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
