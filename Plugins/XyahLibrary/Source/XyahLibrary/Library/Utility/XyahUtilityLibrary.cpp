// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#include "XyahUtilityLibrary.h"
#include "../Math/XyahMathLibrary.h"

#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Engine/World.h"

// Blueprint ONLY

bool UXyahUtilityLibrary::GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors
	, const TSet<TSubclassOf<AActor>>& ClassesToIgnore
	, FName FilterFunction /*= NAME_None*/, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

void UXyahUtilityLibrary::GetClassDefaultObject(TSubclassOf<UObject> ObjectClass, UObject*& OutObject)
{
	OutObject = GetMutableDefault<UObject>(ObjectClass);
}

#include "XyahLibrary/Settings/XyahSettings.h"
void UXyahUtilityLibrary::GetSettings(TSubclassOf<UXyahBaseSettings> SettingsClass, UXyahBaseSettings*& OutSettings)
{
	OutSettings = GetMutableDefault<UXyahBaseSettings>(SettingsClass);
}

//Blueprint & C++
void UXyahUtilityLibrary::PrintMessage(const FString& Message /*= FString(TEXT("Xyah Library"))*/, int32 LogID /*= -1 */, bool bPrintToScreen /*= true*/
	, FLinearColor ScreenTextColor /*= FLinearColor(0.0, 0.66, 1.0) */, bool bPrintToLog /*= true*/, int32 ConsoleTextColor /*= 0x1111 */
	, float Duration /*= 2.f*/, bool bNewerOnTop /*= true*/, float TextScale /*= 1.f*/)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

	if (bPrintToLog)
	{
		FString BinaryString(4, *XYAH(Math) ToBinaryString(ConsoleTextColor).Reverse());
		SET_WARN_COLOR(*BinaryString);
		XYAH_LOG(Log, TEXT("%s"), *Message);
		CLEAR_WARN_COLOR();
	}
	else
	{
		XYAH_LOG(Verbose, TEXT("%s"), *Message);
	}

	if (bPrintToScreen)
	{
		if (GAreScreenMessagesEnabled)
		{
			GEngine->AddOnScreenDebugMessage(LogID, Duration > 0.f ? Duration : 1.f, ScreenTextColor.ToFColor(true), Message, bNewerOnTop, FVector2D(TextScale));
		}
		else
		{
			XYAH_LOG(VeryVerbose, TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
		}
	}
#endif

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
	OutPropertyValue.Empty();
	if (IsValid(OwnerObject) && !PropertyName.IsEmpty())
	{
		FProperty* Property = FindFProperty<FProperty>(OwnerObject->GetClass(), *PropertyName);
		if (Property)
		{
			Property->ExportText_InContainer(0, OutPropertyValue, OwnerObject, OwnerObject, OwnerObject, PPF_SimpleObjectText);
			return true;
		}
	}
	return false;
}

FString UXyahUtilityLibrary::VectorToString(const FVector& V)
{
	return FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), V.X, V.Y, V.Z);
}

FString UXyahUtilityLibrary::RotatorToString(const FRotator& R)
{
	return FString::Printf(TEXT("(Pitch=%f,Yaw=%f,Roll=%f)"), R.Pitch, R.Yaw, R.Roll);
}

FString UXyahUtilityLibrary::TransformToString(const FTransform& T)
{
	FQuat Rot = T.GetRotation();
	FVector Loc = T.GetLocation();
	FVector Scale = T.GetScale3D();
	return FString::Printf(TEXT("(Rotation=(X=%f,Y=%f,Z=%f,W=%f),Translation=(X=%f,Y=%f,Z=%f),Scale3D=(X=%f,Y=%f,Z=%f))")
		, Rot.X, Rot.Y, Rot.Z, Rot.W, Loc.X, Loc.Y, Loc.Z, Scale.X, Scale.Y, Scale.Z);
}

FString UXyahUtilityLibrary::Vector2DToString(const FVector2D& V)
{
	return FString::Printf(TEXT("(X=%f,Y=%f)"), V.X, V.Y);
}

bool UXyahUtilityLibrary::StringToVector(const FString& S, FVector& V)
{
	V.X = V.Y = V.Z = 0.f;
	return FParse::Value(*S, TEXT("X="), V.X) && FParse::Value(*S, TEXT("Y="), V.Y) && FParse::Value(*S, TEXT("Z="), V.Z);
}

bool UXyahUtilityLibrary::StringToRotator(const FString& S, FRotator& R)
{
	R.Pitch = R.Yaw = R.Roll = 0.f;
	const bool bSuccessful = FParse::Value(*S, TEXT("Pitch="), R.Pitch) && FParse::Value(*S, TEXT("Yaw="), R.Yaw) && FParse::Value(*S, TEXT("Roll="), R.Roll);
	R.DiagnosticCheckNaN();
	return bSuccessful;
}

