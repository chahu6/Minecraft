#include "MCPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Minecraft/MinecraftComponents/Interactive/InteractiveComponent.h"
#include "Minecraft/Controller/MCPlayerController.h"
#include "Minecraft/MinecraftComponents/Storage/BackpackComponent.h"
#include "Components/SphereComponent.h"
#include "Minecraft/MinecraftComponents/Crafting/CraftingComponent.h"
#include "Minecraft/Item/Info/ItemInfo.h"

AMCPlayer::AMCPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 自身不转动
	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0f, 0.0f));
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->SetRelativeRotation(FRotator(310.0, 0.0, 0.0));
	CameraBoom->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));

	// 第三人称视角
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->bAutoActivate = false;

	// 第一人称视角
	FirstCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCamera"));
	FirstCamera->SetupAttachment(RootComponent);
	FirstCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	FirstCamera->bUsePawnControlRotation = true;

	/* 第一人称手臂，但是有问题
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(FirstCamera);
	ArmMesh->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
	ArmMesh->bOnlyOwnerSee = true;
	ArmMesh->bCastDynamicShadow = false;
	ArmMesh->CastShadow = false;
	*/

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(FirstCamera);

	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOverlap"));
	SphereOverlap->SetupAttachment(RootComponent);
	SphereOverlap->SetSphereRadius(145.0f);
	SphereOverlap->SetCollisionProfileName(TEXT("Player"));

	// 交互组件
	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));

	// 背包
	BackpackComponent = CreateDefaultSubobject<UBackpackComponent>(TEXT("BackpackComponent"));

	// 合成组件
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(TEXT("CraftingComponent"));
}

void AMCPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (InteractiveComponent)
	{
		InteractiveComponent->Player = this;
	}

	if (BackpackComponent)
	{
		BackpackComponent->Player = this;
	}
}

void AMCPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	Initial();
}

void AMCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 交互方块
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Started, this, &AMCPlayer::UseItem);

		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Started, this, &AMCPlayer::OnClickAction);
		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Triggered, this, &AMCPlayer::OngoinAction);
		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Completed, this, &AMCPlayer::OnResetAction);

		EnhancedInputComponent->BindAction(SwitchPerspectivesAction, ETriggerEvent::Started, this, &AMCPlayer::SwitchPerspectives);

		// 打开背包
		EnhancedInputComponent->BindAction(OpenBackpackAction, ETriggerEvent::Started, this, &AMCPlayer::OpenBackpack);

		// 鼠标滚轮
		EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &AMCPlayer::SwitchingItem);

		// DropItem
		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Started, this, &AMCPlayer::DropItem);
	}
}

bool AMCPlayer::AddItem(const ADroppedItem* DroppedItem)
{
	if (BackpackComponent != nullptr)
	{
		return BackpackComponent->AddItemToInventory(DroppedItem);
	}
	return false;
}

void AMCPlayer::SwitchPerspectives()
{
	switch (NextPerspective)
	{
		case AMCPlayer::EPerspective::First:
		{
			FollowCamera->SetActive(false);
			FirstCamera->SetActive(true);
			NextPerspective = EPerspective::Third;
			GetMesh()->SetOwnerNoSee(true);
			//ArmMesh->SetOwnerNoSee(false);
			break;
		}
		case AMCPlayer::EPerspective::Third:
		{
			FollowCamera->SetActive(true);
			FirstCamera->SetActive(false);
			NextPerspective = EPerspective::First;
			GetMesh()->SetOwnerNoSee(false);
			//ArmMesh->SetOwnerNoSee(true);
			break;
		}
		case AMCPlayer::EPerspective::Free:
		{
			break;
		}
	}
}

void AMCPlayer::UseItem()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->UseItem();
	}
}

void AMCPlayer::OnClickAction()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->ClickBlock();
	}
}

void AMCPlayer::OngoinAction()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->OngoingClick();
	}
}

void AMCPlayer::OpenBackpack()
{
	AMCPlayerController* PlayerController = Cast<AMCPlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		PlayerController->OpenBackpack();
	}
}

void AMCPlayer::SwitchingItem(const FInputActionValue& Value)
{
	int32 WheelValue = -Value.Get<float>();

	MainHandIndex = (MainHandIndex + WheelValue + 9) % 9;

	UpdateMainHandItem();

	OnSwitchMainHand.Broadcast(MainHandIndex);
}

void AMCPlayer::DropItem()
{

}

void AMCPlayer::OnResetAction()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->ResetBlockRemoving();
	}
}

FItemStack AMCPlayer::GetMainHandItem()
{
	if (BackpackComponent)
	{
		return BackpackComponent->GetSelected(MainHandIndex);
	}

	return FItemStack();
}

void AMCPlayer::ConsumeItemStack()
{
	if (BackpackComponent)
	{
		BackpackComponent->ConsumeItemStack();
	}
}

void AMCPlayer::Initial()
{
	UpdateMainHandItem();
}

void AMCPlayer::UpdateMainHandItem()
{
	int32 ID = GetMainHandItem().ID;
	if (ItemsDataTable)
	{
		FItemDetails* ItemDetails = ItemsDataTable->FindRow<FItemDetails>(FName(FString::FromInt(ID)), TEXT("AMCPlayer"));
		if (ItemDetails)
		{
			ItemMesh->SetStaticMesh(ItemDetails->Mesh);
		}
		else
		{
			ItemMesh->SetStaticMesh(nullptr);
		}
	}
}
