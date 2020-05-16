// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"

// Sets default values
AElevator::AElevator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();

	Trigger = GetWorld()->SpawnActor<ATriggerBox>(FVector(this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z + 190), FRotator(0, 0, 0));
	Trigger->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	Trigger->SetActorScale3D(FVector(2.5f, 2.5f, 4.5f));

	Player = GetWorld()->GetFirstPlayerController()->GetPawn();	

	OriginalHeight = this->GetActorLocation().Z;
	OriginalX = this->GetActorLocation().X;
	OriginalY = this->GetActorLocation().Y;

	CurrentFloor = 0;
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: This is not performant, we should use on trigger overlap instead, or simply use control panel

	if (!Trigger)
	{
		return;
	}

	//if (Trigger->IsOverlappingActor(Player))
	//{
		Move(DeltaTime, OriginalHeight + FloorHeight * CurrentFloor);
	//}
}

void AElevator::Move(float DeltaTime, float TargetHeight)
{
	CurrentHeight = this->GetActorLocation().Z;

	this->SetActorLocation(FVector(OriginalX, OriginalY, FMath::FInterpConstantTo(CurrentHeight, TargetHeight, DeltaTime, MoveSpeed)));
}

void AElevator::InputUp()
{
	CurrentFloor++;
}

void AElevator::InputDown()
{
	CurrentFloor--;
}