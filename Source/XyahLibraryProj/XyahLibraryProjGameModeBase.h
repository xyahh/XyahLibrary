// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XyahLibraryProjGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class XYAHLIBRARYPROJ_API AXyahLibraryProjGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllActorsThatAreCloseToTheOrigin(TSubclassOf<AActor> ActorClass, float Threshold);

	UFUNCTION(BlueprintCallable)
	bool IsAnyNameLongerThan(int32 Length);

	UFUNCTION(BlueprintCallable)
	bool AreAllNamesLongerThan(int32 Length);

	UFUNCTION(BlueprintPure)
	FString GetRandomName() const;

	UFUNCTION(BlueprintPure)
	void GetRandomNumber(FString& String, int32& Number) const;

	UFUNCTION(BlueprintPure)
	void GetNumberFromIndex(int32 Index, FString& String, int32& Number) const;

	UFUNCTION(BlueprintCallable)
	void ReplaceNumberKey(const FString& OldNumber, const FString& NewNumber);

	UFUNCTION(BlueprintCallable)
	void RemoveIfNumberLessThan(int32 Number);

	UFUNCTION(BlueprintCallable)
	void FindIfNumberLessThan(int32 Number, TArray<FString>& OutKeys);

	UFUNCTION(BlueprintCallable)
	bool SafeAddNumber(const FString& String, const int32& Number);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> Numbers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> TestNames;


};
