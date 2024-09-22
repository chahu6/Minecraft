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
	
	Initializer();
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
	if (IPlayerControllerInterface* PlayerControllerInterface = GetController<IPlayerControllerInterface>())
	{
		IPlayerControllerInterface::Execute_OpenBackpack(GetController());
	}
}

void AEntityPlayer::SwitchingItem(const FInputActionValue& Value)
{
	int32 WheelValue = -Value.Get<float>();
	
	MainHandIndex = (MainHandIndex + WheelValue + 9) % 9;

	UpdateMainHandItem();

	OnSwitchMainHand.Broadcast(MainHandIndex);
}

void AEntityPlayer::Initializer()
{
	UpdateMainHandItem();
}

void AEntityPlayer::UpdateMainHandItem()
{
	FItemData MainItemData = GetMainHandItem();

	ItemMesh->SetStaticMesh(MainItemData.IsValid() ? MainItemData.Mesh : nullptr);
}

FItemData AEntityPlayer::GetMainHandItem()
{
	if (BackpackComponent)
	{
		return BackpackComponent->GetSelected(MainHandIndex);
	}

	return {};
}

void AEntityPlayer::DropItem()
{
}

void AEntityPlayer::ConsumeItem()
{
	if (BackpackComponent)
	{
		BackpackComponent->ConsumeItem(MainHandIndex);
	}
}

bool AEntityPlayer::AddItemToInventory_Implementation(FItemData& ItemData)
{
	return BackpackComponent->AddItemToInventory(ItemData);
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
		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Started, this, &AEntityPlayer::DropItem);
	}
}