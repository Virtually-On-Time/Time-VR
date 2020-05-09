// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "InputCoreTypes.h"
#include "MotionControllerComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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
	// Default sub object
	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MotionController;

	// Parameters
	UPROPERTY(EditDefaultsOnly)
	class UHapticFeedbackEffect_Base* HapticEffect;

	class UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY(VisibleAnywhere)
	EControllerHand MCHand;

	// Callbacks

	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// Helpers
	FString GetHandName();
	bool CanPickup() const;
	void FindPhysicsHandle();

	// Return first actor with physics
	FHitResult GetFirstPhysicsBodyInReach() const;

	// Return the LineTraceEnd
	FVector GetHandReach() const;

	// State
	bool bCanPickup = false;
};
