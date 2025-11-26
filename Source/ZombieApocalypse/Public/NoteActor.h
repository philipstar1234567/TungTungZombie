// Copyright University of Inland Norway

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteActor.generated.h"

class UBoxComponent;


UCLASS()
class ZOMBIEAPOCALYPSE_API ANoteActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteActor();



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> CollisionBox;
	
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;


};
