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
			
			HumanModelCount--;
			BittenModelCount++;
			// Breaks out of the loop, so it only does it on one
			break;
		}
	}
	
}

void AMeshSynchronizer::ZombifyBitten()
{
	for (AHumanActor* Actor : ActorArray)
	{
		// Gets the first bitten it finds, and zombifies it
		if (Actor->GetZombieStatus() == EZombieState::Bitten)
		{
			Actor->Zombiefied();
			
			ZombieModelCount++;
			BittenModelCount--;
			// Breaks out of the loop, so it only does it on one
			break;
		}
	}
	
}

void AMeshSynchronizer::CureBitten()
{
	for (AHumanActor* Actor : ActorArray)
	{
		// Gets the first bitten it finds, and cures it
		if (Actor->GetZombieStatus() == EZombieState::Bitten)
		{
			Actor->Cure();
			
			HumanModelCount++;
			BittenModelCount--;
			// Breaks out of the loop, so it only does it on one
			break;
			
		}
	}
	
}

// Called every time the step time updates
void AMeshSynchronizer::UpdateStepTime()
{
	int32 deltaBitten = SimulationController->Bitten - BittenModelCount;
	int32 deltaZombies = SimulationController->Zombies - ZombieModelCount;
	int32 deltaHumans = SimulationController->Susceptible - HumanModelCount;

	// Increases
	if (deltaBitten > 0)
	{
		for (int i = 0; i < deltaBitten; i++)
		{
			BiteHuman();
		}
	}
	else // In an else statement, because both at once here is impossible.
	{
		if (deltaHumans > 0)
		{
			for (int i = 0; i < deltaHumans; i++)
			{
				CureBitten();
			}
		}
	}
	
	if (deltaZombies > 0)
	{
		for (int i = 0; i < deltaZombies; i++)
		{
			ZombifyBitten();
		}
	}
	
	
	
}



