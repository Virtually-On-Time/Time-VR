// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

#include "Elevator.generated.h"

UCLASS()
class TIMEVR_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(float DeltaTime, float Height);

	FVector OriginalPosition;
	int CurrentFloor;

	UPROPERTY(EditAnywhere)
	int MaxFloors;
	
	UPROPERTY(EditAnywhere)
	float DistanceBetweenFloors;
	
	UPROPERTY(EditAnywhere)
	float Speed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void FloorUp();

	UFUNCTION(BlueprintCallable)
	void FloorDown();
};
