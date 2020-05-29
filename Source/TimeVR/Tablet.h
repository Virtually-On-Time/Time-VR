// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"

#include "Tablet.generated.h"

UCLASS()
class TIMEVR_API ATablet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATablet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	float CurrentTime;
	
	UFUNCTION(BlueprintCallable)
	void Play();

	UFUNCTION(BlueprintCallable)
	void Pause();

	UFUNCTION(BlueprintCallable)
	void NextTrack();

	UFUNCTION(BlueprintCallable)
	void PrevTrack();

	UFUNCTION(BlueprintCallable)
	void AddNewFile();

	UPROPERTY(VisibleAnywhere)
	bool bIsNewFile;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere)
	int CurrentSound;
	
	UPROPERTY(VisibleAnywhere)
	int MaxAllowedSound;

	UPROPERTY(EditAnywhere, Category = "AudioFiles")
	TArray<USoundWave*> Sounds;
};
