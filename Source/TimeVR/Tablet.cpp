// Fill out your copyright notice in the Description page of Project Settings.


#include "Tablet.h"

// Sets default values
ATablet::ATablet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATablet::BeginPlay()
{
	Super::BeginPlay();
	
	AudioComponent = FindComponentByClass<UAudioComponent>();
	CurrentSound = 0;
	MaxAllowedSound = 0;
	CurrentTime = 0.f;
	AudioComponent->SetSound(Sounds[CurrentSound]);
}

// Called every frame
void ATablet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsNewFile)
	{
		// blinking code goes here
	}
}

void ATablet::Play()
{
	AudioComponent->Play(CurrentTime);
}

void ATablet::Pause()
{
	CurrentTime = GetWorld()->GetTimeSeconds() - AudioComponent->TimeAudioComponentPlayed;
	AudioComponent->Stop();
}

void ATablet::NextTrack()
{
	CurrentTime = 0;
	CurrentSound++;
	if (CurrentSound == MaxAllowedSound)
	{
		bIsNewFile = false;
	}
	if (CurrentSound > MaxAllowedSound)
	{
		CurrentSound = 0;
	}
	AudioComponent->SetSound(Sounds[CurrentSound]);
}

void ATablet::PrevTrack()
{
	CurrentTime = 0;
	CurrentSound--;
	if (CurrentSound < 0)
	{
		CurrentSound = MaxAllowedSound;
	}
	AudioComponent->SetSound(Sounds[CurrentSound]);
}

void ATablet::AddNewFile()
{
	bIsNewFile = true;
	MaxAllowedSound++;
	if (MaxAllowedSound == Sounds.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot add new file, exceeding array size!"));
	}
}