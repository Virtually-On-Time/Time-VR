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
	CurrentFloor = 0;
	OriginalPosition = GetActorLocation();
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Move(DeltaTime, OriginalPosition.Z + CurrentFloor * DistanceBetweenFloors);
}

void AElevator::FloorUp()
{
	if (CurrentFloor != MaxFloors)
	{
		CurrentFloor++;
	}
}

void AElevator::FloorDown()
{
	if (CurrentFloor != 0)
	{
		CurrentFloor--;
	}
}

void AElevator::Move(float DeltaTime, float Height)
{
	if (Height != GetActorLocation().Z) {
		SetActorLocation(FVector(OriginalPosition.X, OriginalPosition.Y, FMath::FInterpConstantTo(GetActorLocation().Z, Height, DeltaTime, Speed)));
	}
}