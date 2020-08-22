// Copyright Epic Games, Inc. All Rights Reserved.

#include "XyahLibraryProjGameModeBase.h"
#include "XyahArrayLibrary.h"
#include "XyahMapLibrary.h"
#include "XyahUtilityLibrary.h"
#include "XyahGameLibrary.h"

void AXyahLibraryProjGameModeBase::BeginPlay()
{
	XYAH(Utility) Print(FXyahPrint(100, true, FLinearColor::Red, false, 0, 100.f, false, 1.f), TEXT("1 Begin Play! %s"), *GetName());
	Super::BeginPlay();
	XYAH(Utility) Print(FXyahPrint(101, true, FLinearColor::Red, false, 0, 100.f, false, 1.f), TEXT("2 Begin Play! %s"), *GetName());
}

TArray<AActor*> AXyahLibraryProjGameModeBase::GetAllActorsThatAreCloseToTheOrigin(TSubclassOf<AActor> ActorClass, float Threshold)
{
	TArray<AActor*> Result;
	XYAH(Game) GetAllActorsOfClass(this, ActorClass, Result, [Threshold](AActor* A)
	{
		if (IsValid(A))
		{
			return A->GetActorLocation().Size() < Threshold;
		}
		return false;
	});
	return Result;
}

bool AXyahLibraryProjGameModeBase::IsAnyNameLongerThan(int32 Length)
{
	return XYAH(Array) AnyIf(TestNames, [Length](const FString& Name) { return Name.Len() > Length; });
}

bool AXyahLibraryProjGameModeBase::AreAllNamesLongerThan(int32 Length)
{
	return XYAH(Array) AllIf(TestNames, [Length](const FString& Name) { return Name.Len() > Length; });
}

FString AXyahLibraryProjGameModeBase::GetRandomName() const
{
	FString RandomElement;
	XYAH(Array) GetRandom(TestNames, &RandomElement);	
	return RandomElement;
}

void AXyahLibraryProjGameModeBase::GetRandomNumber(FString& String, int32& Number) const
{
	XYAH(Map) GetRandom(Numbers, &String, &Number);
}

void AXyahLibraryProjGameModeBase::GetNumberFromIndex(int32 Index, FString& String, int32& Number) const
{
	XYAH(Map) GetFromIndex(Numbers, Index, &String, &Number);
}

void AXyahLibraryProjGameModeBase::ReplaceNumberKey(const FString& OldNumber, const FString& NewNumber)
{
	XYAH(Map) ReplaceKey(Numbers, OldNumber, NewNumber);
}

void AXyahLibraryProjGameModeBase::RemoveIfNumberLessThan(int32 Number)
{
	XYAH(Map) RemoveIf(Numbers, [Number](const FString& Key, const int32& Value) { return Value < Number; });
}

void AXyahLibraryProjGameModeBase::FindIfNumberLessThan(int32 Number, TArray<FString>& OutKeys)
{
	XYAH(Map) FindIf(Numbers, OutKeys, [Number](const FString& Key, const int32& Value) { return Value < Number; });

}

bool AXyahLibraryProjGameModeBase::SafeAddNumber(const FString& String, const int32& Number)
{
	return XYAH(Map) SafeAdd(Numbers, String, Number);
}
