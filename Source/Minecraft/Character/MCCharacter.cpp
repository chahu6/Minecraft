#include "MCCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Minecraft/Controller/MCPlayerController.h"
#include "Camera/CameraComponent.h"

AMCCharacter::AMCCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 自身不转动
	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0f, 0.0f));
	GetMesh()->bOwnerNoSee = true;

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
}

void AMCCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMCCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMCCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Switch Perspectives", IE_Pressed, this, &AMCCharacter::SwitchPerspectives);
	PlayerInputComponent->BindAction("Add Block", IE_Pressed, this, &AMCCharacter::AddBlock);
	PlayerInputComponent->BindAction("Remove Block", IE_Pressed, this, &AMCCharacter::RemoveBlock);

}

void AMCCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMCCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
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
		break;
	}
	case AMCCharacter::EPerspective::Third:
	{
		FollowCamera->SetActive(true);
		FirstCamera->SetActive(false);
		NextPerspective = EPerspective::First;
		GetMesh()->SetOwnerNoSee(false);
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
	AMCPlayerController* PlayerController = Cast<AMCPlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->AddBlock();
	}
}

void AMCCharacter::RemoveBlock()
{
	AMCPlayerController* PlayerController = Cast<AMCPlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->RemoveBlock();
	}
}
