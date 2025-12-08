// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"	
#include "ZombieApocalypse/SimulationController.h"						// Chart struct declaration
#include "Quartz/QuartzSubsystem.h"										// We now will attempt to bring in Quartz into this.
#include "Quartz/QuartzMetronome.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "Sound/QuartzQuantizationUtilities.h"
#include "Sound/QuartzInterfaces.h"
#include "NoteSpawner.generated.h"

UCLASS()
class ZOMBIEAPOCALYPSE_API ANoteSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANoteSpawner();

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> NoteActorClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<FVector> LaneLocations;

	// Spawn a note actor at a given lane (0-3)
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnNoteAtLane(int32 LaneIndex);

	UPROPERTY(EditAnywhere, Category = "Rhythm")
	class UDataTable* SongChart;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(Category = "Spawning")
	void InitialiseSongState();

	FChartFileData* GetRowByName(const FName& RowName) const;

	/** Songs will only play if their file name matches exactly the one on the data table. */
	UFUNCTION()
	void PlaySong();

	//UFUNCTION()
	//void PlaySongQuantized();

	//UFUNCTION()
	//void OnQuartzBeat(const FQuartzMetronomeDelegateData& EventData);

	UPROPERTY(EditAnywhere, Category = "Rhythm")
	FName SongName;

	UPROPERTY(VisibleAnywhere, Category = "Rhythm")
	float SongPosition;

	UPROPERTY(VisibleAnywhere, Category = "Rhythm")
	float SongDuration;

	UPROPERTY(VisibleAnywhere, Category = "Rhythm")
	TMap<int, int> NoteMap;

	UPROPERTY(VisibleAnywhere, Category = "Rhythm")
	int Resolution;

	UPROPERTY(VisibleAnywhere, Category = "Rhythm")
	int BPM;

	UPROPERTY(VisibleAnywhere, Category = "Rhythm")
	bool bIsThereASongToPlay{ false };

	int LastTick;

	/** This represents the filepath where all our songs SHOULD be at. Ensure your song is in there otherwise it won't be played. */
	UPROPERTY(EditAnywhere, Category = "Rhythm")
	FString SongAssetPath{ "/Game/RythmGameInsideAZombieNightmare/MusicFiles/" };

	// Quartzification

	//// Quartz clock handle for scheduling and events
	//UQuartzClockHandle* QuartzClockHandle = nullptr;
	//FQuartzClockSettings ClockSettings;

	// Track current bar and beat
	int32 CurrentTick = 0;
};
