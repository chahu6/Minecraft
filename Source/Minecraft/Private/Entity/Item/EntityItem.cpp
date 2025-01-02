// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Item/EntityItem.h"
#include "Components/BoxComponent.h"
#include "Item/Item.h"
#include "Interfaces/InteractiveInterface.h"
#include "Kismet/GameplayStatics.h"

AEntityItem::AEntityItem()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 60.0f;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(TEXT("Item"));
	Box->SetSimulatePhysics(true);
	Box->SetEnableGravity(true);
	Box->BodyInstance.bLockXRotation = true;
	Box->BodyInstance.bLockYRotation = true;
	RootComponent = Box;

	AvatarMesh = CreateDefaultSubobject<USceneComponent>(TEXT("AvatarMesh"));
	AvatarMesh->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(AvatarMesh);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMesh->SetRelativeScale3D(FVector(0.4f));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(AvatarMesh);
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMesh->SetRelativeScale3D(FVector(0.4f));
}

void AEntityItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!ItemStack.IsEmpty())
	{
		const UItem* Item = ItemStack.GetItem();
		if (Item->StaticMesh)
		{
			StaticMesh->SetStaticMesh(Item->StaticMesh);
		}
		else if(Item->SkeletalMesh)
		{
			SkeletalMesh->SetSkeletalMesh(Item->SkeletalMesh);
		}
		else
		{
			StaticMesh->SetStaticMesh(nullptr);
			SkeletalMesh->SetSkeletalMesh(nullptr);
		}
	}
	else
	{
		StaticMesh->SetStaticMesh(nullptr);
		SkeletalMesh->SetSkeletalMesh(nullptr);
	}
}

void AEntityItem::AddImpulse(const FVector& Impulse)
{
	if (Box)
	{
		Box->AddImpulse(Impulse);
	}
}

void AEntityItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		Box->OnComponentBeginOverlap.AddDynamic(this, &AEntityItem::OnOverlapBegin);
		Box->OnComponentEndOverlap.AddDynamic(this, &AEntityItem::OnOverlapEnd);
	}
}

void AEntityItem::BeginPlay()
{
	Super::BeginPlay();
	
	DelayBeforeCanPickupTimer = DelayBeforeCanPickup;
}

void AEntityItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WorldTimer += DeltaTime * FloatSpeed;

	const FVector DeltaLocation(0.0, 0.0, FMath::Sin(WorldTimer) * FloatAmplitude);

	AvatarMesh->SetRelativeLocation(DeltaLocation);
	AvatarMesh->AddRelativeRotation(FRotator(0.0, DeltaTime * RotationSpeed, 0.0));

	if (bIsPickingUp)
	{
		DelayBeforeCanPickupTimer -= DeltaTime;

		if (DelayBeforeCanPickupTimer <= 0)
		{
			AActor* Player = GetNearestPlayer();
			if (Player)
			{
				SetActorLocation(FMath::VInterpTo(GetActorLocation(), Player->GetActorLocation(), DeltaTime, InterpSeepd));
				if (GetActorLocation().Equals(Player->GetActorLocation(), 10.0f))
				{
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, TEXT("EntityItem: Pick Up"));
					
					if (IInteractiveInterface::Execute_OnItemPickup(Player, ItemStack))
					{
						UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
						Destroy();
					}
				}
			}
		}
	}
}

void AEntityItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Îü¸½ÒÆ¶¯Ïà¹Ø
	if (IsValid(OtherActor) && OtherActor->Implements<UInteractiveInterface>())
	{
		if (!Players.Contains(OtherActor))
		{
			Players.Add(OtherActor);
			bIsPickingUp = true;
		}
	}
}

void AEntityItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Players.Contains(OtherActor))
	{
		Players.Remove(OtherActor);
	}

	if (Players.IsEmpty())
	{
		bIsPickingUp = false;
		DelayBeforeCanPickupTimer = DelayBeforeCanPickup;
	}
}

AActor* AEntityItem::GetNearestPlayer()
{
	double NearestDistance = INT_MAX;
	AActor* NearestPlayer = nullptr;

	for (AActor* Player : Players)
	{
		double Distance = FVector::Distance(GetActorLocation(), Player->GetActorLocation());
		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestPlayer = Player;
		}
	}

	return NearestPlayer;
}
