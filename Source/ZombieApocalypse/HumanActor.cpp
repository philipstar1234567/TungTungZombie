// Copyright University of Inland Norway


#include "HumanActor.h"
#include "CoreGlobals.h"



/*
 * Notes to self for later development:
 * 
 * In Bitten(), make it call Zombiefy() after 15 steps.
 */



// Sets default values
AHumanActor::AHumanActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHumanActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHumanActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EZombieState AHumanActor::GetZombieStatus() const
{
	return ZombieStatus;
}

void AHumanActor::Bitten()
{
	if (ZombieStatus != EZombieState::Human)
	{
		UE_LOG(LogTemp, Warning, TEXT("An actor that isn't a human was attempted to be bitten!"));
		return;
	}

	// Starts the bite process
	ZombieStatus = EZombieState::Bitten;
	
	// Change skeletal mesh in BP
	IAmBitten();
	
}

void AHumanActor::Zombiefied()
{
	if (ZombieStatus != EZombieState::Bitten)
	{
		UE_LOG(LogTemp, Warning, TEXT("An actor that isn't even bitten wants to become a zombie >( "));
		return;
	}

	// Change the state formality
	ZombieStatus = EZombieState::Zombie;

	// Make the zombie look the part
	IAmAZombie();
}

void AHumanActor::Cure()
{
	if (ZombieStatus != EZombieState::Bitten)
	{
		UE_LOG(LogTemp, Warning, TEXT("An actor that isn't even bitten wants to be cured >( "));
		return;
	}
	
	ZombieStatus = EZombieState::Human;
	
	// Resets zombification timer
	ZombificationCountdown = 15;
	
	// Changes mesh in BP
	IAmAHuman();
	
}

