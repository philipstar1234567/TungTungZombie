// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	bool bIsZombie = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Returns whether this actor is a zombie or not.
	bool GetIsZombie() const;
	// Turns this actor into a zombie.
	void Zombify();
	

};
