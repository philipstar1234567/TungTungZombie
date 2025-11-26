// Copyright University of Inland Norway


#include "NoteActor.h"

// #include "TargetArea.h"
#include "Components/BoxComponent.h"

// Sets default values
ANoteActor::ANoteActor()
{
	// Disable ticking for performance
	PrimaryActorTick.bCanEverTick = false;

	// Create the collision box & set it as root
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	// Set box size (adjust as you want)
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	// Setup collision responses and enable overlap events
	// CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->SetGenerateOverlapEvents(true);
	
}

// Called when the game starts or when spawned
// void ANoteActor::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }
