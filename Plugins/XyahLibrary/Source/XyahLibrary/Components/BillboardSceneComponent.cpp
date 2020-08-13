// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.


#include "BillboardSceneComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UBillboardSceneComponent::UBillboardSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	BillboardScale = 1.f;
	DistanceForScaleOf1 = 100.f;
	PlayerControllerOwner = NULL;
	PlayerControllerIndex = 0;

	bEnableRotation = true;
	bEnableScale = true;
	bOnlyConsiderYaw = true;
}


#include "Kismet/GameplayStatics.h"
void UBillboardSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerControllerIndex >= 0)
	{
		SetPlayerControllerOwner(UGameplayStatics::GetPlayerController(this, PlayerControllerIndex));
	}
	// ...
	
}


// Called every frame
void UBillboardSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (PlayerControllerOwner && PlayerControllerOwner->PlayerCameraManager)
	{
		UpdateBillboard(PlayerControllerOwner->PlayerCameraManager->GetCameraLocation()
			, PlayerControllerOwner->PlayerCameraManager->GetCameraRotation());
	}
}

void UBillboardSceneComponent::SetPlayerControllerOwner(APlayerController* PC)
{
	PlayerControllerOwner = PC;
}

#include "Kismet/KismetMathLibrary.h"
void UBillboardSceneComponent::UpdateBillboard(const FVector& ReferenceLocation, const FRotator& ReferenceRotation)
{
	const FQuat ReferenceQuat = ReferenceRotation.Quaternion();
	const FVector RealDisplacement = (GetComponentLocation() - ReferenceLocation);
	const FVector ForwardDisplacement = RealDisplacement.ProjectOnTo(ReferenceQuat.GetForwardVector());

	if (bEnableScale)
		SetWorldScale3D(FVector(BillboardScale * ForwardDisplacement.Size() / DistanceForScaleOf1));

	if (bEnableRotation)
	{
		FRotator Rotation = UKismetMathLibrary::MakeRotFromXZ(-1.f * RealDisplacement, ReferenceQuat.GetUpVector());
		if (bOnlyConsiderYaw)
		{
			Rotation.Roll = 0.f;
			Rotation.Pitch = 0.f;
		}
		SetWorldRotation(Rotation);
	}
}

void UBillboardSceneComponent::SetDistanceForScaleOf1(float Distance)
{
	if (Distance > KINDA_SMALL_NUMBER)
		DistanceForScaleOf1 = Distance;
	else
	{
		XYAH_CMP_LOG(Warning, TEXT("Trying to Set UBillboardSceneComponent::DistanceForScaleOf1 a value too small or zero!"));
	}
}

