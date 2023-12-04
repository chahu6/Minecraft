#include "Item.h"
#include "Components/BoxComponent.h"
#include "Minecraft/Interfaces/ItemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator::ZeroRotator);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector DeltaLocation(0.0, 0.0, FMath::Sin(GetWorld()->GetTimeSeconds() * DeltaTime * FloatSpeed) * FloatAmplitude);

	MeshComponent->SetRelativeLocation(DeltaLocation);
	MeshComponent->AddRelativeRotation(FRotator(0.0, DeltaTime * RotationSpeed, 0.0));
}

void AItem::SetItemDetails(const FItemDetails& NewItemDetails)
{
	this->ID = NewItemDetails.ID;
	MeshComponent->SetStaticMesh(NewItemDetails.Mesh);
}

FItemDetails AItem::GetItemDetails() const
{
	FItemDetails ItemDetails;
	ItemDetails.ID = this->ID;
	ItemDetails.Mesh = MeshComponent->GetStaticMesh();

	return ItemDetails;
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UClass* ActorClass = OtherActor->GetClass();

	if (ActorClass->ImplementsInterface(UItemInterface::StaticClass()))
	{
		IItemInterface* InterfaceIns = CastChecked<IItemInterface>(OtherActor);

		if (InterfaceIns->AddItem(ID, Count))
		{
			if (PickupSound)
			{
				UGameplayStatics::PlaySound2D(this, PickupSound);
			}
			Destroy();
		}
	}

}

