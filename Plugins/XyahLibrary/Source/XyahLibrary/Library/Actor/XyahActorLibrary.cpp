// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.


#include "XyahActorLibrary.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Utility/XyahUtilityLibrary.h"

bool UXyahActorLibrary::BP_GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass
, TArray<AActor*>& OutActors, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName /*= NAME_None*/
, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

bool UXyahActorLibrary::BP_SpawnActorDeferred(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, AActor*& SpawnedActor, const FTransform& WorldTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride /*= ESpawnActorCollisionHandlingMethod::Undefined */, AActor* SpawnedActorOwner /*= nullptr */, FName PreBeginPlayFunction /*= NAME_None */, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

bool UXyahActorLibrary::GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass
, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, TArray<AActor*>& OutActors)
{
	OutActors.Empty();
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		bool bNotIgnoringClasses = (ClassesToIgnore.Num() == 0);
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			if (!ClassesToIgnore.Contains(It->GetClass()))
				OutActors.Add(*It);		
		}
		return OutActors.Num() > 0;
	}
	return false;
}

void UXyahActorLibrary::Generic_GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FProperty* ArrayInnerProperty
	, TArray<AActor*>& OutActors, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName, UObject* FuncOwner)
{
	OutActors.Empty();
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		UFunction* FilterFunc = 0;
		uint8* FilterFuncParams = 0;

		if (FilterFunctionName != NAME_None)
		{
			FilterFunc = XYAH(Utility) FindFunction(FuncOwner, FilterFunctionName, TEXT("Get All Actor of Class Error"), ArrayInnerProperty, 1);
			if (FilterFunc)
				FilterFuncParams = (uint8*)FMemory_Alloca(FilterFunc->ParmsSize);
		}

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
