// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XyahActorLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahActorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//Blueprint Only	
protected:

	/*
	Gets all the Actors of a Given Class but first Filters out all the actors whose class belongs in the ClassesToIgnore set.
	Additionally, if present and valid, the Filter Function will be called for every actor filtered after the ClassesToIgnore.
	If the Function returns true, the actor will be kept in the Output list, else it will be filtered out of the list.

	@param ClassesToIgnore - The Subclasses of the ActorClass to NOT consider and be left out.
	@param FilterFunctionName - The Name of the Function to call, if not specified, this extra Filtering Stage will not take place and return immediately. If specified this Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@param OutActors - The filtered list of actors
	@return [bool] Whether GetAllActorsOfClass actually took place or if the given Filter Function was not found.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Actor", meta = (DisplayName = "Get All Actors of Class", WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutActors", AutoCreateRefTerm = "ClassesToIgnore", AdvancedDisplay = "ClassesToIgnore,FilterFunctionName,FunctionOwner"))
	static bool BP_GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors
		, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName = NAME_None, UObject* FunctionOwner = nullptr);

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Actor", meta = (DisplayName = "Spawn Actor Deferred", WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass", DynamicOutputParam = "SpawnedActor", AutoCreateRefTerm = "ClassesToIgnore", AdvancedDisplay = "2"))
	static bool BP_SpawnActorDeferred(const UObject* WorldContextObject
		, TSubclassOf<AActor> ActorClass, AActor*& SpawnedActor, const FTransform& WorldTransform
		, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined
		, AActor* SpawnedActorOwner = nullptr
		, FName PreBeginPlayFunction = NAME_None
		, UObject* FunctionOwner = nullptr);

//C++
public:

	//Gets all the Actors that belong to a class. Doesn't consider those child classes in the "ClassesToIgnore" set. Returns true if there were actors found.
	static bool GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const TSet<TSubclassOf<AActor>>& ClassesToIgnore
		, TArray<AActor*>& OutActors);

	//Gets all the Actors that belong to a class. Doesn't consider those child classes in the "ClassesToIgnore" set or if the FilterPred function returns false. Returns true if there were actors found.
	template<typename FilterPredicate>
	static bool GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const TSet<TSubclassOf<AActor>>& ClassesToIgnore
		, TArray<AActor*>& OutActors, FilterPredicate FilterPred);

//Generic Funcs
public:

	static void Generic_GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass
		, FProperty* ArrayInnerProperty, TArray<AActor*>& OutActors, const TSet<TSubclassOf<AActor>>& ClassesToIgnore
		, FName FilterFunctionName, UObject* FuncOwner);

//Exec Funcs 
public:

	DECLARE_FUNCTION(execBP_GetAllActorsOfClass);
	DECLARE_FUNCTION(execBP_SpawnActorDeferred);
};


#if XYAH_ACTOR_LIBRARY
#include "XyahActorLibrary.inl"
#endif