bool UXyahUtilityLibrary::StringToTransform(const FString& S, FTransform& T)
{
	/* ToDo: Better way to Parse a String to the Transform Format given by Get/SetProperty? */

	FQuat Rotation;
	FVector Location;
	FVector Scale;

	//Rotation
	{
		int32 StartingIndex = S.Find("Rotation=");
		if (StartingIndex < 0)
			return false;

		FString RotationString = S.RightChop(StartingIndex);
		if (false == (FParse::Value(*RotationString, TEXT("X="), Rotation.X) && FParse::Value(*RotationString, TEXT("Y="), Rotation.Y)
			&& FParse::Value(*RotationString, TEXT("Z="), Rotation.Z) && FParse::Value(*RotationString, TEXT("W="), Rotation.W)))
			return false;
	}

	//Translation
	{
		int32 StartingIndex = S.Find("Translation=");
		if (StartingIndex < 0)
			return false;

		FString LocationString = S.RightChop(StartingIndex);
		if (false == (FParse::Value(*LocationString, TEXT("X="), Location.X) && FParse::Value(*LocationString, TEXT("Y="), Location.Y)
			&& FParse::Value(*LocationString, TEXT("Z="), Location.Z)))
			return false;
	}
	
	//Scale
	{

		int32 StartingIndex = S.Find("Scale3D=");
		if (StartingIndex < 0)
			return false;

		FString ScaleString = S.RightChop(StartingIndex);
		if (false == (FParse::Value(*ScaleString, TEXT("X="), Scale.X) && FParse::Value(*ScaleString, TEXT("Y="), Scale.Y)
			&& FParse::Value(*ScaleString, TEXT("Z="), Scale.Z)))
			return false;
	}

	T = FTransform(Rotation, Location, Scale);
	return true;
}

bool UXyahUtilityLibrary::StringToVector2D(const FString& S, FVector2D& V)
{
	V.X = V.Y  = 0.f;
	return FParse::Value(*S, TEXT("X="), V.X) && FParse::Value(*S, TEXT("Y="), V.Y);
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
		XYAH_LOG(Warning, TEXT("(%s)! Object is Invalid!")
			, *ErrorString);
		return nullptr;
	}

	UFunction* Function = Object->FindFunction(FunctionName);

	if (!Function)
	{
		XYAH_LOG(Warning, TEXT("(%s)! Could not find Function (%s) in Object (%s)")
			, *ErrorString, *FunctionName.ToString(), *Object->GetName());
		return nullptr;
	}

	const int32 TotalParams = ExpectedInputParams + 1; //+ bool retval

	if (Function->NumParms != TotalParams)
	{
		XYAH_LOG(Warning, TEXT("(%s)! Pred Function (%s) Parameter Count Incorrect. There should be %d input params and a bool return value!")
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
					XYAH_LOG(Warning, TEXT("(%s)! Comparator (%s) in Function (%s) must be of type (%s).")
						, *ErrorString
						, *FuncProperty->GetAuthoredName()
						, *FunctionName.ToString()
						, *InnerProperty->GetCPPType());
				}
				else
				{
					XYAH_LOG(Warning, TEXT("(%s)! Comparator (%s) in Function (%s) failed! Inner Property is NULL.")
						, *ErrorString
						, *FuncProperty->GetAuthoredName()
						, *FunctionName.ToString());
				}

			
				return nullptr; // Mismatch in Func Property types
			}
			if (FuncIterator->PropertyFlags & CPF_ReturnParm)
			{
				XYAH_LOG(Warning, TEXT("(%s)! There should be %d input Params in Function (%s)!")
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
				XYAH_LOG(Warning, TEXT("(%s)! Last Parameter (Return Value) in Predicate Function (%s) must be of type bool.")
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
		XYAH_LOG(Warning, TEXT("(%s)! Function (%s) has additional parameters. Please remove these and try again.")
			, *ErrorString
			, *FunctionName.ToString());
		return nullptr;
	}

	return Function;
}

void UXyahUtilityLibrary::Generic_GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FProperty* ArrayInnerProperty
	, TArray<AActor*>& OutActors,  const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName, UObject* FuncOwner)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		UFunction* FilterFunc = 0;
		uint8* FilterFuncParams = 0;

		if (FilterFunctionName != NAME_None)
		{
			FilterFunc = FindFunction(FuncOwner, FilterFunctionName, TEXT("Get All Actor of Class Error"),  ArrayInnerProperty , 1);
			if (FilterFunc)
				FilterFuncParams = (uint8*)FMemory_Alloca(FilterFunc->ParmsSize);
		}

		bool bNotIgnoringClasses = (ClassesToIgnore.Num() == 0);

		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;
			if (ClassesToIgnore.Contains(Actor->GetClass()))
				continue;

			bool bAddActor = true;

			if (FilterFunc)
			{
				bAddActor = false;
				FMemory::Memzero(FilterFuncParams, FilterFunc->ParmsSize);
				int32 ParamsProcessed = 0;
				uint8* ReturnParam = 0;
				for (TFieldIterator<FProperty> PropIter(FilterFunc); PropIter && (PropIter->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++PropIter)
				{
					switch (ParamsProcessed)
					{
					case 0: 
						//The Prop Iter SHOULD be an Actor Property (i.e. FObjectProperty)
						if (FObjectProperty* ObjProp = CastField<FObjectProperty>(*PropIter))
							ObjProp->SetPropertyValue_InContainer(PropIter->ContainerPtrToValuePtr<uint8>(FilterFuncParams), Actor);
						break;
					default: 
						ReturnParam = PropIter->ContainerPtrToValuePtr<uint8>(FilterFuncParams); break;
					}

					++ParamsProcessed;
					if (ParamsProcessed >= 2)
						break;
				}
				FuncOwner->ProcessEvent(FilterFunc, FilterFuncParams);
				if (bool* ReturnBool = (bool*)ReturnParam)
					bAddActor = *ReturnBool;
			}

			if (bAddActor)
				OutActors.Add(Actor);
		}


		if (FilterFunc)
		{
			//Destroy Allocations
			for (TFieldIterator<FProperty> PropIter(FilterFunc); PropIter && (PropIter->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++PropIter)
			{
				PropIter->DestroyValue_InContainer(FilterFuncParams);
			}
		}
	}
}
