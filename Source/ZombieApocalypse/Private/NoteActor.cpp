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
	bIsOverlappingHitZone = false;
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
			InputComponent->BindAction("HitNote", IE_Pressed, this, &ANoteActor::OnHitKeyPressed);
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
	// Check if overlapping the hit zone by tag or class
	if (OtherActor && OtherActor->ActorHasTag("HitZone"))
	{
		bIsOverlappingHitZone = true;
	}
}

void ANoteActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("HitZone"))
	{
		bIsOverlappingHitZone = false;
	}
}

void ANoteActor::OnHitKeyPressed()
{
	if (bIsOverlappingHitZone)
	{
		Destroy(); // eliminate the note actor on key press during overlap
	}
}