// Copyright University of Inland Norway


#include "HumanActor.h"

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

void AHumanActor::Zombify()
{
	// Starts the bite process
	ZombieStatus = EZombieState::Bitten;

	// Change skeletal mesh here

	
}

