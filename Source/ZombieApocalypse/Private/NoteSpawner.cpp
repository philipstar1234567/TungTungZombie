// Copyright University of Inland Norway


#include "NoteSpawner.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"										// This inclusion and the two below are to start the song from the name.
#include "Sound/SoundBase.h"
#include "UObject/SoftObjectPath.h"

#include "Components/AudioComponent.h"


// Sets default values
ANoteSpawner::ANoteSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	 
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
}

void ANoteSpawner::BeginPlay()
{
	Super::BeginPlay();

	InitialiseSongState();

	//UQuartzSubsystem* Quartz = GEngine->GetEngineSubsystem<UQuartzSubsystem>();
	//if (!Quartz)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Quartz subsystem not found"));
	//	return;
	//}

	//// Prepare Quartz clock settings
	//ClockSettings.BeatsPerMinute = BPM;
	//ClockSettings.TimeSignature.NumBeats = 4;                       // e.g., 4/4 time
	//ClockSettings.TimeSignature.BeatType = EQuartzTimeSignatureQuantization::QuarterNote;

	//// Create Quartz clock
	//Quartz->CreateNewClock(this, FName("SongClock"), ClockSettings);

	//QuartzClockHandle = Quartz->GetHandleForClock(this, FName("SongClock"));
	//if (!QuartzClockHandle)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to get Quartz clock handle"));
	//	return;
	//}

	//// Subscribe to Beat quantization event
	//QuartzClockHandle->SubscribeToQuantizationEvent(
	//	this,
	//	EQuartzCommandQuantization::Beat,
	//	FOnQuartzMetronomeEventBP::CreateUObject(this, &ANoteSpawner::OnQuartzBeat)
	//);

	//// Start song playback quantized to the Quartz clock
	//PlaySongQuantized();
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
	BPM = CurrentRow->BeatsPerMinute;
	NoteMap = CurrentRow->NoteMap;
	bIsThereASongToPlay = true;
	//PlaySong();
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
	FSoftObjectPath SoftPath(SongAssetPath + SongName.ToString() + "." + SongName.ToString());
	USoundBase* SoundAsset = Cast<USoundBase>(SoftPath.TryLoad());

	if (!SoundAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load song named: %s at path: %s"), *SongName.ToString(), *SongAssetPath);
		return;
	}

	UGameplayStatics::SpawnSound2D(this, SoundAsset);
}

//void ANoteSpawner::PlaySongQuantized()
//{
//	if (!QuartzClockHandle) return;
//
//	FSoftObjectPath SoftPath(SongAssetPath + SongName.ToString() + TEXT(".") + SongName.ToString());
//	USoundBase* SoundAsset = Cast<USoundBase>(SoftPath.TryLoad());
//
//	if (!SoundAsset)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Failed to load song named: %s at path: %s"), *SongName.ToString(), *SongAssetPath);
//		return;
//	}
//
//	UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(this, SoundAsset, 1.f, 1.f, 0.f, nullptr, true);
//	if (AudioComp)
//	{
//		AudioComp->PlayQuantized(this, QuartzClockHandle, EQuartzCommandQuantization::Bar, nullptr, 0.f);
//	}
//}
