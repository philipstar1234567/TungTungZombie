// Copyright University of Inland Norway


#include "NoteManager.h"
#include "NoteSpawner.h"
#include "NoteActor.h"
#include "TargetArea.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANoteManager::ANoteManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANoteManager::BeginPlay()
{
	Super::BeginPlay();
	
    SpawnHitboxes();
}

// Called every frame
void ANoteManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANoteManager::SpawnHitboxes()
{
    UE_LOG(LogTemp, Error, TEXT("Attempting to spawn hitzones..."));

    if (!HitBoxZoneClass) 
    {
        UE_LOG(LogTemp, Error, TEXT("HitBoxClass is not defined."));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get world."));
        return;
    }
    AActor* HitBoxA = World->SpawnActor<AActor>(HitBoxZoneClass, FVector(0, -310, 1300), FRotator::ZeroRotator);
    if (HitBoxA)
    {
        HitBoxA->Tags.AddUnique(FName(TEXT("HitZoneA")));
    }
    AActor* HitBoxS = World->SpawnActor<AActor>(HitBoxZoneClass, FVector(0, -110, 1300), FRotator::ZeroRotator);
    if (HitBoxS)
    {
        HitBoxS->Tags.AddUnique(FName(TEXT("HitZoneS")));
    }
    AActor* HitBoxK = World->SpawnActor<AActor>(HitBoxZoneClass, FVector(0, 110, 1300), FRotator::ZeroRotator);
    if (HitBoxK)
    {
        HitBoxK->Tags.AddUnique(FName(TEXT("HitZoneK")));
    }
    AActor* HitBoxL = World->SpawnActor<AActor>(HitBoxZoneClass, FVector(0, 310, 1300), FRotator::ZeroRotator);
    if (HitBoxL)
    {
        HitBoxL->Tags.AddUnique(FName(TEXT("HitZoneL")));
    }
}

void ANoteManager::StartSong()
{
    if (!Beatmap || !NoteActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("NoteManager is missing Beatmap or NoteActorClass!"));
        return;
    }

    // We will add the timer logic here later in Blueprints for simplicity,
    // but you could also do it here in C++.
    // For now, let's just log a message.
    UE_LOG(LogTemp, Log, TEXT("StartSong function called!"));

    // You would also play the music here.
    // UGameplayStatics::PlaySound2D(this, YourSoundWave);
}
