// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "Actor.h"
#include "TargetArea.generated.h"

UCLASS()
class ZOMBIEAPOCALYPSE_API ATargetArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// The invisible box for detecting notes.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rhythm Game")
	class UBoxComponent* CollisionBox;

};
