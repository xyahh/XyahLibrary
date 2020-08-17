// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#include "XyahUtilityLibrary.h"
#include "Math/XyahMathLibrary.h"
#include "Engine/Engine.h"

// Blueprint ONLY

void UXyahUtilityLibrary::GetWildcard(int32 Wildcard, FString& Name, FString& Type)
{
	XYAH_SHOULD_NEVER_HIT_THIS();
}

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

void UXyahUtilityLibrary::StatFPS(float& FPS, float& Milliseconds)
{
#if WITH_ENGINE
	extern ENGINE_API float GAverageFPS;
	extern ENGINE_API float GAverageMS;
	FPS = GAverageFPS;
	Milliseconds = GAverageMS;
#endif
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
				GetPropertyValueString(OwnerObject, *PropertyIter, PropertyValue);
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
		return GetPropertyValueString(OwnerObject
		, FindFProperty<FProperty>(OwnerObject->GetClass(), *PropertyName)
		, OutPropertyValue);
	}
	return false;
}

TArray<FString> UXyahUtilityLibrary::GetAllFilesInDirectory(const FString& Directory
	, TEnumAsByte<EXyahDirectoryType> DirectoryType, const FString& FileExtension, bool bRecursiveSearch)
{
	FString DirectoryToSearch;
	switch (DirectoryType)
	{
	case EXyahDirectoryType::XDT_Absolute: 
		DirectoryToSearch = Directory; break;
	case EXyahDirectoryType::XDT_Game: 
		DirectoryToSearch = FPaths::ProjectDir() + Directory; break;
	case EXyahDirectoryType::XDT_Saved: 
		DirectoryToSearch = FPaths::ProjectSavedDir() + Directory; break;
	}
		
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	TArray<FString> FoundFiles;
	if (bRecursiveSearch)
	{
		PlatformFile.FindFilesRecursively(FoundFiles, *DirectoryToSearch, *FileExtension);
	}
	else
	{
		PlatformFile.FindFiles(FoundFiles, *DirectoryToSearch, *FileExtension);
	}
	return FoundFiles;
}

bool UXyahUtilityLibrary::GetPropertyValueString(UObject* OwnerObject, FProperty* Property, FString& OutPropertyValue)
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

UFunction* UXyahUtilityLibrary::ValidateFunction(const UObject* Object, FName FunctionName, const FString& ErrorString
	, TArray<FProperty*> InputProperties, TArray<FFieldClass*> OutputProperties)
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

	const int32 InputParamCount = InputProperties.Num();
	const int32 OutputParamCount = OutputProperties.Num();
	const int32 TotalParamCount = InputParamCount + OutputParamCount; 

	if (Function->NumParms != TotalParamCount)
	{
		XYAH_LIB_LOG(Warning, TEXT("(%s)! Pred Function (%s) Parameter Count Incorrect. There should be %d input params and %d output params!")
			, *ErrorString
			, *FunctionName.ToString()
			, InputParamCount, OutputParamCount);
		return nullptr;
	}

	TFieldIterator<FProperty> FuncIterator(Function);
	auto InputParamIter = InputProperties.CreateIterator();
	auto OutputParamIter = OutputProperties.CreateIterator();
	int32 CheckedParams = 0;

	//Check for Input Params
	while (FuncIterator && (FuncIterator->PropertyFlags & CPF_Parm) 
		&& InputParamIter && CheckedParams <= InputParamCount)
	{
		FProperty* FuncProperty = *FuncIterator;
		FProperty* InputProperty = *InputParamIter;

		if (!XYAH(Utility) CheckProperties(FuncProperty, InputProperty))
		{
			if (InputProperty)
			{
				XYAH_LIB_LOG(Warning, TEXT("(%s)! Input Property (%s) in Function (%s) must be of type (%s).")
					, *ErrorString
					, *FuncProperty->GetAuthoredName()
					, *FunctionName.ToString()
					, *InputProperty->GetCPPType());
			}
			else
			{
				XYAH_LIB_LOG(Warning, TEXT("(%s)! Input Property (%s) in Function (%s) failed! Input Property is invalid.")
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
				, InputParamCount
				, *FunctionName.ToString());
			return nullptr;
		}
		
	
		++CheckedParams;
		++FuncIterator;
		++InputParamIter;
	}

	//Reset Checked Params for Output now.
	CheckedParams = 0;
	//Check for Output Params
	while (FuncIterator && (FuncIterator->PropertyFlags & (CPF_Parm)) 
		&& (FuncIterator->PropertyFlags & (CPF_OutParm))
		&& OutputParamIter && CheckedParams <= OutputParamCount)
	{
		FProperty* FuncProperty = *FuncIterator;
		FFieldClass* OutputFieldClass = *OutputParamIter;

		if (FuncProperty->GetClass() != OutputFieldClass)
		{
			if (OutputFieldClass)
			{
				XYAH_LIB_LOG(Warning, TEXT("(%s)! Output Property (%s) in Function (%s) must be of type (%s).")
					, *ErrorString
					, *FuncProperty->GetAuthoredName()
					, *FunctionName.ToString()
					, *OutputFieldClass->GetName());
			}
			else
			{
				XYAH_LIB_LOG(Warning, TEXT("(%s)! Output Property (%s) in Function (%s) failed! Output Property is invalid.")
					, *ErrorString
					, *FuncProperty->GetAuthoredName()
					, *FunctionName.ToString());
			}
			return nullptr; // Mismatch in Func Property types
		}

		++CheckedParams;
		++FuncIterator;
		++OutputParamIter;
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
	OutString.Empty();
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

void UXyahUtilityLibrary::Generic_GetWildcardType(FProperty* Property, FString& Name, FString& Type)
{
	Name.Empty();
	Type.Empty();
	
#define GetPropertyTypeName(PropertyType, Prefix) { if(PropertyType* Prop = CastField<PropertyType>(Property)) { Name = Prop->PropertyClass->GetName(); Type = Prefix; return; } }

	if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		Name = StructProperty->Struct->GetAuthoredName();
		Type = TEXT("Struct");
		return;
	}

	GetPropertyTypeName(FObjectProperty, TEXT("Object"));
	GetPropertyTypeName(FClassProperty, TEXT("Class"));
	GetPropertyTypeName(FSoftObjectProperty, TEXT("SoftObject"));
	GetPropertyTypeName(FSoftClassProperty, TEXT("SoftClass"));

	if (Property)
	{
		Name = Type = Property->GetCPPType();
	}		
}
