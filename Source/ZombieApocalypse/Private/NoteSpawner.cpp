// Copyright University of Inland Norway


#include "NoteSpawner.h"
#include "Engine/World.h"

// Sets default values
ANoteSpawner::ANoteSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	 
	// Initialize default lane positions (can override in editor)
	LaneLocations.Init(FVector::ZeroVector, 4);
	 
	LaneLocations[0] = FVector(0.f, -300.f, 100.f); // Lane 0
	LaneLocations[1] = FVector(0.f, -100.f, 100.f); // Lane 1
	LaneLocations[2] = FVector(0.f,  100.f, 100.f); // Lane 2
	LaneLocations[3] = FVector(0.f,  300.f, 100.f); // Lane 3
}

void ANoteSpawner::SpawnNoteAtLane(int32 LaneIndex)
{
	if (!NoteActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoteActorClass is not set!"));
		return;
	}
	 
	if (!LaneLocations.IsValidIndex(LaneIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnNoteAtLane: Invalid LaneIndex %d"), LaneIndex);
		return;
	}
	 
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnNoteAtLane: World is null!"));
		return;
	}
	 
	FVector SpawnLocation = LaneLocations[LaneIndex];
	FRotator SpawnRotation = FRotator::ZeroRotator;

	UE_LOG(LogTemp, Error, TEXT("Slapping actor here"));
	World->SpawnActor<AActor>(NoteActorClass, SpawnLocation, SpawnRotation);
}
