// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#include "XyahUtilityLibrary.h"
#include "Math/XyahMathLibrary.h"
#include "Engine/Engine.h"

// Blueprint ONLY

bool UXyahUtilityLibrary::ToString(int32 InValue, FString& OutString)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

bool UXyahUtilityLibrary::FromString(const FString& InString, int32& RefValue)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

void UXyahUtilityLibrary::GetClassDefaultObject(TSubclassOf<UObject> ObjectClass, UObject*& OutObject)
{
	OutObject = GetMutableDefault<UObject>(ObjectClass);
}

#include "XyahLibrary/Settings/XyahSettings.h"
void UXyahUtilityLibrary::GetSettings(TSubclassOf<UXyahSettings> SettingsClass, UXyahSettings*& OutSettings)
{
	OutSettings = GetMutableDefault<UXyahSettings>(SettingsClass);
}

//Blueprint & C++
void UXyahUtilityLibrary::PrintMessage(const FString& Message, const FXyahPrintSettings& Settings)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

	if (Settings.bPrintToLog)
	{
		FString BinaryString(4, *XYAH(Math) ToBinaryString(Settings.ConsoleTextColor, true).Reverse());
		SET_WARN_COLOR(*BinaryString);
		XYAH_LIB_LOG(Log, TEXT("%s"), *Message);
		CLEAR_WARN_COLOR();
	}
	else
	{
		XYAH_LIB_LOG(Verbose, TEXT("%s"), *Message);
	}

	if (Settings.bPrintToScreen)
	{
		if (GAreScreenMessagesEnabled)
		{
			GEngine->AddOnScreenDebugMessage(Settings.LogID, Settings.Duration > 0.f ? Settings.Duration : 1.f
			, Settings.ScreenTextColor.ToFColor(true), Message, Settings.bNewerOnTop, FVector2D(Settings.TextScale));
		}
		else
		{
			XYAH_LIB_LOG(VeryVerbose, TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
		}
	}
#endif
}

void UXyahUtilityLibrary::GetProperties(UObject* OwnerObject, const TSet<TSubclassOf<UObject>>& ExcludeParents
, TMap<FString, FString>& OutProperties, int32 PropertyFlags /*= 0*/)
{
	OutProperties.Empty();
	if (OwnerObject)
	{
		for (TFieldIterator<FProperty> PropertyIter(OwnerObject->GetClass()); PropertyIter; ++PropertyIter)
		{
			if (ExcludeParents.Contains(PropertyIter->GetOwnerClass()))
				continue;

			if(PropertyFlags == EXyahPropertyFlags::XPF_All 
			|| (static_cast<uint64>(PropertyIter->GetPropertyFlags()) & ConvertXyahPropertyFlags(PropertyFlags)))
			{
				FString PropertyValue;
				GetPropertyValue_Internal(OwnerObject, *PropertyIter, PropertyValue);
				OutProperties.Add(PropertyIter->GetName(), PropertyValue);
			}
		}
	}
}

bool UXyahUtilityLibrary::SetPropertyValue(UObject* OwnerObject, const FString& PropertyName, const FString& PropertyValue)
{

	if (IsValid(OwnerObject) && !PropertyName.IsEmpty())
	{
		FProperty* Property = FindFProperty<FProperty>(OwnerObject->GetClass(), *PropertyName);
		if (Property)
		{
#if WITH_EDITOR
			OwnerObject->Modify();
			OwnerObject->PreEditChange(Property);
#endif
			Property->ImportText(*PropertyValue, Property->ContainerPtrToValuePtr<uint8>(OwnerObject), 0, OwnerObject);

#if WITH_EDITOR
			FPropertyChangedEvent PropertyChangedEvent(Property, EPropertyChangeType::ValueSet);
			OwnerObject->PostEditChangeProperty(PropertyChangedEvent);
#endif
			return true;
		}
	}
	return false;
}

bool UXyahUtilityLibrary::GetPropertyValue(UObject* OwnerObject, const FString& PropertyName, FString& OutPropertyValue)
{
	if (IsValid(OwnerObject) && !PropertyName.IsEmpty())
	{
		return GetPropertyValue_Internal(OwnerObject
		, FindFProperty<FProperty>(OwnerObject->GetClass(), *PropertyName)
		, OutPropertyValue);
	}
	return false;
}

bool UXyahUtilityLibrary::GetPropertyValue_Internal(UObject* OwnerObject, FProperty* Property, FString& OutPropertyValue)
{
	OutPropertyValue.Empty();
	if (Property)
	{
		FString PropertyVal;
		bool bProcessedFirst = false;
		for (int32 i = 0; i < Property->ArrayDim; ++i)
		{
			if (Property->ExportText_InContainer(i, PropertyVal, OwnerObject, OwnerObject, OwnerObject, PPF_SimpleObjectText))
			{
				if (bProcessedFirst)
					OutPropertyValue.Append(",");
				OutPropertyValue += PropertyVal;
				bProcessedFirst = true;

			}
		}
		return !OutPropertyValue.IsEmpty();
	}
	return false;
}

