// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

//Exec Funcs
FORCEINLINE DEFINE_FUNCTION(UXyahArrayLibrary::execBP_GetRandom)
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

FORCEINLINE DEFINE_FUNCTION(UXyahArrayLibrary::execBP_Sort)
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

FORCEINLINE DEFINE_FUNCTION(UXyahArrayLibrary::execBP_RemoveIf)
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

FORCEINLINE DEFINE_FUNCTION(UXyahArrayLibrary::execBP_FindIf)
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

FORCEINLINE DEFINE_FUNCTION(UXyahArrayLibrary::execBP_AnyIf)
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

FORCEINLINE DEFINE_FUNCTION(UXyahArrayLibrary::execBP_AllIf)
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