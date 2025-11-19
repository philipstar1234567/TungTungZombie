// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteSpawner.generated.h"

UCLASS()
class ZOMBIEAPOCALYPSE_API ANoteSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteSpawner();

	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<AActor> NoteActorClass;

	UPROPERTY(EditAnywhere, Category="Spawning")
	TArray<FVector> LaneLocations;
	 
	// Spawn a note actor at a given lane (0-3)
	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SpawnNoteAtLane(int32 LaneIndex);
	
};
