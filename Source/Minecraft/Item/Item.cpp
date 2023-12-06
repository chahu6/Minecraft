﻿#include "Item.h"
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

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Minecraft/Assets/Sound/Pickup_Cue.Pickup_Cue'"));
	if (SoundCueObject.Succeeded())
	{
		PickupSound = SoundCueObject.Object;
	}
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

void AItem::SetItemData(const FItemDetails* ItemDetails, int32 Quantity)
{
	ItemData.ID = ItemDetails->ID;
	ItemData.MaxCount = ItemDetails->MaxCount;
	ItemData.Count = Quantity;
	MeshComponent->SetStaticMesh(ItemDetails->Mesh);
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UClass* ActorClass = OtherActor->GetClass();

	if (ActorClass->ImplementsInterface(UItemInterface::StaticClass()))
	{
		IItemInterface* InterfaceIns = CastChecked<IItemInterface>(OtherActor);

		if (InterfaceIns->AddItem(this))
		{
			if (PickupSound)
			{
				UGameplayStatics::PlaySound2D(this, PickupSound);
			}
			Destroy();
		}
	}
}

