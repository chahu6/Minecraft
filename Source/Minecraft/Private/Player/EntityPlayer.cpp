#include "Player/EntityPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/Interactive/InteractiveComponent.h"
#include "Controller/MCPlayerController.h"
#include "Components/Inventory/BackpackComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Crafting/CraftingComponent.h"
#include "UI/HUD/MinecraftHUD.h"
#include "Components/CapsuleComponent.h"
#include "Components/Crafting/CraftingResultComponent.h"
#include "Blueprint/UserWidget.h"

#include "UI/Widget/StorageUI/Backpack.h"
#include "Item/Item.h"
#include "World/WorldManager.h"
#include "Entity/Item/EntityItem.h"

AEntityPlayer::AEntityPlayer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// 自身不转动
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

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
	ItemMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));
	ItemMesh->SetRelativeRotation(FRotator(0.f, 30.f, 0.f));
	ItemMesh->SetRelativeLocation(FVector(20.f, 14.f, -15.f));
	ItemMesh->SetRelativeScale3D(FVector(0.1f));
	ItemMesh->SetCastShadow(false);

	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMesh"));
	ItemSkeletalMesh->SetupAttachment(FirstCamera);
	ItemSkeletalMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));
	ItemSkeletalMesh->SetRelativeRotation(FRotator(0.f, 30.f, 0.f));
	ItemSkeletalMesh->SetRelativeLocation(FVector(20.f, 14.f, -15.f));
	ItemSkeletalMesh->SetRelativeScale3D(FVector(0.1f));
	ItemSkeletalMesh->SetCastShadow(false);

	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOverlap"));
	SphereOverlap->SetupAttachment(RootComponent);
	SphereOverlap->SetSphereRadius(145.0f);
	SphereOverlap->SetCollisionProfileName(TEXT("Player"));

	// 交互组件
	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));

	// 背包
	BackpackComponent = CreateDefaultSubobject<UBackpackComponent>(TEXT("BackpackComponent"));

	// 制作组件
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(TEXT("CraftingComponent"));
	CraftingResultComponent = CreateDefaultSubobject<UCraftingResultComponent>(TEXT("CraftingResultComponent"));
}

void AEntityPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IPlayerControllerInterface* PlayerControllerInterface = GetController<IPlayerControllerInterface>())
	{
		IPlayerControllerInterface::Execute_InitMainUI(NewController);
	}
}

void AEntityPlayer::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (IPlayerControllerInterface* PlayerControllerInterface = GetController<IPlayerControllerInterface>())
	{
		IPlayerControllerInterface::Execute_InitMainUI(GetController());
	}
}

void AEntityPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (InteractiveComponent)
	{
		InteractiveComponent->Player = this;
	}

	if (BackpackComponent)
	{
		BackpackComponent->OnHotbarUpdate.AddDynamic(this, &AEntityPlayer::UpdateMainHandItem);
	}
}

void AEntityPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	Initialization();
}

void AEntityPlayer::SwitchPerspectives()
{
	switch (NextPerspective)
	{
		case AEntityPlayer::EPerspective::First:
		{
			FollowCamera->SetActive(false);
			FirstCamera->SetActive(true);
			NextPerspective = EPerspective::Third;
			GetMesh()->SetOwnerNoSee(true);
			//ArmMesh->SetOwnerNoSee(false);
			break;
		}
		case AEntityPlayer::EPerspective::Third:
		{
			FollowCamera->SetActive(true);
			FirstCamera->SetActive(false);
			NextPerspective = EPerspective::First;
			GetMesh()->SetOwnerNoSee(false);
			//ArmMesh->SetOwnerNoSee(true);
			break;
		}
		case AEntityPlayer::EPerspective::Free:
		{
			break;
		}
	}
}

void AEntityPlayer::UseItem()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->UseItem();
	}
}

void AEntityPlayer::OnClickAction()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->ClickBlock();
	}
}

void AEntityPlayer::OngoinAction()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->OngoingClick();
	}
}

void AEntityPlayer::OnResetAction()
{
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->ResetBlockRemoving();
	}
}

void AEntityPlayer::OpenBackpack()
{
	check(InventoryWidgetClass);
	InventoryContainer = CreateWidget<UBackpack>(GetWorld()->GetFirstPlayerController(), InventoryWidgetClass);
	InventoryContainer->AddToViewport();
	OpenContainer = InventoryContainer;

	SetInputModeUIOnly();

	//ToggleInventory();
}

void AEntityPlayer::SwitchingItem(const FInputActionValue& Value)
{
	const int32 WheelValue = -Value.Get<float>();
	
	MainHandIndex = (MainHandIndex + WheelValue + 9) % 9;

	UpdateMainHandItem();

	OnSwitchMainHand.Broadcast(MainHandIndex);
}

void AEntityPlayer::Initialization()
{
	//InitialInventoryUI();

	UpdateMainHandItem();
}

void AEntityPlayer::InitialInventoryUI()
{
	check(InventoryWidgetClass);
	InventoryContainer = CreateWidget<UBackpack>(GetWorld()->GetFirstPlayerController(), InventoryWidgetClass);
	InventoryContainer->AddToViewport();
	InventoryContainer->SetVisibility(ESlateVisibility::Hidden);
	OpenContainer = InventoryContainer;
}

