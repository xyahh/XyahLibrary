// Fill out your copyright notice in the Description page of Project Settings.


#include "XyahUtilityLibrary.h"
#include "XyahLibrary.h"

void UXyahUtilityLibrary::Array_GetRandom(const TArray<int32>& Array, int32& OutRandomElement)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
}

bool UXyahUtilityLibrary::Array_Sort(const TArray<int32>& Array, FName PredicateFunctionName)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

void UXyahUtilityLibrary::GenericArray_GetRandom(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		ArrayProp->Inner->CopyCompleteValueFromScriptVM(OutItem, ArrayHelper.GetRawPtr(FMath::RandHelper(ArrayHelper.Num())));
	}
}

bool UXyahUtilityLibrary::GenericArray_Sort(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
	, FName PredicateFunctionName)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

		if (IsValid(FuncOwner) && ArrayHelper.Num() > 1)
		{
			UFunction* Predicate = FuncOwner->FindFunction(PredicateFunctionName);

			if (!Predicate)
			{
				XYAH_LOG(Warning, TEXT("Sorting Failed! Could not find Function (%s) in Object (%s)")
					, *PredicateFunctionName.ToString(), *FuncOwner->GetName());
				return false;
			}

			if (Predicate->NumParms != 3)
			{
				XYAH_LOG(Warning, TEXT("Sorting Failed! Pred Function (%s) Parameter Count Incorrect. It has to have 2 Input Elements and One Bool Return Value")
					, *PredicateFunctionName.ToString());
				return false;
			}

			TFieldIterator<FProperty> PredIterator(Predicate);
			FProperty* Inner = ArrayProp->Inner;
			int32 ParameterCount = 0;

			while (PredIterator && (PredIterator->PropertyFlags & CPF_Parm) && ParameterCount < 3)
			{
				FProperty* PredProperty = *PredIterator;

				if (ParameterCount < 2)
				{
					if (PredProperty->GetClass() != Inner->GetClass())
					{
						XYAH_LOG(Warning, TEXT("Sorting Failed! Comparator (%s) in Predicate Function (%s) must be of type (%s).")
							, *PredProperty->GetAuthoredName(), *PredicateFunctionName.ToString()
							, *Inner->GetClass()->GetName());
						return false; // Mismatch in Pred Property types
					}
					if (PredIterator->PropertyFlags & CPF_ReturnParm)
					{
						XYAH_LOG(Warning, TEXT("Sorting Failed! There should be 2 input Params in Predicate Function (%s)!")
							, *PredicateFunctionName.ToString());
						return false;
					}
				}
				else
				{
					if (PredProperty->GetClass() != FBoolProperty::StaticClass())
					{
						XYAH_LOG(Warning, TEXT("Sorting Failed! Last Parameter (Return Value) in Predicate Function (%s) must be of type bool.")
							, *PredicateFunctionName.ToString());
						return false; //last property (ret val) is not Bool
					}
				}

				++ParameterCount;
				++PredIterator;
			}

			if (PredIterator && (PredIterator->PropertyFlags & CPF_Parm))
			{
				//Predicate has additional Parameters
				XYAH_LOG(Warning, TEXT("Sorting Failed! Predicate Function (%s) has additional parameters. Please remove these and try sorting again.")
					, *PredicateFunctionName.ToString());
				return false;
			}

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
	return false;
}
