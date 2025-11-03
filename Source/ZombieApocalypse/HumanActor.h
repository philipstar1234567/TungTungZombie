// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieStatus.h"
#include "HumanActor.generated.h"



UCLASS()
class ZOMBIEAPOCALYPSE_API AHumanActor : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	AHumanActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, category = "ZombieSim")
	EZombieState ZombieStatus = EZombieState::Human;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Returns the status of the zombie infection in this actor
	EZombieState GetZombieStatus() const;
	// Bites this human
	void Zombify();
	

};
