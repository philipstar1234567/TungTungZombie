// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "Actor.h"
#include "NoteActor.generated.h"

UCLASS()
class ZOMBIEAPOCALYPSE_API ANoteActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
