// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.


#include "XyahGameLibrary.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Utility/XyahUtilityLibrary.h"

bool UXyahGameLibrary::BP_GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass
, TArray<AActor*>& OutActors, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName /*= NAME_None*/
, UObject* FunctionOwner /*= nullptr*/)
{
	XYAH_SHOULD_NEVER_HIT_THIS(false);
}

EXyahNetRole UXyahGameLibrary::GetLocalRole(UObject* Object)
{
	if (AActor* Actor = Cast<AActor>(Object))
	{
		return ConvertXyahNetRole(Actor->GetLocalRole());
	}
	else if (UActorComponent* ActorComponent = Cast<UActorComponent>(Object))
	{
		return ConvertXyahNetRole(ActorComponent->GetOwnerRole());
	}
	return EXyahNetRole::XNR_None;
}

EXyahNetRole UXyahGameLibrary::GetRemoteRole(UObject* Object)
{
	if (AActor* Actor = Cast<AActor>(Object))
	{
		return ConvertXyahNetRole(Actor->GetRemoteRole());
	}
	else if (UActorComponent* ActorComponent = Cast<UActorComponent>(Object))
	{
		AActor* ActorOwner = ActorComponent->GetOwner();
		return ActorOwner ? ConvertXyahNetRole(ActorOwner->GetLocalRole()) : EXyahNetRole::XNR_None;
	}
	return EXyahNetRole::XNR_None;
}

EXyahNetMode UXyahGameLibrary::GetNetMode(UObject* Object)
{
	if (AActor* Actor = Cast<AActor>(Object))
	{
		return ConvertXyahNetMode(Actor->GetNetMode());
	}
	else if (UActorComponent* ActorComponent = Cast<UActorComponent>(Object))
	{
		return ConvertXyahNetMode(ActorComponent->GetNetMode());
	}
	return EXyahNetMode::XNM_MAX;
}

#include "GameFramework/PlayerController.h"
void UXyahGameLibrary::ForceAddCheats(class APlayerController* PC)
{
#if !UE_BUILD_SHIPPING
	if (IsValid(PC))
	{
		PC->AddCheats(true);
	}
#endif
}

#include "UObject/UObjectGlobals.h"
bool UXyahGameLibrary::StartLoadingLevel(const FString& Level
	, int32 LoadingPriority, FOnLevelLoadingComplete OnLevelLoadingComplete)
{
	FString WantedMapPath = Level;
	if (!Level.EndsWith(TEXT(".umap")))
		WantedMapPath += TEXT(".umap");

	TArray<FString> FoundMapPaths = XYAH(Utility) GetAllFilesInDirectory(TEXT("Content"), EXyahDirectoryType::XDT_Game, TEXT(".umap"), true);
	FString* FoundMapPathPtr = FoundMapPaths.FindByPredicate([&WantedMapPath](const FString& MapPath)
	{
		return MapPath.EndsWith(WantedMapPath);
	});

	if (FoundMapPathPtr)
	{
		WantedMapPath = FPackageName::FilenameToLongPackageName(*FoundMapPathPtr);
		LoadPackageAsync(WantedMapPath
			, FLoadPackageAsyncDelegate::CreateLambda(
				[OnLevelLoadingComplete, WantedMapPath](const FName& PackageName, UPackage* LoadedPackage
					, EAsyncLoadingResult::Type Result)
		{
			OnLevelLoadingComplete.ExecuteIfBound(LoadedPackage, Result == EAsyncLoadingResult::Succeeded);

			switch (Result)
			{
			case EAsyncLoadingResult::Failed:
				XYAH_LIB_LOG(Warning, TEXT("Level Loading Failed! (%s)"), *WantedMapPath);
				break;

			case EAsyncLoadingResult::Succeeded:
				XYAH_LIB_LOG(Log, TEXT("Level Loading Success! (%s)"), *WantedMapPath);
				break;

			case EAsyncLoadingResult::Canceled:
				XYAH_LIB_LOG(Log, TEXT("Level Loading was Cancelled! (%s)"), *WantedMapPath);
				break;
			}
		}), LoadingPriority, PKG_ContainsMap);
		return true;
	}
	return false;
}

#include "Kismet/GameplayStatics.h"
void UXyahGameLibrary::FinishLoadingLevel(UObject* WorldContextObject
	, UObject* LevelPackage, bool bAbsolute, const FString& Options)
{
	if (IsValid(LevelPackage))
		UGameplayStatics::OpenLevel(WorldContextObject, LevelPackage->GetFName(), bAbsolute, Options);
}



bool UXyahGameLibrary::GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass
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

void UXyahGameLibrary::Generic_GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FProperty* ArrayInnerProperty
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
