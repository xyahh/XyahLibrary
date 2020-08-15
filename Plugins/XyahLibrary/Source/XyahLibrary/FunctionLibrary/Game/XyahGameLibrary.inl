// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

FORCEINLINE DEFINE_FUNCTION(UXyahGameLibrary::execBP_GetAllActorsOfClass)
{
	P_GET_OBJECT(UObject, WorldContextObject);
	P_GET_OBJECT(UClass, ActorClass);

	P_GET_TARRAY_REF(AActor*, OutActors);
	FArrayProperty* ActorProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);

	P_GET_TSET_REF(TSubclassOf<AActor>, ClassesToIgnore);
	P_GET_PROPERTY(FNameProperty, FilterFuncName);
	P_GET_PROPERTY(FObjectProperty, FuncOwner);
	P_FINISH;
	P_NATIVE_BEGIN;

	//No need to Mark property as dirty since we are getting TArrayRef
	Generic_GetAllActorsOfClass(WorldContextObject, ActorClass, ActorProperty ? ActorProperty->Inner : nullptr, OutActors, ClassesToIgnore, FilterFuncName, IsValid(FuncOwner) ? FuncOwner : XYAH_GET_OBJECT);
	P_NATIVE_END;
}

template<typename FilterPredicate>
bool UXyahGameLibrary::GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass
, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, TArray<AActor*>& OutActors, FilterPredicate FilterPred)
{
	OutActors.Empty();
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* CurrActor = *It;
			if (CurrActor && !ClassesToIgnore.Contains(CurrActor->GetClass()) && FilterPred(CurrActor))
				OutActors.Add(CurrActor);
		}
		return OutActors.Num() > 0;
	}
	return false;
}