// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "IHeadMountedDisplay.h"
#include "HandController.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsVR = IHeadMountedDisplayModule::IsAvailable();
	
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	if (bIsVR) {

		InvalidTeleportMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvalidTeleportMesh"));
		InvalidTeleportMesh->SetupAttachment(VRRoot);

		TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("TeleportPath"));
		TeleportPath->SetupAttachment(VRRoot);

		DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestinationMarker"));
		DestinationMarker->SetupAttachment(GetRootComponent());
	}
	else {
		// Hide the VR stuff
		//VRRoot->SetVisibility(false, true);
	}
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setup global references
	PlayerController = Cast<APlayerController>(GetController());

	// Set up motion controllers
	if (bIsVR) {
		DestinationMarker->SetVisibility(false);

		LeftController = GetWorld()->SpawnActor<AHandController>(HandControllerClass);
		if (LeftController != nullptr) {
			LeftController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
			LeftController->SetOwner(this);
			LeftController->SetHand(FXRMotionControllerBase::LeftHandSourceId);
		}

		RightController = GetWorld()->SpawnActor<AHandController>(HandControllerClass);
		if (RightController != nullptr) {
			RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
			RightController->SetOwner(this);
			RightController->SetHand(FXRMotionControllerBase::RightHandSourceId);
		}
	}
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// The following logic is only required for VR
	if (!bIsVR) return;

	// Calculate camera (player) movement in playspace
	FVector CameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	CameraOffset.Z = 0;

	// Move Actor to that position.
	AddActorWorldOffset(CameraOffset);


	// Move VRRoot in opposite Vector
	VRRoot->AddWorldOffset(-CameraOffset);

	UpdateDestinationMarker();
}

bool AVRCharacter::FindDestinationMarker(TArray<FVector>& OutPath, FVector& OutLocation)
{
	FVector Start = RightController->GetActorLocation();
	FVector LookVector;

	LookVector = RightController->GetActorForwardVector();
	
	/*
		// Project in a straight line to find location

		FVector End = Start + LookVector * MaxTeleportDistance;
		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Emerald, false, -1, 0, 10);
	*/

	// Draw parabolic arc to teleport destination
	FPredictProjectilePathParams Params(TeleportProjectileRadius, Start, LookVector * TeleportProjectileSpeed, TeleportSimulationTime, ECollisionChannel::ECC_Visibility, this);
	Params.bTraceComplex = true; // to fix buggy placement

	FPredictProjectilePathResult PredictResult;
	bool bHit = UGameplayStatics::PredictProjectilePath(this, Params, PredictResult);

	for (FPredictProjectilePathPointData Data : PredictResult.PathData) {
		OutPath.Add(Data.Location);
	}

	if (!bHit) return false;

	FVector Out;
	FNavLocation NavLocation;
	bool bOnNavMesh = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(PredictResult.HitResult.Location, NavLocation, TeleportProjectionExtent);

	if (!bOnNavMesh) return false;

	OutLocation = NavLocation.Location;

	return true;
}

void AVRCharacter::UpdateDestinationMarker()
{
	if (!bIsVR)
	{
		DestinationMarker->SetVisibility(false);
	}

	FVector OutLocation;
	TArray<FVector> Path;
	bool IsAllowedToTeleport = FindDestinationMarker(Path, OutLocation);

	if (IsAllowedToTeleport) {
		DestinationMarker->SetVisibility(true);

		if (bIsFading) {
			DestinationMarker->SetWorldLocation(NewTeleportLocation);
		}
		else {
			DestinationMarker->SetWorldLocation(OutLocation);
		}
		DrawTeleportPath(Path);
		InvalidTeleportMesh->SetVisibility(false);
	}
	else {
		DestinationMarker->SetVisibility(false);

		TArray <FVector>EmptyPath;
		DrawTeleportPath(EmptyPath);
		InvalidTeleportMesh->SetVisibility(true);
	};
}

