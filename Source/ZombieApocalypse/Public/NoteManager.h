#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "NoteActor.h"
#include "Quartz/QuartzSubsystem.h" // Important for audio sync
#include "Sound/SoundBase.h"       // Important for the music track

#include "NoteManager.generated.h"

UCLASS()
class ZOMBIEAPOCALYPSE_API ANoteManager : public AActor
{
    GENERATED_BODY()

public:
    // This is the Blueprint version of our Note that we will spawn.
    // We set this in the editor.
    UPROPERTY(EditAnywhere, Category = "Rhythm Game")
    TSubclassOf<ANoteActor> NoteActorClass;

    // This is the Beatmap Data Table for the current song.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm Game")
    UDataTable* Beatmap;

    // This is the music track that will be played.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm Game")
    USoundBase* MusicTrack;

private:
    // A private variable to hold a reference to our active Quartz clock.
    UPROPERTY()
    FQuartzClockHandle ActiveClockHandle;

public:
    // A C++ function that Blueprints can call to do the complex clock setup.
    UFUNCTION(BlueprintCallable, Category = "Rhythm Game", meta = (WorldContext = "WorldContextObject"))
    FQuartzClockHandle SetupAndStartQuartzClock(const UObject* WorldContextObject);

    // This function will be called from Blueprint to kick off the spawning loop.
    UFUNCTION(BlueprintCallable, Category = "Rhythm Game")
    void StartSong();
};
