// Copyright University of Inland Norway


#include "TargetArea.h"
#include "NoteActor.h"
#include "Components/BoxComponent.h"
#include "Engine/Note.h"
#include "SimulationController.h"

// Sets default values
ATargetArea::ATargetArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Create and set up the collision box.
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->SetGenerateOverlapEvents(true);
}


// Called when the game starts or when spawned
void ATargetArea::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATargetArea::OnBoxBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ATargetArea::OnBoxEndOverlap);


	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		EnableInput(PC);
		if (InputComponent)
		{
			//Let's try binding the input to the target area ONLY if it has the right tag. What could go wrong?
			
			if (this->ActorHasTag("HitZoneA"))
				InputComponent->BindAction("HitNoteA", IE_Pressed, this, &ATargetArea::OnHitKeyPressed);
			
			if (this->ActorHasTag("HitZoneS"))
			InputComponent->BindAction("HitNoteS", IE_Pressed, this, &ATargetArea::OnHitKeyPressed);
			
			if (this->ActorHasTag("HitZoneK"))
			InputComponent->BindAction("HitNoteK", IE_Pressed, this, &ATargetArea::OnHitKeyPressed);
			
			if (this->ActorHasTag("HitZoneL"))
			InputComponent->BindAction("HitNoteL", IE_Pressed, this, &ATargetArea::OnHitKeyPressed);
			
		}
	}
}

// Called every frame (OH NO YOU DON'T)
// void ATargetArea::Tick(float DeltaTime)
//{
// 	Super::Tick(DeltaTime);
//
// }


//@TODO: CHANGE VARIABLES

void ATargetArea::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("COLLISION HAS HAPPENED, WOOHOO"));
	// Check if overlapping the hit zone by tag or class
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(ANoteActor::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("bIsOverlapping blabla is set to TRUEE"));
			bIsOverlappingNote = true;
			OverlappingNote = Cast<ANoteActor>(OtherActor);
			

			
			
		}
	}
	// else if (OtherActor && OtherActor->ActorHasTag("HitZoneS"))
	// {
	// 	bIsOverlappingHitZoneS = true;
	// }
	// else if (OtherActor && OtherActor->ActorHasTag("HitZoneK"))
	// {
	// 	bIsOverlappingHitZoneK = true;
	// }
	// else if (OtherActor && OtherActor->ActorHasTag("HitZoneL"))
	// {
	// 	bIsOverlappingHitZoneL = true;
	// }
	// else
	// {
	// 	return;
	// }
}

//@TODO: CHANGE THIS TOO

void ATargetArea::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(ANoteActor::StaticClass()))
		
		{
			UE_LOG(LogTemp, Warning, TEXT("bIsOverlapping blabla is set to FALSSEE"));
			bIsOverlappingNote = false;
		}
	}
	// else if (OtherActor && OtherActor->ActorHasTag("HitZoneS"))
	// {
	// 	bIsOverlappingHitZoneS = false;
	// }
	// else if (OtherActor && OtherActor->ActorHasTag("HitZoneK"))
	// {
	// 	bIsOverlappingHitZoneK = false;
	// }
	// else if (OtherActor && OtherActor->ActorHasTag("HitZoneL"))
	// {
	// 	bIsOverlappingHitZoneL = false;
	// }
	// else
	// {
	// 	return;
	// }
}


void ATargetArea::OnHitKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Key has been pressed"));
	if (!OverlappingNote) return;
	if (bIsOverlappingNote)
	{
		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
		OverlappingNote->Destroy();
		// eliminate the note actor on key press during overlap
		if (!TheZombieSituation) return;
		if (TheZombieSituation->Bitten>0)
		{
			TheZombieSituation->Bitten-=4*TheZombieSituation->graph_lookup(((TheZombieSituation->Bitten+TheZombieSituation->Susceptible)/TheZombieSituation->land_area)/TheZombieSituation->normal_population_density);
			TheZombieSituation->Susceptible+=4*TheZombieSituation->graph_lookup(((TheZombieSituation->Bitten+TheZombieSituation->Susceptible)/TheZombieSituation->land_area)/TheZombieSituation->normal_population_density);
		}
		if (TheZombieSituation->Bitten<=0)
		{
			
			TheZombieSituation->Zombies-=2*TheZombieSituation->graph_lookup(((TheZombieSituation->Bitten+TheZombieSituation->Susceptible)/TheZombieSituation->land_area)/TheZombieSituation->normal_population_density);
			
			TheZombieSituation->Susceptible+=2*TheZombieSituation->graph_lookup(((TheZombieSituation->Bitten+TheZombieSituation->Susceptible)/TheZombieSituation->land_area)/TheZombieSituation->normal_population_density);
		}

	}
}

// void ATargetArea::OnSHitKeyPressed()
// {
// 	if (!OverlappingNote) return;
// 	if (bIsOverlappingNote)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
// 		OverlappingNote->Destroy();
// 		
// 		// eliminate the note actor on key press during overlap
// 	}
// }
//
// void ATargetArea::OnKHitKeyPressed()
// {
// 	if (!OverlappingNote) return;
// 	if (bIsOverlappingNote)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
// 		OverlappingNote->Destroy();
// 		
// 		// eliminate the note actor on key press during overlap
// 	}
// }
//
// void ATargetArea::OnLHitKeyPressed()
// {
// 	
// 	if (!OverlappingNote) return;
// 	if (bIsOverlappingNote)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
// 		OverlappingNote->Destroy();
// 		
// 		// eliminate the note actor on key press during overlap
// 	}
// }





//@TODO: THIS WILL EITHER BE MODIFIED OR ERASED FROM EXISTENCE DUE TO THERE BEING THERE WAYS TO DO IT

// bool ATargetArea::IsOverlappingActorOfClass(TSubclassOf<AActor> ANoteActor)
// {
// 	if (this->Tags.IsEmpty())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("%s Invalid class"), *FString(__FUNCTION__));
// 		return false;
// 	}
// 	TArray<AActor*> OverlappedActors;
// 	CollisionBox->GetOverlappingActors(OverlappedActors, ANoteActor::StaticClass());
// 	for (int32 i = 0; i < OverlappedActors.Num(); i++)
// 	{
// 		AActor* OverlappedActor = OverlappedActors[i];
// 		
// 	}
// 	for(AActor* OverlappedActor : OverlappedActors)
// 	{
// 		if (OverlappedActor && OverlappedActor->IsA(ANoteActor::StaticClass()))
// 			return true;
// 	}
// 	return false;
// }