// Copyright University of Inland Norway


#include "NoteSpawner.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"										// This inclusion and the two below are to start the song from the name.
#include "Sound/SoundBase.h"
#include "UObject/SoftObjectPath.h"
#include "Components/AudioComponent.h"

#include "AudioMixerBlueprintLibrary.h" // Needed for Quartz
#include "Quartz/QuartzSubsystem.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "Sound/QuartzQuantizationUtilities.h"


// Sets default values
ANoteSpawner::ANoteSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	//// Create an Audio Component so we can control it (Pause/Stop/Quartz)
	//AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicComponent"));
	//AudioComp->SetupAttachment(RootComponent);
	//AudioComp->bAutoActivate = false; // Don't play until we tell it to
	//AudioComp->bAllowSpatialization = false;

	// Initialize default lane positions (can override in editor)
	LaneLocations.Init(FVector::ZeroVector, 4);

	LaneLocations[0] = FVector(0.f, -300.f, 100.f); // Lane 0
	LaneLocations[1] = FVector(0.f, -100.f, 100.f); // Lane 1
	LaneLocations[2] = FVector(0.f, 100.f, 100.f); // Lane 2
	LaneLocations[3] = FVector(0.f, 300.f, 100.f); // Lane 3
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

void ANoteSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bIsThereASongToPlay)
	//{
	//	SongPosition += DeltaTime;

	//	int CurrentTick = SongPosition * Resolution * (BPM/1000) / 60.f;

	//	UE_LOG(LogTemp, Warning, TEXT("ANoteSpawner::Tick says: CurrentTick is %d"), CurrentTick);

	//	for (int Tick = LastTick + 1; Tick <= CurrentTick; Tick++)
	//	{
	//		if (NoteMap.Contains(Tick))
	//		{
	//			SpawnNoteAtLane(NoteMap.FindAndRemoveChecked(Tick));
	//		}
	//	}

	//	LastTick = CurrentTick;

	//	if (NoteMap.IsEmpty())
	//	{
	//		bIsThereASongToPlay = false;
	//	}
	//}
	if (AudioComp == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Audio Component pointer is null!");
	}
	
	if (!AudioComp->IsPlaying()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Song finished"));
		bIsSongPlaying = false;
	}

	if (!bIsSongPlaying || !ClockHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("No song || No clock"));
		return;
	}

	// --- THE CORE LOGIC CHANGE ---

	UWorld* World = GetWorld();
	if (!World) return;

	// Instead of adding DeltaTime, we ask Quartz: "How long has the clock been running?"
	// This returns the precise time of the Audio Thread.
	float CurrentAudioTime = ClockHandle->GetEstimatedRunTime(World);

	// Calculate current Tick based on precise Audio Time
	// Formula: Time * (BPM / 60) * Resolution
	// (BPM / 60) = Beats Per Second
	float CurrentBeat = CurrentAudioTime * ((BPM) / 60.0f);
	int32 CurrentTick = FMath::FloorToInt(CurrentBeat * Resolution);

	//UE_LOG(LogTemp, Warning, TEXT("ANoteSpawner::Tick says: CurrentTick is %d"), CurrentTick);
	/*UE_LOG(LogTemp, Warning, TEXT("ANoteSpawner blabla says: IS MY AUDIO COMP WORKING? %d"), AudioComp->IsPlaying());*/

	// Using a loop ensures that if the frame rate drops and we jump 10 ticks,
	// we still spawn all the notes in between.
	if (CurrentTick > LastProcessedTick)
	{
		for (int32 Tick = LastProcessedTick + 1; Tick <= CurrentTick; Tick++)
		{
			if (NoteMap.Contains(Tick))
			{
				SpawnNoteAtLane(NoteMap.FindAndRemoveChecked(Tick));
			}
		}

		LastProcessedTick = CurrentTick;

		if (NoteMap.IsEmpty())
		{
			 bIsSongPlaying = false; 
		}
	}
}

void ANoteSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Create a unique name for this clock so it doesn't conflict with other music
	ClockName = FName(*FString::Printf(TEXT("RhythmClock_%s"), *GetName()));

	InitialiseSongState();
}