//void AEntityPlayer::ToggleInventory()
//{
//	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
//
//	if (InventoryContainer->GetVisibility() == ESlateVisibility::Visible)
//	{
//		InventoryContainer->SetVisibility(ESlateVisibility::Hidden);
//
//		FInputModeGameOnly InputModeGameOnly;
//		PlayerController->SetInputMode(InputModeGameOnly);
//		PlayerController->bShowMouseCursor = false;
//		PlayerController->SetIgnoreLookInput(false);
//	}
//	else if (InventoryContainer->GetVisibility() == ESlateVisibility::Hidden)
//	{
//		InventoryContainer->SetVisibility(ESlateVisibility::Visible);
//
//		FInputModeGameAndUI InputModeGameAndUI;
//		InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
//		InputModeGameAndUI.SetWidgetToFocus(InventoryContainer->TakeWidget());
//		PlayerController->SetInputMode(InputModeGameAndUI);
//		PlayerController->bShowMouseCursor = true;
//		PlayerController->SetIgnoreLookInput(true);
//	}
//}

FVector AEntityPlayer::GetItemSpawnLocation()
{
	const FVector ForwardVector = GetController<APlayerController>()->PlayerCameraManager->GetActorForwardVector();
	
	return ForwardVector * 100.f + GetPawnViewLocation();
}

void AEntityPlayer::SetInputModeUIOnly()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeUIOnly.SetWidgetToFocus(OpenContainer->TakeWidget());
	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->bShowMouseCursor = true;
	PlayerController->SetIgnoreLookInput(true);
}

void AEntityPlayer::SetInputModeGameOnly()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
	PlayerController->bShowMouseCursor = false;
	PlayerController->SetIgnoreLookInput(false);
}

void AEntityPlayer::UpdateMainHandItem()
{
	FItemStack MainItemStack = GetMainHandItem();

	if (!MainItemStack.IsEmpty())
	{

	}

	ItemMesh->SetStaticMesh(MainItemStack.GetItem()->StaticMesh);
	ItemSkeletalMesh->SetSkeletalMesh(MainItemStack.GetItem()->SkeletalMesh);
}

FItemStack AEntityPlayer::GetMainHandItem()
{
	if (BackpackComponent)
	{
		return BackpackComponent->GetHotbarItemStack(MainHandIndex);
	}

	return {};
}

void AEntityPlayer::DropAction()
{
	//UGameplayStatics::DeprojectScreenToWorld // 2D坐标转3D

	DropItem(false);
}

AEntityItem* AEntityPlayer::DropItem(bool bDropAll)
{
	FItemStack MainHandItemStack = GetMainHandItem();
	int32 Count = bDropAll && !MainHandItemStack.IsEmpty() ? MainHandItemStack.GetCount() : 1;

	return DropItem(BackpackComponent->DecreStackSize(MainHandIndex, Count));
}

AEntityItem* AEntityPlayer::DropItem(const FItemStack& ItemStack)
{
	if (ItemStack.IsEmpty()) return nullptr;

	const FVector ForwardVector = GetController<APlayerController>()->PlayerCameraManager->GetActorForwardVector();
	AEntityItem* EntityItem = AWorldManager::Get()->SpawnEntity(GetItemSpawnLocation(), ItemStack);
	if (EntityItem)
	{
		EntityItem->AddImpulse(ForwardVector * Thrust);
	}
	return EntityItem;
}

void AEntityPlayer::ConsumeItem()
{
	if (BackpackComponent)
	{
		BackpackComponent->ConsumeItem(MainHandIndex);
	}
}

bool AEntityPlayer::OnItemPickup_Implementation(FItemStack& ItemStack)
{
	return BackpackComponent->AddItemToInventory(ItemStack);
}

bool AEntityPlayer::DisplayGui(UContainer* Container)
{
	if (Container)
	{
		OpenContainer = Container;
		OpenContainer->AddToViewport();
		SetInputModeUIOnly();
		return true;
	}

	return false;
}

void AEntityPlayer::CloseContainer()
{
	OpenContainer->OnContainerClosed(this);
	OpenContainer = InventoryContainer;
}

void AEntityPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 交互方块
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Started, this, &AEntityPlayer::UseItem);

		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Started, this, &AEntityPlayer::OnClickAction);
		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Triggered, this, &AEntityPlayer::OngoinAction);
		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Completed, this, &AEntityPlayer::OnResetAction);

		EnhancedInputComponent->BindAction(SwitchPerspectivesAction, ETriggerEvent::Started, this, &AEntityPlayer::SwitchPerspectives);

		// 打开背包
		EnhancedInputComponent->BindAction(OpenBackpackAction, ETriggerEvent::Started, this, &AEntityPlayer::OpenBackpack);

		// 鼠标滚轮
		EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &AEntityPlayer::SwitchingItem);

		// DropItem
		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Started, this, &AEntityPlayer::DropAction);
	}
}