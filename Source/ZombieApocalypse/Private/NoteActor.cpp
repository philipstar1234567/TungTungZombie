// Copyright University of Inland Norway


#include "NoteActor.h"

#include "TargetArea.h"
#include "Components/BoxComponent.h"

// Sets default values
ANoteActor::ANoteActor()
{
	// Disable ticking for performance
	PrimaryActorTick.bCanEverTick = false;

	// Create the collision box & set it as root
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	// Set box size (adjust as you want)
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	// Setup collision responses and enable overlap events
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->SetGenerateOverlapEvents(true);

	// Initialize overlap flag
	bIsOverlappingHitZoneA = false;
	bIsOverlappingHitZoneS = false;
	bIsOverlappingHitZoneK = false;
	bIsOverlappingHitZoneL = false;
}

// Called when the game starts or when spawned
void ANoteActor::BeginPlay()
{
	Super::BeginPlay();
	/* TODO from Johannes
	- First main issue: Every note, subscribes to the input of every other note. Even if it's irrelevant.
	This will cause bugs if multiple targetareas are close together, since it will check all 4 input areas, even if it is just ball A

	- Second main issue: Inputs by default will be consumed by the last valid actor that subscribed to the input action.
	Meaning that only the final note will get any input at all. Even if that input is not intended for the ball

	- Third issue: Since every note subscribes to the input, the input code will be ran once for every ball. Potentially leading to bugs and performance issues.

	Suggested fix:
	- Move input code from note to the TargetArea. This will fix most of the aforementioned issues.
	- Only subscribe to one input action per target area, this will let you consume input without interfering with the other areas
	- This can also let you create new areas later with different input or mechanics. E.g super power that merges 2 areas into one.
	- Bonus: This will make it much easier to implement cooldowns and grace periods, since you only have to worry about a single actor per input.
*/
	
	// Bind overlap event handlers
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ANoteActor::OnBoxBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ANoteActor::OnBoxEndOverlap);

	// Enable input to receive key presses
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		EnableInput(PC);
		if (InputComponent)
		{
			// TODO: Remember to remove the .bConsumeInput assignment if this is moved into TargetArea. 
			InputComponent->BindAction("HitNoteA", IE_Pressed, this, &ANoteActor::OnAHitKeyPressed).bConsumeInput = false;
			InputComponent->BindAction("HitNoteS", IE_Pressed, this, &ANoteActor::OnSHitKeyPressed).bConsumeInput = false;
			InputComponent->BindAction("HitNoteK", IE_Pressed, this, &ANoteActor::OnKHitKeyPressed).bConsumeInput = false;
			InputComponent->BindAction("HitNoteL", IE_Pressed, this, &ANoteActor::OnLHitKeyPressed).bConsumeInput = false;
		}
	}
}

// Called every frame
void ANoteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANoteActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("COLLISION HAS HAPPENED, WOOHOO"));
	// Check if overlapping the hit zone by tag or class
	if (OtherActor && OtherActor->ActorHasTag("HitZoneA"))
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsOverlapping blabla is set to TRUEE"));
		bIsOverlappingHitZoneA = true;
	}
	else if (OtherActor && OtherActor->ActorHasTag("HitZoneS"))
	{
		bIsOverlappingHitZoneS = true;
	}
	else if (OtherActor && OtherActor->ActorHasTag("HitZoneK"))
	{
		bIsOverlappingHitZoneK = true;
	}
	else if (OtherActor && OtherActor->ActorHasTag("HitZoneL"))
	{
		bIsOverlappingHitZoneL = true;
	}
	else
	{
		return;
	}
}

void ANoteActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("HitZoneA"))
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsOverlapping blabla is set to FALSSEE"));
		bIsOverlappingHitZoneA = false;
	}
	else if (OtherActor && OtherActor->ActorHasTag("HitZoneS"))
	{
		bIsOverlappingHitZoneS = false;
	}
	else if (OtherActor && OtherActor->ActorHasTag("HitZoneK"))
	{
		bIsOverlappingHitZoneK = false;
	}
	else if (OtherActor && OtherActor->ActorHasTag("HitZoneL"))
	{
		bIsOverlappingHitZoneL = false;
	}
	else
	{
		return;
	}
}

void ANoteActor::OnAHitKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Key has been pressed"));
	if (IsOverlappingActorWithTag("HitZoneA"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
		Destroy(); // eliminate the note actor on key press during overlap
	}
}

void ANoteActor::OnSHitKeyPressed()
{
	if (IsOverlappingActorWithTag("HitZoneS"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
		Destroy(); // eliminate the note actor on key press during overlap
	}
}

void ANoteActor::OnKHitKeyPressed()
{
	if (IsOverlappingActorWithTag("HitZoneK"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
		Destroy(); // eliminate the note actor on key press during overlap
	}
}



void ANoteActor::OnLHitKeyPressed()
{
	
	if (IsOverlappingActorWithTag("HitZoneL"))
	{
		// TODO: Write a function that takes in a tag corresponding to a key. Instead of using the bool above, just call the function instead.
		
		
		// loop through actors
		// Check for tag corresponding to this key
		// If it overlaps any with the tag, return true
		
		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
		Destroy(); // eliminate the note actor on key press during overlap
	}
}

bool ANoteActor::IsOverlappingActorWithTag(FName Tag)
{
	if (Tag.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Invalid tag"), *FString(__FUNCTION__));
		return false;
	}
	TArray<AActor*> OverlappedActors;
	CollisionBox->GetOverlappingActors(OverlappedActors, ATargetArea::StaticClass());
	// for (int32 i = 0; i < OverlappedActors.Num(); i++)
	// {
	// 	AActor* OverlappedActor = OverlappedActors[i];
	// 	
	// }
	for(AActor* OverlappedActor : OverlappedActors)
	{
	if (OverlappedActor && OverlappedActor->ActorHasTag(Tag))
		return true;
	}
	return false;
}