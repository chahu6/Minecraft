#include "Item/DroppedItem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Interfaces/InteractiveInterface.h"

ADroppedItem::ADroppedItem()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 60.0f;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(TEXT("Item"));
	Box->SetSimulatePhysics(true);
	Box->SetEnableGravity(true);
	RootComponent = Box;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComponent->SetRelativeScale3D(FVector(0.4f));
}

void ADroppedItem::OnConstruction(const FTransform& Transform)
{
	ConstructionInit();
}

void ADroppedItem::ConstructionInit()
{
	SetItemData();
}

void ADroppedItem::SetItemData()
{
	FItemInstance* ItemInstance = ItemHandle.GetRow<FItemInstance>(nullptr);
	if (ItemInstance)
	{
		ItemData.ID = ItemInstance->ID;
		ItemData.bIsStack = ItemInstance->bIsStack;
		ItemData.Discription = ItemInstance->Discription;
		ItemData.Icon = ItemInstance->Icon;
		ItemData.MaxCount = ItemInstance->MaxCount;
		ItemData.Name = ItemInstance->Name;
		ItemData.Mesh = ItemInstance->Mesh;
		ItemData.Type = ItemInstance->Type;
		ItemData.Quantity = 1;
		MeshComponent->SetStaticMesh(ItemData.Mesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item << %s >> not found in data table!"), *ItemHandle.RowName.ToString())
	}
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
			if (IInteractiveInterface::Execute_AddItemToInventory(Player, this))
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

void ADroppedItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 吸附移动相关
	if (IsValid(OtherActor) && OtherActor->Implements<UInteractiveInterface>())
	{
		Player = OtherActor;
		bIsPickingUp = true;
	}
}
