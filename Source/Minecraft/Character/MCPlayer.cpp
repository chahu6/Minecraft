#include "MCPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Minecraft/MinecraftComponents/Movement/MinecraftPlayerMovement.h"
#include "Minecraft/Controller/MCPlayerController.h"


AMCPlayer::AMCPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BoxCollision->InitBoxExtent(FVector(31.0f, 31.0f, 90.0f));
	BoxCollision->SetCollisionProfileName(TEXT("BlockAll"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxCollision);
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	Mesh->SetRelativeRotation(FRotator(0.0, -90.0f, 0.0f));
	Mesh->bOwnerNoSee = true;

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

	// 自由视角
	//FreeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FreeCamera"));

	MinecraftPlayerMovement = CreateDefaultSubobject<UMinecraftPlayerMovement>(TEXT("MinecraftPlayerMovement"));
	MinecraftPlayerMovement->UpdatedComponent = RootComponent;
}

void AMCPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorRotation(FRotator::ZeroRotator);
}

void AMCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, GetVelocity().ToString());
	}
}

void AMCPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCPlayer::MoveRight);

	PlayerInputComponent->BindAxis("TurnRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMCPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMCPlayer::StopJump);

	PlayerInputComponent->BindAction("Switch Perspectives", IE_Pressed, this, &AMCPlayer::SwitchPerspectives);
	PlayerInputComponent->BindAction("Add Block", IE_Pressed, this, &AMCPlayer::AddBlock);
	PlayerInputComponent->BindAction("Remove Block", IE_Pressed, this, &AMCPlayer::RemoveBlock);
}

UPawnMovementComponent* AMCPlayer::GetMovementComponent() const
{
	return MinecraftPlayerMovement;
}

void AMCPlayer::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMCPlayer::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
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
			Mesh->SetOwnerNoSee(true);
			break;
		}
		case AMCPlayer::EPerspective::Third:
		{
			FollowCamera->SetActive(true);
			FirstCamera->SetActive(false);
			NextPerspective = EPerspective::First;
			Mesh->SetOwnerNoSee(false);
			break;
		}
		case AMCPlayer::EPerspective::Free:
		{
			break;
		}
	}
}

void AMCPlayer::AddBlock()
{
	AMCPlayerController* PlayerController = Cast<AMCPlayerController>(Controller);
	if (PlayerController)
	{
		//PlayerController->AddBlock();
	}
}

void AMCPlayer::RemoveBlock()
{
	AMCPlayerController* PlayerController = Cast<AMCPlayerController>(Controller);
	if (PlayerController)
	{
		//PlayerController->RemoveBlock();
	}
}

void AMCPlayer::Jump()
{
	if (!bIsJumping)
	{

		bIsJumping = true;
	}
}

void AMCPlayer::StopJump()
{
	if (bIsJumping)
	{
		bIsJumping = false;
	}
}
