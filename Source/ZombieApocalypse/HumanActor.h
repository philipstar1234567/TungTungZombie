// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieStatus.h" // For the enum values
#include "HumanActor.generated.h"



UCLASS()
class ZOMBIEAPOCALYPSE_API AHumanActor : public AActor
{
	GENERATED_BODY()
	FTimerHandle TimerHandle;
	
public:
	
	// Sets default values for this actor's properties
	AHumanActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "ZombieSim")
	EZombieState ZombieStatus = EZombieState::Human;

	//Implement in BP to change mesh to bitten
	UFUNCTION(BlueprintImplementableEvent, Category = "ZombieSim")
	void IAmBitten();
	//Implement in BP to change mesh to zombie
	UFUNCTION(BlueprintImplementableEvent, Category = "ZombieSim")
	void IAmAZombie();
	//Implement in BP to change mesh to human
	UFUNCTION(BlueprintImplementableEvent, Category = "ZombieSim")
	void IAmAHuman();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Returns the status of the zombie infection in this actor
	EZombieState GetZombieStatus() const;
	// Bites this human
	UFUNCTION(BlueprintCallable, Category = "ZombieSim")
	void Bitten();
	// Turns this actor into a zombie if it is bitten
	UFUNCTION(BlueprintCallable, Category = "ZombieSim")
	void Zombify();
	// Turns this actor into a human if it is bitten (but not zombie)
	UFUNCTION(BlueprintCallable, Category = "ZombieSim")
	void Cure();
	
	

};
