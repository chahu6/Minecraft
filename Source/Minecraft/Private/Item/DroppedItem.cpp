#include "Item/DroppedItem.h"
#include "Components/BoxComponent.h"
#include "Interfaces/ItemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ADroppedItem::ADroppedItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetCollisionProfileName(TEXT("Item"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComponent->SetRelativeScale3D(FVector(0.4f));

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCueObject(TEXT("/Script/Engine.SoundCue'/Game/Minecraft/Assets/Sound/Pickup_Cue.Pickup_Cue'"));
	if (SoundCueObject.Succeeded())
	{
		PickupSound = SoundCueObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT("/Script/Engine.DataTable'/Game/Minecraft/Datas/DataTable/DT_ItemDetails.DT_ItemDetails'"));
	if (ItemDataTableObject.Succeeded())
	{
		ItemDataTable = ItemDataTableObject.Object;
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
			UClass* ActorClass = Player->GetClass();

			if (ActorClass->ImplementsInterface(UItemInterface::StaticClass()))
			{
				IItemInterface* InterfaceIns = CastChecked<IItemInterface>(Player);

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
	}
}

void ADroppedItem::SetItemStack(const FItemStack& NewItemStack)
{
	ItemStack = NewItemStack;
	if (ItemDataTable)
	{
		FItemDetails* ItemDetails = ItemDataTable->FindRow<FItemDetails>(FName(FString::FromInt(ItemStack.ID)), TEXT("ADroppedItem"));
		if (ItemDetails)
		{
			ItemStack.MaxCount = ItemDetails->MaxCount;
			ItemStack.Type = ItemDetails->Type;
			MeshComponent->SetStaticMesh(ItemDetails->Mesh);
		}
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

