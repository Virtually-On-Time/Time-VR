// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundWave.h"
<<<<<<< Updated upstream
#include "Components/AudioComponent.h"
=======
>>>>>>> Stashed changes

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

<<<<<<< Updated upstream
	float CurrentTime;
	void Play();
	void Pause();
	void NextTrack();
	void PrevTrack();

	void AddNewFile();
	bool bIsNewFile;

	UAudioComponent* AudioComponent;
	int CurrentSound;
	int MaxAllowedSound;

	UPROPERTY(EditAnywhere, Category="AudioFiles")
	TArray<USoundWave*> Sounds;
=======
	UPROPERTY(EditAnywhere)
	TArray<USoundWave>* Sounds;
>>>>>>> Stashed changes
};
