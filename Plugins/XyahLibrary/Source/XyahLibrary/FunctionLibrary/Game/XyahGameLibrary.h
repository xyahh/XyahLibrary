// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahLibraryCore.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XyahGameLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLevelLoadingComplete, bool, bSucceeded);

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahGameLibrary : public UBlueprintFunctionLibrary
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

//C++ and Blueprint
public:

	//Gets the Net Local Role of a Actor or Actor Component (other UObjects will return EXyahNetRole::None)
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Actor", meta = (Keywords = "net role authority proxy simulated autonomous"))
	static EXyahNetRole GetLocalRole(UObject* Object);

	//Gets the Net Local Role of a Actor or Actor Component (other UObjects will return EXyahNetRole::None)
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Actor", meta = (Keywords = "net role authority proxy simulated autonomous"))
	static EXyahNetRole GetRemoteRole(UObject* Object);

	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Actor", meta = (Keywords = "server standalone client listen dedicated"))
	static EXyahNetMode GetNetMode(UObject* Object);

	//Forces the Addition of Cheats on Non-Shipping Builds. It doesn't do anything in Shipping Builds
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Actor")
	static void ForceAddCheats(class APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Actor", meta = (WorldContext = "WorldContextObject"))
	static bool LoadLevel(const UObject* WorldContextObject, const FString& Level, bool bAbsolute, const FString& Options
		, FOnLevelLoadingComplete OnLevelLoadComplete);

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

};


#if XYAH_GAME_LIBRARY
#include "XyahGameLibrary.inl"
#endif