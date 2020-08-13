// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "XyahComponentsCore.h"
#include "BillboardSceneComponent.generated.h"


class APlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XYAHLIBRARY_API UBillboardSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBillboardSceneComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Billboard Scene")
	void SetPlayerControllerOwner(APlayerController* PC);
	
	/*
	Updates the Billboard Scale based on the Reference Location and Direction,
	and the Distance at which the Objects would be at Scale 1 (Normal Size).

	@param ReferenceLocation - The Location of the View (in almost all cases the Camera's World Location)
	@param ReferenceRotation - The Rotation of the View (in almost all cases the Camera's World Rotation)
	*/
	UFUNCTION(BlueprintCallable, Category = "Billboard Scene")
	void UpdateBillboard(const FVector& ReferenceLocation, const FRotator& ReferenceRotation);

	UFUNCTION(BlueprintCallable, Category = "Billboard Scene")
	void SetDistanceForScaleOf1(float Distance);


protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboard Scene")
	APlayerController* PlayerControllerOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard Scene")
	float BillboardScale;

	//The Distance (in Centimeters/Unreal Units) that the Billboard Scene must be in so its Scale is 1
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Billboard Scene")
	float DistanceForScaleOf1;

	/*
	The Index of the Player Controller that will be used for this Component. Set at Begin Play. 
	If Index is less than 0, the Reference Player Controller will remain NULL and will need to be set manually.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard Scene")
	int32 PlayerControllerIndex;

	//Whether to perform the Billboard Scaling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard Scene")
	bool bEnableScale;

	//Whether to perform the Billboard Rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard Scene")
	bool bEnableRotation;

	//When performing rotation, whether to only consider the "Yaw" of the Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard Scene")
	bool bOnlyConsiderYaw;


};
