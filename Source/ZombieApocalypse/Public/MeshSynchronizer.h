// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshSynchronizer.generated.h"

// Forward declaration
class AHumanActor;
class ASimulationController;

UCLASS()
class ZOMBIEAPOCALYPSE_API AMeshSynchronizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshSynchronizer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Count of how many actors actually have each mesh
	int32 ZombieModelCount = 1;
	int32 BittenModelCount = 0;
	int32 HumanModelCount = 100;
	
	
	FTimerHandle TimerHandle;
	
	bool bFirstTimeRunningStepTime = true;
	
	// Sets the values of the pointers properly. Called after a slight delay to prevent errors.
	void InitializePointers();
	
	// Array of all AHumanActors in the scene
	UPROPERTY()
	TArray<AHumanActor*> ActorArray;

	// Pointer to the simulationcontroller
	UPROPERTY()
	TObjectPtr<ASimulationController> SimulationController;
	
	// Functions to be called in steptime to call a function in an actor
	void BiteHuman();
	void ZombifyBitten();
	void CureBitten();
	
	
public:
	// Please only call this from Simulationcontroller.cpp
	UFUNCTION()
	void UpdateStepTime();

};
