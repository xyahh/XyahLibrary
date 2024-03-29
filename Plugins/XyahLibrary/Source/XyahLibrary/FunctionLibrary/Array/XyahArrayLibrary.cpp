// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#include "XyahArrayLibrary.h"
#include "XyahUtilityLibrary.h"

void UXyahArrayLibrary::BP_GetRandom(const TArray<int32>& Array, int32& OutRandomElement)
{
	XYAH_SHOULD_NEVER_HIT_THIS();
}

bool UXyahArrayLibrary::BP_Sort(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(0);
}

bool UXyahArrayLibrary::BP_RemoveIf(const TArray<int32>& Array, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(0);
}

bool UXyahArrayLibrary::BP_FindIf(const TArray<int32>& Array, TArray<int32>& OutArray, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(0);
}

bool UXyahArrayLibrary::BP_AnyIf(const TArray<int32>& Array, bool& bEvaluation, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(0);
}

bool UXyahArrayLibrary::BP_AllIf(const TArray<int32>& Array, bool& bEvaluation, FName PredicateFunctionName, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(0);
}

int32 UXyahArrayLibrary::GetNextIndex(int32 CurrentIndex, int32 DeltaIndex
	, const TArray<int32>& InArray, bool bClampAtEnds)
{
	const int32 Sum = CurrentIndex + DeltaIndex;
	const int32 Result = Sum % InArray.Num();

	if (bClampAtEnds)
	{
		if(Sum >= InArray.Num())
			return InArray.Num() - 1;
		else if(Sum < 0)
			return 0;
	}

	if (Result < 0)
		return InArray.Num() + Result;
	return Result;
}

void UXyahArrayLibrary::Generic_GetRandom(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		if(ArrayHelper.Num() > 0)
			ArrayProp->Inner->CopyCompleteValueFromScriptVM(OutItem, ArrayHelper.GetRawPtr(FMath::RandHelper(ArrayHelper.Num())));
	}
}

bool UXyahArrayLibrary::Generic_Sort(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner
	, FName PredicateFunctionName)
{
	if (TargetArray && IsValid(FuncOwner))
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		if (ArrayHelper.Num() > 1)
		{
			if (UFunction* Predicate = XYAH(Utility) ValidateFunction(FuncOwner, PredicateFunctionName
				, TEXT("Array Sorting Failed"), {ArrayProp->Inner, ArrayProp->Inner}, {FBoolProperty::StaticClass()}))
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
		else
		{
			XYAH_LIB_LOG(Log, TEXT("Sort did not take place! Array Element count less 2!"));
		}
	}
	return false;
}

bool UXyahArrayLibrary::Generic_RemoveIf(void* TargetArray, const FArrayProperty* ArrayProp, UObject* FuncOwner, FName PredicateFunctionName)
{
	if (TargetArray && IsValid(FuncOwner))
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

		if (ArrayHelper.Num() > 0)
		{
			if (UFunction* Predicate = XYAH(Utility) ValidateFunction(FuncOwner, PredicateFunctionName, TEXT("Array RemoveIf Failed")
				, { ArrayProp->Inner }, { FBoolProperty::StaticClass() }))
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
						{
							ArrayHelper.RemoveValues(i, 1);
							--i;
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
		else
		{
			XYAH_LIB_LOG(Log, TEXT("RemoveIf did not take place! Array Element count is 0!"));
		}
	}
	return false;
}

bool UXyahArrayLibrary::Generic_FindIf(void* TargetArray, void* OutTargetArray, const FArrayProperty* ArrayProp, const FArrayProperty* OutArrayProp, UObject* FuncOwner, FName PredicateFunctionName)
{
	if (TargetArray && OutTargetArray && IsValid(FuncOwner))
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		FScriptArrayHelper OutArrayHelper(OutArrayProp, OutTargetArray);

		if (ArrayHelper.Num() > 0)
		{
			if (UFunction* Predicate = XYAH(Utility) ValidateFunction(FuncOwner, PredicateFunctionName, TEXT("Array FindIf Failed")
				, { ArrayProp->Inner }, { FBoolProperty::StaticClass() }))
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
		else
		{
			XYAH_LIB_LOG(Log, TEXT("FindIf did not take place! Array Element count is 0!"));
		}
	}
	return false;
}

bool UXyahArrayLibrary::Generic_AllIf(void* TargetArray, const FArrayProperty* ArrayProp, bool& OutAllIfRetVal, UObject* FuncOwner, FName PredicateFunctionName)
{
	if (TargetArray && IsValid(FuncOwner))
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

		if (ArrayHelper.Num() > 0)
		{
			if (UFunction* Predicate = XYAH(Utility) ValidateFunction(FuncOwner, PredicateFunctionName, TEXT("Array AllIf Failed")
				, { ArrayProp->Inner }, { FBoolProperty::StaticClass() }))
			{
				//Allocations Done Here
				uint8* Params = (uint8*)FMemory_Alloca(Predicate->ParmsSize);

				OutAllIfRetVal = true;

				//Allif
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
						if (false == *ReturnBool)
							OutAllIfRetVal = false;
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
		else
		{
			XYAH_LIB_LOG(Log, TEXT("AllIf did not take place! Array Element count is 0!"));
		}
	}
	return false;
	
}

bool UXyahArrayLibrary::Generic_AnyIf(void* TargetArray, const FArrayProperty* ArrayProp, bool& OutAnyIfRetVal, UObject* FuncOwner, FName PredicateFunctionName)
{
	if (TargetArray && IsValid(FuncOwner))
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

		if (ArrayHelper.Num() > 0)
		{
			if (UFunction* Predicate = XYAH(Utility) ValidateFunction(FuncOwner, PredicateFunctionName, TEXT("Array AnyIf Failed")
				, { ArrayProp->Inner }, { FBoolProperty::StaticClass() }))
			{
				//Allocations Done Here
				uint8* Params = (uint8*)FMemory_Alloca(Predicate->ParmsSize);

				OutAnyIfRetVal = false;

				//AnyIf
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
							OutAnyIfRetVal = true;
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
		else
		{
			XYAH_LIB_LOG(Log, TEXT("AnyIf did not take place! Array Element count is 0!"));
		}
	}
	return false;
}

