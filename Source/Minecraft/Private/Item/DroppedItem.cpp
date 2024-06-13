#include "Item/DroppedItem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "World/MinecraftSettings.h"
#include "Item/Item.h"
#include "Interfaces/InventoryInterface.h"

ADroppedItem::ADroppedItem()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 5.0f;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetCollisionProfileName(TEXT("Item"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComponent->SetRelativeScale3D(FVector(0.4f));
}

void ADroppedItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ADroppedItem::OnOverlapBegin);
}

void ADroppedItem::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator::ZeroRotator);
}

void ADroppedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector DeltaLocation(0.0, 0.0, FMath::Sin(GetWorld()->GetTimeSeconds() * DeltaTime * FloatSpeed) * FloatAmplitude);

	MeshComponent->SetRelativeLocation(DeltaLocation);
	MeshComponent->AddRelativeRotation(FRotator(0.0, DeltaTime * RotationSpeed, 0.0));

	// 拾取相关
	if (bIsPickingUp)
	{
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), Player->GetActorLocation(), DeltaTime, InterpSeepd));
		if (GetActorLocation().Equals(Player->GetActorLocation(), 10.0f))
		{
			UClass* ActorClass = Player->GetClass();

			if (ActorClass->ImplementsInterface(UInventoryInterface::StaticClass()))
			{
				IInventoryInterface* InterfaceIns = CastChecked<IInventoryInterface>(Player);

				if (InterfaceIns->AddItemToInventory(ItemStack))
				{
					if (PickupSound)
					{
						UGameplayStatics::PlaySound2D(this, PickupSound);
					}
					Destroy();
				}
			}
		}
	}
}

void ADroppedItem::SetItemStack(const FItemStack& NewItemStack)
{
	ItemStack = NewItemStack;
	if (ItemStack.Item)
	{
		MeshComponent->SetStaticMesh(ItemStack.Item->GetMesh());
	}
}

void ADroppedItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 吸附移动相关
	if (IsValid(OtherActor))
	{
		Player = OtherActor;
		bIsPickingUp = true;
	}
}

