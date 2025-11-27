// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HumanActor.generated.h"

// An enum for the current status of the human.
UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Human UMETA(DisplayName = "Human"),
	Bitten UMETA(DisplayName = "Bitten"),
	Zombie UMETA(DisplayName = "Zombie")
};

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
	//Implement in BP to change mesh to zombie
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
	// This human becomes a full fledged zombie
	UFUNCTION(BlueprintCallable, Category = "ZombieSim")
	void Zombiefied();
	// Turns into human if bitten
	UFUNCTION(BlueprintCallable, Category = "ZombieSim")
	void Cure();
	
	
	

};
