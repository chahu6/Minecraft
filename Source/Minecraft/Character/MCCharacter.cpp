#include "MCCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Minecraft/MinecraftComponents/Interactive/InteractiveComponent.h"
#include "Minecraft/MinecraftComponents/Inventory/InventoryComponent.h"
#include "Minecraft/Controller/MCPlayerController.h"


AMCCharacter::AMCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

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

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(FirstCamera);
	ArmMesh->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
	ArmMesh->bOnlyOwnerSee = true;
	ArmMesh->bCastDynamicShadow = false;
	ArmMesh->CastShadow = false;

	// 交互组件
	InteractiveComp = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));

	// 背包
	BackpackComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("BackpackComponent"));
	BackpackComp->SetInventorySize(27);

	// 快捷栏
	HotbarComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("HotComponent"));
	HotbarComp->SetInventorySize(9);
}

void AMCCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (InteractiveComp)
	{
		InteractiveComp->Character = this;
	}
}

void AMCCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMCCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMCCharacter::Look);

		// 交互方块
		EnhancedInputComponent->BindAction(AddBlockAction, ETriggerEvent::Started, this, &AMCCharacter::AddBlock);
		EnhancedInputComponent->BindAction(RemoveBlockAction, ETriggerEvent::Started, this, &AMCCharacter::RemoveBlock);
		EnhancedInputComponent->BindAction(SwitchPerspectivesAction, ETriggerEvent::Started, this, &AMCCharacter::SwitchPerspectives);

		// 打开背包
		EnhancedInputComponent->BindAction(OpenBackpackAction, ETriggerEvent::Started, this, &AMCCharacter::OpenBackpack);
	}
}

bool AMCCharacter::AddItem(const ADroppedItem* Item)
{
	if (BackpackComp != nullptr)
	{
		return BackpackComp->AddItemToInventory(Item);
	}
	return false;
}

void AMCCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMCCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMCCharacter::SwitchPerspectives()
{
	switch (NextPerspective)
	{
		case AMCCharacter::EPerspective::First:
		{
			FollowCamera->SetActive(false);
			FirstCamera->SetActive(true);
			NextPerspective = EPerspective::Third;
			GetMesh()->SetOwnerNoSee(true);
			ArmMesh->SetOwnerNoSee(false);
			break;
		}
		case AMCCharacter::EPerspective::Third:
		{
			FollowCamera->SetActive(true);
			FirstCamera->SetActive(false);
			NextPerspective = EPerspective::First;
			GetMesh()->SetOwnerNoSee(false);
			ArmMesh->SetOwnerNoSee(true);
			break;
		}
		case AMCCharacter::EPerspective::Free:
		{
			break;
		}
	}
}

void AMCCharacter::AddBlock()
{
	if (InteractiveComp != nullptr)
	{
		InteractiveComp->AddBlock();
	}
}

void AMCCharacter::RemoveBlock()
{
	if (InteractiveComp != nullptr)
	{
		InteractiveComp->RemoveBlock();
	}
}

void AMCCharacter::OpenBackpack()
{
	AMCPlayerController* PlayerController = Cast<AMCPlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		PlayerController->OpenBackpack();
	}
}
