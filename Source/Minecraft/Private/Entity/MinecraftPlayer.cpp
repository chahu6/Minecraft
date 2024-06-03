﻿#include "Entity/MinecraftPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/Interactive/InteractiveComponent.h"
#include "Controller/MCPlayerController.h"
#include "Components/Inventory/BackpackComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Crafting/CraftingComponent.h"
#include "Item/Info/ItemInfo.h"

AMinecraftPlayer::AMinecraftPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 自身不转动
	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
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
	CraftingComponent->SetSize(2);
}

void AMinecraftPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (InteractiveComponent)
	{
		InteractiveComponent->Player = this;
	}
}

void AMinecraftPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	Initial();
}

void AMinecraftPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMinecraftPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 交互方块
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Started, this, &AMinecraftPlayer::UseItem);

		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Started, this, &AMinecraftPlayer::OnClickAction);
		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Triggered, this, &AMinecraftPlayer::OngoinAction);
		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Completed, this, &AMinecraftPlayer::OnResetAction);

		EnhancedInputComponent->BindAction(SwitchPerspectivesAction, ETriggerEvent::Started, this, &AMinecraftPlayer::SwitchPerspectives);

		// 打开背包
		EnhancedInputComponent->BindAction(OpenBackpackAction, ETriggerEvent::Started, this, &AMinecraftPlayer::OpenBackpack);

		// 鼠标滚轮
		EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &AMinecraftPlayer::SwitchingItem);

		// DropItem
		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Started, this, &AMinecraftPlayer::DropItem);
	}
}

void AMinecraftPlayer::SwitchPerspectives()
{
	switch (NextPerspective)
	{
		case AMinecraftPlayer::EPerspective::First:
		{
			FollowCamera->SetActive(false);
			FirstCamera->SetActive(true);
			NextPerspective = EPerspective::Third;
			GetMesh()->SetOwnerNoSee(true);
			//ArmMesh->SetOwnerNoSee(false);
			break;
		}
		case AMinecraftPlayer::EPerspective::Third:
		{
			FollowCamera->SetActive(true);
			FirstCamera->SetActive(false);
			NextPerspective = EPerspective::First;
			GetMesh()->SetOwnerNoSee(false);
			//ArmMesh->SetOwnerNoSee(true);
			break;
		}
		case AMinecraftPlayer::EPerspective::Free:
		{
			break;
		}
	}
}

void AMinecraftPlayer::UseItem()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->UseItem();
	}
}

void AMinecraftPlayer::OnClickAction()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->ClickBlock();
	}
}

void AMinecraftPlayer::OngoinAction()
{
	if (InteractiveComponent != nullptr)
	{
		//InteractiveComponent->OngoingClick();
	}
}

void AMinecraftPlayer::OpenBackpack()
{
	AMCPlayerController* PlayerController = Cast<AMCPlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		PlayerController->OpenBackpack();
	}
}

void AMinecraftPlayer::SwitchingItem(const FInputActionValue& Value)
{
	int32 WheelValue = -Value.Get<float>();

	MainHandIndex = (MainHandIndex + WheelValue + 9) % 9;

	UpdateMainHandItem();

	OnSwitchMainHand.Broadcast(MainHandIndex);
}

void AMinecraftPlayer::DropItem()
{

}

void AMinecraftPlayer::OnResetAction()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->ResetBlockRemoving();
	}
}

FItemStack AMinecraftPlayer::GetMainHandItem()
{
	if (BackpackComponent)
	{
		return BackpackComponent->GetSelected(MainHandIndex);
	}

	return FItemStack();
}

void AMinecraftPlayer::ConsumeItemStack()
{
	//if (BackpackComponent)
	{
		//BackpackComponent->ConsumeItemStack();
	}
}

void AMinecraftPlayer::Initial()
{
	UpdateMainHandItem();
}

void AMinecraftPlayer::UpdateMainHandItem()
{
	int32 ID = GetMainHandItem().ID;

	if (ID < 0)
	{
		ItemMesh->SetStaticMesh(nullptr);
		return;
	}

	if (ItemsDataTable)
	{
		FItemDetails* ItemDetails = ItemsDataTable->FindRow<FItemDetails>(FName(FString::FromInt(ID)), TEXT("AMinecraftPlayer"));
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