void AVRCharacter::DrawTeleportPath(const TArray<FVector>& Path)
{
	UpdateSpline(Path);

	// hide teleport arc paths
	for (USplineMeshComponent* SplineMesh : ArcMeshObjctPool) {
		SplineMesh->SetVisibility(false);
	}

	// Draw path
	int32 ArcSegmentCount = Path.Num() - 1;
	for (int32 i = 0; i < ArcSegmentCount; i++)
	{
		USplineMeshComponent* SplineMesh;

		// Allocate and draw meshes
		if (i >= ArcMeshObjctPool.Num()) {
			// Create new mesh for pool
			SplineMesh = NewObject<USplineMeshComponent>(this);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->AttachToComponent(TeleportPath, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->SetStaticMesh(TeleportArcMesh);
			SplineMesh->SetMaterial(0, TeleportArcMaterial);
			SplineMesh->RegisterComponent();
			ArcMeshObjctPool.Add(SplineMesh);
		}
		else {
			SplineMesh = ArcMeshObjctPool[i];
		}

		// Get arc path tangent
		FVector StartLocation, EndLocation, StartTangent, EndTangent;
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i, StartLocation, StartTangent);
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i + 1, EndLocation, EndTangent);

		// Pass to spline mesh
		SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
		SplineMesh->SetVisibility(true);
	}
}

void AVRCharacter::UpdateSpline(const TArray<FVector>& Path)
{
	TeleportPath->ClearSplinePoints(false);

	for (int32 i = 0; i < Path.Num(); i++)
	{
		FVector LocalPosition = TeleportPath->GetComponentTransform().InverseTransformPosition(Path[i]);
		TeleportPath->AddPoint(FSplinePoint(i, LocalPosition, ESplinePointType::Curve));
	}
	TeleportPath->UpdateSpline();
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// VR Controls
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Released, this, &AVRCharacter::BeginTeleport);
	PlayerInputComponent->BindAction(TEXT("TimeTravel"), IE_Released, this, &AVRCharacter::TimeTravel);
	PlayerInputComponent->BindAction(TEXT("ResetPlayer"), IE_Released, this, &AVRCharacter::ResetPlayer);
	
	// Non-VR Controls
	PlayerInputComponent->BindAxis(TEXT("MouseX"), this, &AVRCharacter::CameraX);
	PlayerInputComponent->BindAxis(TEXT("MouseY"), this, &AVRCharacter::CameraY);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AVRCharacter::MoveRight);
}

void AVRCharacter::MoveForward(float throttle)
{
	AddMovementInput(throttle * Camera->GetForwardVector());
}

void AVRCharacter::MoveRight(float throttle)
{
	AddMovementInput(throttle * Camera->GetRightVector());
}

void AVRCharacter::CameraX(float speed)
{
	FRotator Rotation = FRotator(0, speed, 0);
	Camera->AddRelativeRotation(Rotation);
}

void AVRCharacter::CameraY(float speed)
{
	FRotator Rotation = FRotator(speed, 0, 0);
	Camera->AddRelativeRotation(Rotation);
}

// Switch between the levels to simlulate time travel
void AVRCharacter::TimeTravel()
{
	FVector currentLocation = GetActorLocation();
	if (bIsInPast) {
		currentLocation.Y -= UnitsBetweenlevels;
	}
	else {
		currentLocation.Y += UnitsBetweenlevels;
	}
	SetActorLocation(currentLocation);
	bIsInPast = !bIsInPast;
}

// Reset the play location (in case you fall off the map)
void AVRCharacter::ResetPlayer()
{
	SetActorLocation(FVector(0.0, 0.0, 112.0));
	bIsInPast = false;
}

void AVRCharacter::BeginTeleport()
{
	if (bIsTeleporting == false && PlayerController != nullptr && DestinationMarker->IsVisible()) {

		// Set Teleport status
		bIsTeleporting = true;
		bIsFading = true;
		NewTeleportLocation = DestinationMarker->GetComponentLocation();

		CameraFade(0, 1, true);

		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &AVRCharacter::DoTeleport, TeleportFadeTime, false);
	}
}

void AVRCharacter::DoTeleport()
{
	// Get Marker location
	FVector TeleportMarkerLocation = DestinationMarker->GetComponentLocation();

	// Set correct player height/location
	TeleportMarkerLocation += GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * GetActorUpVector();
	SetActorLocation(TeleportMarkerLocation);

	// Re-show destination marker
	bIsFading = false;

	// Hold dark screen for a moment
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AVRCharacter::EndTeleport, TeleportPauseTime, false);
}

void AVRCharacter::EndTeleport()
{
	// Fade back in
	CameraFade(1, 0, false);

	// Allow user to teleport as fade in occurs
	bIsTeleporting = false;
}

void AVRCharacter::CameraFade(float FromAlpha, float ToAlpha, bool ShouldHold)
{
	if (PlayerController != nullptr)
	{
		PlayerController->PlayerCameraManager->StartCameraFade(FromAlpha, ToAlpha, TeleportFadeTime, FLinearColor::Black, true, ShouldHold);
	}
}

