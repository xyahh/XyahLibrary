// Fill out your copyright notice in the Description page of Project Settings.


#include "XyahArrayLibrary.h"
#include "XyahUtilityLibrary.h"
#include "XyahLibrary.h"

void UXyahArrayLibrary::Array_GetRandom(const TArray<int32>& Array, int32& OutRandomElement)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
}

bool UXyahArrayLibrary::Array_Sort(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

bool UXyahArrayLibrary::Array_RemoveIf(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

void UXyahArrayLibrary::GenericArray_GetRandom(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		ArrayProp->Inner->CopyCompleteValueFromScriptVM(OutItem, ArrayHelper.GetRawPtr(FMath::RandHelper(ArrayHelper.Num())));
	}
}


bool UXyahArrayLibrary::GenericArray_Sort(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
	, FName PredicateFunctionName)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

		if (IsValid(FuncOwner) && ArrayHelper.Num() > 1)
		{
			if (UFunction* Predicate = FindFunction_Checked(FuncOwner, PredicateFunctionName, TEXT("Sorting Failed"), ArrayProp->Inner, 2))
			{
				//Allocations Done Here
				uint8* Params = (uint8*)FMemory_Alloca(Predicate->ParmsSize);

				//Bubble Sort. ToDo: Improve Algorithm
				for (int32 i = 0; i < ArrayHelper.Num(); ++i)
				{
					for (int32 j = 0; j < ArrayHelper.Num(); ++j)
					{
						FMemory::Memzero(Params, Predicate->ParmsSize);
						int32 ParamsProcessed = 0;
						uint8* ReturnParam = 0;

						for (TFieldIterator<FProperty> It(Predicate); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It)
						{
							switch (ParamsProcessed)
							{
							case 0: It->CopyCompleteValueFromScriptVM(It->ContainerPtrToValuePtr<uint8>(Params), ArrayHelper.GetRawPtr(i)); break;
							case 1: It->CopyCompleteValueFromScriptVM(It->ContainerPtrToValuePtr<uint8>(Params), ArrayHelper.GetRawPtr(j)); break;
							default: ReturnParam = It->ContainerPtrToValuePtr<uint8>(Params); break;
							}

							++ParamsProcessed;
							if (ParamsProcessed >= 3)
								break;
						}
						FuncOwner->ProcessEvent(Predicate, Params);
						if (bool* ReturnBool = (bool*)ReturnParam)
						{
							if (*ReturnBool)
								ArrayHelper.SwapValues(i, j);
						}


					}
				}

				//Destroy Allocations
				for (TFieldIterator<FProperty> It(Predicate); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It)
				{
					It->DestroyValue_InContainer(Params);
				}
				return true;
			}
			
			
		}
	}
	return false;
}

bool UXyahArrayLibrary::GenericArray_RemoveIf(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner, FName PredicateFunctionName)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

		if (IsValid(FuncOwner) && ArrayHelper.Num() > 1)
		{
			if (UFunction* Predicate = FindFunction_Checked(FuncOwner, PredicateFunctionName, TEXT("RemoveIf Failed"), ArrayProp->Inner, 1))
			{
				//Allocations Done Here
				uint8* Params = (uint8*)FMemory_Alloca(Predicate->ParmsSize);

				//RemoveIf
				for (int32 i = 0; i < ArrayHelper.Num(); ++i)
				{
					FMemory::Memzero(Params, Predicate->ParmsSize);
					int32 ParamsProcessed = 0;
					uint8* ReturnParam = 0;

					for (TFieldIterator<FProperty> It(Predicate); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It)
					{
						switch (ParamsProcessed)
						{
						case 0: It->CopyCompleteValueFromScriptVM(It->ContainerPtrToValuePtr<uint8>(Params), ArrayHelper.GetRawPtr(i)); break;
						default: ReturnParam = It->ContainerPtrToValuePtr<uint8>(Params); break;
						}

						++ParamsProcessed;
						if (ParamsProcessed >= 2)
							break;
					}
					FuncOwner->ProcessEvent(Predicate, Params);
					if (bool* ReturnBool = (bool*)ReturnParam)
					{
						if (*ReturnBool)
							ArrayHelper.RemoveValues(i, 1);
					}
				}

				//Destroy Allocations
				for (TFieldIterator<FProperty> It(Predicate); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It)
				{
					It->DestroyValue_InContainer(Params);
				}
				return true;
			}


		}
	}
	return false;
}

UFunction* UXyahArrayLibrary::FindFunction_Checked(UObject* Object, FName FunctionName
	, const FString& ErrorString, FProperty* InnerProperty, int32 ExpectedInputParams)
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
			, *ErrorString , *FunctionName.ToString(), *Object->GetName());
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
			if (!XYAH(Utility) ArePropertiesTheSame(FuncProperty, InnerProperty))
			{
				XYAH_LOG(Warning, TEXT("(%s)! Comparator (%s) in Function (%s) must be of type (%s).")
					, *ErrorString
					, *FuncProperty->GetAuthoredName()
					, *FunctionName.ToString()
					, *InnerProperty->GetCPPType());
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


