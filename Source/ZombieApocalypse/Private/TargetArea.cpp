// Copyright University of Inland Norway


#include "TargetArea.h"
#include "Components/BoxComponent.h"

// Sets default values
ATargetArea::ATargetArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create and set up the collision box.
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox; // Make the box the root component.
}

// Called when the game starts or when spawned
void ATargetArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


