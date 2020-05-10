// Fill out your copyright notice in the Description page of Project Settings.


#include "HandController.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "VRCharacter.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AHandController::AHandController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup MotionController
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
}

// Called when the game starts or when spawned
void AHandController::BeginPlay()
{
	Super::BeginPlay();
	
	// Subscribe to overlap events
	OnActorBeginOverlap.AddDynamic(this, &AHandController::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AHandController::ActorEndOverlap);
}

// Called every frame
void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GrabbedComponent) {
		GrabbedComponent->SetWorldLocation(GetActorLocation());
		GrabbedComponent->SetWorldRotation(GetActorRotation());
	}
}

void AHandController::SetHand(FName Hand) {
	MotionController->SetTrackingMotionSource(Hand);
	if (Hand == "Left") {
		MCHand = EControllerHand::Left;
	}
	else {
		MCHand = EControllerHand::Right;
	}

	// Cache owner reference
	Character = Cast<AVRCharacter>(GetOwner());
	if (Character) {

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No owner found!"));
	}
}

void AHandController::Grip()
{
	//if (!bCanPickup) return;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if (HitResult.GetActor()) {
		/*PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			HitResult.Location
		);*/
		if (Character) {
			Character->OnItemPickedUp(MCHand, ComponentToGrab->GetUniqueID());
		}
		GrabbedComponent = ComponentToGrab;
		ComponentToGrab->SetSimulatePhysics(false); // TODO: To improve, set custom physics here
	}
}

void AHandController::Release()
{
	if (GrabbedComponent) {
		GrabbedComponent->SetSimulatePhysics(true);
		GrabbedComponent = nullptr;
	}
}

void AHandController::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) 
{
	UE_LOG(LogTemp, Warning, TEXT("%s hand hit %s"), *GetHandName(), *OverlappedActor->GetName());

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

FString AHandController::GetHandName()
{
	if (MCHand == EControllerHand::Left) {
		return "Left hand";
	}
	else {
		return "Right hand";
	}
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

FHitResult AHandController::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetActorLocation(),
		GetHandReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// See what it hits
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()))
	}

	return Hit;
}

FVector AHandController::GetHandReach() const
{
	FVector ViewPointLocation = GetActorLocation();
	FRotator PlayerViewPointRotation = GetActorRotation();
	float Reach = 5.0f;
	
	return ViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