void ANoteSpawner::InitialiseSongState()
{
	const FChartFileData* CurrentRow = GetRowByName(SongName);

	if (!CurrentRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANoteSpawner::InitialiseSongState says: Row Name produced no rows."));
		return;
	}

	Resolution = CurrentRow->Resolution;
	BPM = (CurrentRow->BeatsPerMinute)/1000;
	NoteMap = CurrentRow->NoteMap;
	bIsSongPlaying = true;

	LastProcessedTick = -1;

	PlaySong();
	//PlaySongQuantized();
}

FChartFileData* ANoteSpawner::GetRowByName(const FName& RowName) const
{
	if (!SongChart)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable not assigned"));
		return nullptr;
	}

	return SongChart->FindRow<FChartFileData>(RowName, TEXT("GetRowByName"));
}

void ANoteSpawner::PlaySong()
{
	//FSoftObjectPath SoftPath(SongAssetPath + SongName.ToString() + "." + SongName.ToString());
	//USoundBase* SoundAsset = Cast<USoundBase>(SoftPath.TryLoad());

	//if (!SoundAsset)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed to load song named: %s at path: %s"), *SongName.ToString(), *SongAssetPath);
	//	return;
	//}

	//UGameplayStatics::SpawnSound2D(this, SoundAsset);

	//*************************************************************************************************************************************

	// 1. Load the Sound
	FSoftObjectPath SoftPath(SongAssetPath + SongName.ToString() + "." + SongName.ToString());
	USoundBase* SoundAsset = Cast<USoundBase>(SoftPath.TryLoad());

	if (!SoundAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load song: %s"), *SongName.ToString());
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	// 2. CREATE THE COMPONENT DYNAMICALLY
	// We stop using the 'AudioComp' defined in the constructor.
	// We create a new one exactly like SpawnSound2D does.
	if (AudioComp && AudioComp->IsValidLowLevel())
	{
		AudioComp->Stop();
		AudioComp->DestroyComponent(); // Clean up previous attempts
	}

	// This function creates a component configured for 2D UI sound (No 3D attenuation, No silence)
	// It does NOT start playing automatically.
	AudioComp = UGameplayStatics::CreateSound2D(World, SoundAsset, 1.0f, 1.0f, 0.0f, nullptr, true, false);

	if (!AudioComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Create Sound 2D Component"));
		return;
	}

	// 3. Setup Quartz
	UQuartzSubsystem* QuartzSubsystem = UQuartzSubsystem::Get(World);
	if (!QuartzSubsystem) return;

	FQuartzClockSettings ClockSettings;
	ClockSettings.TimeSignature.NumBeats = 4;
	ClockSettings.TimeSignature.BeatType = EQuartzTimeSignatureQuantization::QuarterNote;
	ClockSettings.bIgnoreLevelChange = true;

	ClockHandle = QuartzSubsystem->CreateNewClock(World, ClockName, ClockSettings);

	if (ClockHandle)
	{
		// 4. Set BPM
		FQuartzQuantizationBoundary BPMBoundary;
		BPMBoundary.Quantization = EQuartzCommandQuantization::Bar;
		BPMBoundary.Multiplier = 1.0f;
		BPMBoundary.CountingReferencePoint = EQuarztQuantizationReference::BarRelative;

		FOnQuartzCommandEventBP DummyDelegate;
		ClockHandle->SetBeatsPerMinute(World, BPMBoundary, DummyDelegate, ClockHandle, BPM);

		// 5. Queue the Audio
		// We use "None" to ensure it attaches to the clock INSTANTLY without waiting for a bar.
		FQuartzQuantizationBoundary PlayBoundary;
		PlayBoundary.Quantization = EQuartzCommandQuantization::None;

		AudioComp->PlayQuantized(
			World,
			ClockHandle,
			PlayBoundary,
			DummyDelegate,
			0.f,
			0.f,
			1.f,
			EAudioFaderCurve::Linear
		);

		// 6. Start the Clock
		ClockHandle->StartClock(World, ClockHandle);

		bIsSongPlaying = true;
		UE_LOG(LogTemp, Warning, TEXT("Quartz Clock Started via CreateSound2D."));
	}
}

