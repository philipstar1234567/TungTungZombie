// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetArea.generated.h"

class ANoteActor;

UCLASS()
class ZOMBIEAPOCALYPSE_API ATargetArea : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bIsOverlappingNote;

	ANoteActor* OverlappingNote = nullptr;

	// UPROPERTY()
	// bool bIsOverlappingHitZoneS;
	//
	// UPROPERTY()
	// bool bIsOverlappingHitZoneK;
	//
	// UPROPERTY()
	// bool bIsOverlappingHitZoneL;
	//
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//moved from Balls
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

	 // UFUNCTION()
	 // bool IsOverlappingActorOfClass(TSubclassOf<AActor> ANoteActor);

	//Moved from balls ^
	//Cooking commenced.

public:

	// Sets default values for this actor's properties
	ATargetArea();
	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	
	// The invisible box for detecting notes.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rhythm Game")
	class UBoxComponent* CollisionBox;


	void OnAHitKeyPressed();
	

	void OnSHitKeyPressed();
	

	void OnKHitKeyPressed();
	

	void OnLHitKeyPressed();
};
