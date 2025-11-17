#include "NoteManager.h"
#include "Kismet/GameplayStatics.h"

// This is our main setup function.
FQuartzClockHandle ANoteManager::SetupAndStartQuartzClock(const UObject* WorldContextObject)
{
    // Get the master Quartz Subsystem from the engine.
    UQuartzSubsystem* QuartzSubsystem = GEngine->GetEngineSubsystem<UQuartzSubsystem>();

    // Always check if pointers are valid before using them!
    if (!QuartzSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Quartz Subsystem not found!"));
        return FQuartzClockHandle(); // Return an empty/invalid handle on failure.
    }

    if (!MusicTrack)
    {
        UE_LOG(LogTemp, Error, TEXT("MusicTrack is not set in BP_NoteManager!"));
        return FQuartzClockHandle();
    }

    // --- Clock Creation ---
    FQuartzClockSettings ClockSettings;
    ClockSettings.TimeSignature.NumBeats = 4;
    ClockSettings.TimeSignature.BeatType = EQuartzBeatType::Quarter;

    FName ClockName = FName("MusicClock");
    // We must get the subsystem first, THEN tell it to create the clock.
    ActiveClockHandle = QuartzSubsystem->CreateNewClock(WorldContextObject, ClockName, ClockSettings, true);

    // --- Configuration ---
    // You should manually set the BPM of your audio file in the asset details in Unreal.
    const float Bpm = MusicTrack->GetBeatsPerMinute();
    if (Bpm > 0.f)
    {
        QuartzSubsystem->SetBeatsPerMinute(WorldContextObject, ActiveClockHandle, Bpm);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not find BPM on MusicTrack asset. Defaulting. Please set it in the asset details."));
    }

    // --- Start Clock and Music ---
    QuartzSubsystem->StartClock(WorldContextObject, ActiveClockHandle);

    // This special function plays a sound perfectly synced to a clock.
    UQuartzClockHandle* ClockHandlePtr = &ActiveClockHandle;
    QuartzSubsystem->StartPlayingFromClock(WorldContextObject, ClockHandlePtr, MusicTrack, FQuartzQuantizationBoundary());

    UE_LOG(LogTemp, Log, TEXT("Quartz Clock '%s' created and started in C++."), *ClockName.ToString());

    // Give the clock handle back to Blueprint so it can schedule events.
    return ActiveClockHandle;
}

// Your old StartSong function.
void ANoteManager::StartSong()
{
    if (!Beatmap || !NoteActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("NoteManager is missing Beatmap or NoteActorClass!"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("StartSong called! Blueprint will now take over spawning loop."));
}