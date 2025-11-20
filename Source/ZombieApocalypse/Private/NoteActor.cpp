// Copyright University of Inland Norway


#include "NoteActor.h"
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
	
	// Bind overlap event handlers
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ANoteActor::OnBoxBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ANoteActor::OnBoxEndOverlap);

	// Enable input to receive key presses
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		EnableInput(PC);
		if (InputComponent)
		{
			InputComponent->BindAction("HitNoteA", IE_Pressed, this, &ANoteActor::OnAHitKeyPressed);
			InputComponent->BindAction("HitNoteS", IE_Pressed, this, &ANoteActor::OnSHitKeyPressed);
			InputComponent->BindAction("HitNoteK", IE_Pressed, this, &ANoteActor::OnKHitKeyPressed);
			InputComponent->BindAction("HitNoteL", IE_Pressed, this, &ANoteActor::OnLHitKeyPressed);
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
	UE_LOG(LogTemp, Warning, TEXT("COLLISION HAS HAPPENED, WOOHOO"));
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
	if (bIsOverlappingHitZoneA)
	{
		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
		Destroy(); // eliminate the note actor on key press during overlap
	}
}

void ANoteActor::OnSHitKeyPressed()
{
	if (bIsOverlappingHitZoneS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
		Destroy(); // eliminate the note actor on key press during overlap
	}
}

void ANoteActor::OnKHitKeyPressed()
{
	if (bIsOverlappingHitZoneK)
	{
		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
		Destroy(); // eliminate the note actor on key press during overlap
	}
}

void ANoteActor::OnLHitKeyPressed()
{
	if (bIsOverlappingHitZoneL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Key has been pressed AND overlap is on"));
		Destroy(); // eliminate the note actor on key press during overlap
	}
}
