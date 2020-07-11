// Fill out your copyright notice in the Description page of Project Settings.


#include "XyahArrayLibrary.h"
#include "XyahUtilityLibrary.h"


void UXyahArrayLibrary::BlueprintGetRandom(const TArray<int32>& Array, int32& OutRandomElement)
{
	XYAH_SHOULD_NEVER_HIT_THIS();
}

bool UXyahArrayLibrary::BlueprintSort(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(0);
}

bool UXyahArrayLibrary::BlueprintRemoveIf(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(0);
}

bool UXyahArrayLibrary::BlueprintFindIf(const TArray<int32>& Array, TArray<int32>& OutArray, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(0);
}

void UXyahArrayLibrary::GenericBlueprintGetRandom(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		ArrayProp->Inner->CopyCompleteValueFromScriptVM(OutItem, ArrayHelper.GetRawPtr(FMath::RandHelper(ArrayHelper.Num())));
	}
}


bool UXyahArrayLibrary::GenericBlueprintSort(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
	, FName PredicateFunctionName)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

		if (IsValid(FuncOwner) && ArrayHelper.Num() > 1)
		{
			if (UFunction* Predicate = XYAH(Utility) FindFunction(FuncOwner, PredicateFunctionName, TEXT("Sorting Failed"), ArrayProp->Inner, 2))
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

bool UXyahArrayLibrary::GenericBlueprintRemoveIf(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner, FName PredicateFunctionName)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

		if (IsValid(FuncOwner) && ArrayHelper.Num() > 1)
		{
			if (UFunction* Predicate = XYAH(Utility) FindFunction(FuncOwner, PredicateFunctionName, TEXT("RemoveIf Failed"), ArrayProp->Inner, 1))
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


bool UXyahArrayLibrary::GenericBlueprintFindIf(void* TargetArray, void* OutTargetArray, const FArrayProperty* ArrayProp, const FArrayProperty* OutArrayProp, UObject* FuncOwner, FName PredicateFunctionName)
{
	if (TargetArray && OutTargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		FScriptArrayHelper OutArrayHelper(OutArrayProp, OutTargetArray);

		if (IsValid(FuncOwner) && ArrayHelper.Num() > 1)
		{
			if (UFunction* Predicate = XYAH(Utility) FindFunction(FuncOwner, PredicateFunctionName, TEXT("FindIf Failed"), ArrayProp->Inner, 1))
			{
				//Allocations Done Here
				uint8* Params = (uint8*)FMemory_Alloca(Predicate->ParmsSize);

				//FindIf
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
						{
							int32 Index = OutArrayHelper.AddValue();
							ArrayProp->Inner->CopyCompleteValueFromScriptVM(OutArrayHelper.GetRawPtr(Index), ArrayHelper.GetRawPtr(i));
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



