// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

FORCEINLINE DEFINE_FUNCTION(UXyahUtilityLibrary::execGetWildcard)
{
	Stack.Step(Stack.Object, NULL);
	FProperty* Property = Stack.MostRecentProperty;

	PARAM_PASSED_BY_REF(Name, FStrProperty, FString);
	PARAM_PASSED_BY_REF(Type, FStrProperty, FString);

	P_FINISH;
	P_NATIVE_BEGIN;
	Generic_GetWildcardType(Property, Name, Type);
	P_NATIVE_END;
}

FORCEINLINE DEFINE_FUNCTION(UXyahUtilityLibrary::execToString)
{
	Stack.Step(Stack.Object, NULL);
	FProperty* InProperty = Stack.MostRecentProperty;
	void* Data = Stack.MostRecentPropertyAddress;

	PARAM_PASSED_BY_REF(OutString, FStrProperty, FString);

	P_FINISH;
	P_NATIVE_BEGIN;
	XYAH_RETURN(bool, Generic_ToString(InProperty, Data, OutString));
	P_NATIVE_END;
}

FORCEINLINE DEFINE_FUNCTION(UXyahUtilityLibrary::execFromString)
{
	P_GET_PROPERTY(FStrProperty, InString);

	Stack.Step(Stack.Object, NULL);
	FProperty* RefProperty = Stack.MostRecentProperty;
	void* Data = Stack.MostRecentPropertyAddress;
	P_FINISH;

	P_NATIVE_BEGIN;
	XYAH_MARK_PROPERTY_DIRTY(RefProperty);
	XYAH_RETURN(bool, Generic_FromString(InString, RefProperty, Data));
	P_NATIVE_END;
}


template<class... Args>
void UXyahUtilityLibrary::Print(const FXyahPrintSettings& Settings, const FString& Format, Args&&... Arguments)
{
	PrintMessage(FString::Printf(Format, Arguments), Settings);
}
