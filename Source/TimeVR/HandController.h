// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "MotionControllerComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "InputCoreTypes.h"
#include "HandController.generated.h"

UCLASS()
class TIMEVR_API AHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetHand(FName hand);
	void Grip();
	void Release();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Callbacks

	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// helpers
	bool CanPickup() const;
	bool CanPickupComponent() const;

	FString hand;

	UPROPERTY(VisibleAnywhere)
	EControllerHand MCHand;

	// Default sub object
	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MotionController;

	// Parameters
	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* HapticEffect;

	// State
	bool bCanPickup = false;
};
