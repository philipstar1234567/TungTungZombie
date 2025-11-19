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
	UPROPERTY()
	TObjectPtr<UBoxComponent> CollisionBox;

	bool bIsOverlappingHitZone;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnHitKeyPressed();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
