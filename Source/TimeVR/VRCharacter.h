// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/SplineComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "HandController.h"
#include "IHeadMountedDisplayModule.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NavigationSystem.h"
#include "VRCharacter.generated.h"

UCLASS()
class TIMEVR_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnItemPickedUp(EControllerHand Hand, int32 Id);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: // Configuration Parameters

	// Functions

	bool FindDestinationMarker(TArray<FVector>& OutPath, FVector& OutLocation);
	void UpdateDestinationMarker();
	void DrawTeleportPath(const TArray<FVector>& Path);
	void UpdateSpline(const TArray<FVector>& Path);

	void MoveForward(float throttle);
	void MoveRight(float throttle);
	
	void GripLeft() { LeftController->Grip(); }
	void ReleaseLeft() { LeftController->Release(); }
	void GripRight() { RightController->Grip(); }
	void ReleaseRight() { RightController->Release(); }

	void CameraX(float speed);
	void CameraY(float speed);

	void TimeTravel();
	void ResetPlayer();

	void BeginTeleport();
	void DoTeleport();
	void EndTeleport();

	void CameraFade(float FromAlpha, float ToAlpha, bool ShouldHold);

	// Globals

	APlayerController* PlayerController;

	FVector NewTeleportLocation;

	bool bIsTeleporting = false;

	bool bIsFading = false;

	bool bIsInPast = false;

	bool bIsVR;
	

	// References

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class AHandController* LeftController;

	UPROPERTY(VisibleAnywhere)
	class AHandController* RightController;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	class USplineComponent* TeleportPath;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMarker;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* InvalidTeleportMesh;

	UPROPERTY(VisibleAnywhere)
	TArray<class USplineMeshComponent*> ArcMeshObjctPool;

	// Editable

	UPROPERTY(EditAnywhere)
	float TeleportProjectileSpeed = 1000;

	UPROPERTY(EditAnywhere)
	float TeleportSimulationTime = 5;

	UPROPERTY(EditAnywhere)
	float TeleportProjectileRadius = 10;

	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 0.6f;

	UPROPERTY(EditAnywhere)
	float TeleportPauseTime = 0.2f;

	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtent = FVector(100, 100, 100);

	UPROPERTY(EditAnywhere)
	float UnitsBetweenlevels = 4000.0f;


	// Editable by Blueprint

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* TeleportArcMesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* TeleportArcMaterial;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandController> HandControllerClass;
};
