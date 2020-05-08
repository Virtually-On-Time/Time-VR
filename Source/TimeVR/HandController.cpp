// Fill out your copyright notice in the Description page of Project Settings.


#include "HandController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AHandController::AHandController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
}

// Called when the game starts or when spawned
void AHandController::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AHandController::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AHandController::ActorEndOverlap);
}

// Called every frame
void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHandController::SetHand(FName Hand) {
	MotionController->SetTrackingMotionSource(Hand);
	if (Hand == "Left") {
		MCHand = EControllerHand::Left;
		hand = "Left hand";
	}
	else {
		MCHand = EControllerHand::Right;
		hand = "Right hand";
	}
}

void AHandController::Grip()
{
	if (!bCanPickup) return;

	UE_LOG(LogTemp, Warning, TEXT("GRABBING!"));
}

void AHandController::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("RELEASING!"));
}

void AHandController::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) 
{
	UE_LOG(LogTemp, Warning, TEXT("%s hand hit %s"), *hand, *OverlappedActor->GetName());

	bool bNewCanPickup = CanPickup();
	
	if (!bCanPickup && bNewCanPickup) {

		APawn* Pawn = Cast<APawn>(GetAttachParentActor());
		if (Pawn != nullptr) {

			APlayerController* Controller = Cast<APlayerController>(Pawn->GetController());
			if (Pawn != nullptr) {
				Controller->PlayHapticEffect(HapticEffect, MCHand);
			}
		}
	}
	bCanPickup = bNewCanPickup;
}

void AHandController::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	bCanPickup = CanPickup();
}

bool AHandController::CanPickup() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors) {
		if (OverlappingActor->ActorHasTag(TEXT("Pickup"))) {
			return true;
		}
	}
	return false;
}
