// Copyright University of Inland Norway


#include "MeshSynchronizer.h"
#include "../SimulationController.h"
#include "../HumanActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeshSynchronizer::AMeshSynchronizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	

}

// Called when the game starts or when spawned
void AMeshSynchronizer::BeginPlay()
{
	Super::BeginPlay();
	
	// Calls InitializePointers() after 0.1s
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMeshSynchronizer::InitializePointers, 0.1f, false);
	
}

void AMeshSynchronizer::InitializePointers()
{
	ActorArray.Empty();
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHumanActor::StaticClass(), Actors);

		for (AActor* Actor : Actors)
		{
			AHumanActor* HumanActor = nullptr;
			
			HumanActor = Cast<AHumanActor>(Actor);
			if (HumanActor)
			{
				// Adds humanactor to the array.
				ActorArray.Add(HumanActor);
			}
		}
		
		
	}
	
	// Assigns the simulationcontroller pointer to the simulationcontroller in the scene :thumbsup:
	SimulationController = Cast<ASimulationController>(UGameplayStatics::GetActorOfClass(GetWorld(), ASimulationController::StaticClass()));
	
	// Bites and Zombifies the first human actor because we start with 1 zombie.
	ActorArray[0]->Bitten();
	ActorArray[0]->Zombiefied();
	
	
	if (SimulationController)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMeshSynchronizer::UpdateMeshes, 0.5f, true);
	}
	
}

// Called every half a second to update the meshes (starting from the 0.6th second)
void AMeshSynchronizer::UpdateMeshes()
{
	
	
	
}


// Called every frame
void AMeshSynchronizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

