// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "NoteActor.h"
#include "NoteManager.generated.h"


class UQuartzClockHandle;
class ANoteSpawner;
class ANoteActor;
class ATargetArea;

UCLASS()
class ZOMBIEAPOCALYPSE_API ANoteManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
    // The beatmap data table for the current song.
    // We can assign our CSV-imported Data Table to this in the editor.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm Game")
    UDataTable* Beatmap;

    // The specific Blueprint version of our NoteActor to spawn.
    // This lets us use a Blueprint with visuals, based on our C++ class.
    UPROPERTY(EditAnywhere, Category = "Rhythm Game")
    TSubclassOf<ANoteActor> NoteActorClass;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	TSubclassOf<ATargetArea> HitBoxZoneClass;

	UFUNCTION(BlueprintCallable, Category = "HitBoxes")
	void SpawnHitboxes();

    // A function we can call from our Blueprint to start the song.
    UFUNCTION(BlueprintCallable, Category = "Rhythm Game")
    void StartSong();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm Game")
	TObjectPtr<ANoteSpawner> NoteSpawner;

};
