// Copyright University of Inland Norway


#include "MeshSynchronizer.h"
#include "../SimulationController.h"
#include "../HumanActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeshSynchronizer::AMeshSynchronizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	

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
		// Sets the pointer to this properly
		SimulationController->SetMeshSynchronizer(this);
		
	}
	
}

void AMeshSynchronizer::BiteHuman()
{
	for (AHumanActor* Actor : ActorArray)
	{
		// Gets the first human it finds, and bites it.
		if (Actor->GetZombieStatus() == EZombieState::Human)
		{
			Actor->Bitten();
			// Breaks out of the loop, so it only does it on one
			break;
		}
	}
	HumanModelCount--;
	BittenModelCount++;
}

void AMeshSynchronizer::ZombifyBitten()
{
	
	ZombieModelCount++;
	BittenModelCount--;
}

void AMeshSynchronizer::CureBitten()
{
	
	HumanModelCount++;
	BittenModelCount--;
}

// Called every time the step time updates
void AMeshSynchronizer::UpdateStepTime()
{
	// Special first-time check to accomodate for any weird discrepancies.
	if (bFirstTimeRunningStepTime)
	{
		int32 deltaBitten = SimulationController->Bitten;
		for (int32 i = 0; i < deltaBitten; i++)
		{
			BiteHuman();
		}	
		bFirstTimeRunningStepTime = false;
		return;
	}
	
	
	
}