bool UXyahUtilityLibrary::CheckProperties(const FProperty* A, const FProperty* B, bool bCheckPropertyOffsets, bool bCheckPropertyNames)
{
	if (A == B)
		return true;

	if (!A || !B) //one of properties is null
		return false;

	if (bCheckPropertyNames && (A->GetFName() != B->GetFName()))
		return false;

	if (A->GetSize() != B->GetSize())
		return false;

	if (bCheckPropertyOffsets && (A->GetOffset_ForGC() != B->GetOffset_ForGC()))
		return false;

	if (!A->SameType(B))
		return false;

	return true;
}

UFunction* UXyahUtilityLibrary::FindFunction(const UObject* Object, FName FunctionName, const FString& ErrorString, FProperty* InnerProperty, int32 ExpectedInputParams)
{
	if (false == IsValid(Object))
	{
		XYAH_LIB_LOG(Warning, TEXT("(%s)! Object is Invalid!")
			, *ErrorString);
		return nullptr;
	}

	UFunction* Function = Object->FindFunction(FunctionName);

	if (!Function)
	{
		XYAH_LIB_LOG(Warning, TEXT("(%s)! Could not find Function (%s) in Object (%s)")
			, *ErrorString, *FunctionName.ToString(), *Object->GetName());
		return nullptr;
	}

	const int32 TotalParams = ExpectedInputParams + 1; //+ bool retval

	if (Function->NumParms != TotalParams)
	{
		XYAH_LIB_LOG(Warning, TEXT("(%s)! Pred Function (%s) Parameter Count Incorrect. There should be %d input params and a bool return value!")
			, *ErrorString
			, *FunctionName.ToString()
			, ExpectedInputParams);
		return nullptr;
	}

	TFieldIterator<FProperty> FuncIterator(Function);
	int32 ParameterCount = 0;

	while (FuncIterator && (FuncIterator->PropertyFlags & CPF_Parm) && ParameterCount < TotalParams)
	{
		FProperty* FuncProperty = *FuncIterator;

		if (ParameterCount < ExpectedInputParams)
		{
			if (!XYAH(Utility) CheckProperties(FuncProperty, InnerProperty))
			{
				if (InnerProperty)
				{
					XYAH_LIB_LOG(Warning, TEXT("(%s)! Comparator (%s) in Function (%s) must be of type (%s).")
						, *ErrorString
						, *FuncProperty->GetAuthoredName()
						, *FunctionName.ToString()
						, *InnerProperty->GetCPPType());
				}
				else
				{
					XYAH_LIB_LOG(Warning, TEXT("(%s)! Comparator (%s) in Function (%s) failed! Inner Property is NULL.")
						, *ErrorString
						, *FuncProperty->GetAuthoredName()
						, *FunctionName.ToString());
				}

			
				return nullptr; // Mismatch in Func Property types
			}
			if (FuncIterator->PropertyFlags & CPF_ReturnParm)
			{
				XYAH_LIB_LOG(Warning, TEXT("(%s)! There should be %d input Params in Function (%s)!")
					, *ErrorString
					, ExpectedInputParams
					, *FunctionName.ToString());
				return nullptr;
			}
		}
		else
		{
			//Always check for a Bool ret val since this Func is used to Check PREDICATE funcs. 
			if (FuncProperty->GetClass() != FBoolProperty::StaticClass()) //I think I do not need to do a rigorous Check for BoolProperty.
			{
				XYAH_LIB_LOG(Warning, TEXT("(%s)! Last Parameter (Return Value) in Predicate Function (%s) must be of type bool.")
					, *ErrorString
					, *FunctionName.ToString());
				return nullptr; //last property (ret val) is not Bool
			}
		}

		++ParameterCount;
		++FuncIterator;
	}

	if (FuncIterator && (FuncIterator->PropertyFlags & CPF_Parm))
	{
		//Predicate has additional Remaining Parameters! This shouldn't be called at all since I checked Param count...
		XYAH_LIB_LOG(Warning, TEXT("(%s)! Function (%s) has additional parameters. Please remove these and try again.")
			, *ErrorString
			, *FunctionName.ToString());
		return nullptr;
	}

	return Function;
}

bool UXyahUtilityLibrary::Generic_ToString(FProperty* Property, void* Data, FString& OutString)
{
	if (Property && Data)
	{
		return Property->ExportText_Direct(OutString, Data, Data, 0, PPF_SimpleObjectText);
	}
	return false;
}

bool UXyahUtilityLibrary::Generic_FromString(const FString& InString, FProperty* Property, void* Data)
{
	if (Property && Data)
	{
		return !!Property->ImportText(*InString, Data, 0, 0);
	}
	return false;
}
