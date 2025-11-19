// Copyright University of Inland Norway


#include "NoteManager.h"
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
	
}

// Called every frame
void ANoteManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
