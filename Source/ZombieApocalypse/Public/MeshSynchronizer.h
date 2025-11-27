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
	
	// Timerhandle for the checks
	FTimerHandle TimerHandle;
	
	// Sets the values of the pointers properly. Called after a slight delay to prevent errors.
	void InitializePointers();
	
	// Array of all AHumanActors in the scene
	UPROPERTY()
	TArray<AHumanActor*> ActorArray;

	// Pointer to the simulationcontroller
	UPROPERTY()
	TObjectPtr<ASimulationController> SimulationController;
	
	UFUNCTION()
	void UpdateMeshes();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
