#include "MCPlayerController.h"
#include "Minecraft/HUD/MinecraftHUD.h"
#include "Blueprint/UserWidget.h"
#include "MinecraftPlayerCameraManager.h"

#include "SimplexNoiseLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Minecraft/Generation/Noise/NormalNoise.h"
#include "Minecraft/Math/Spline.h"

AMCPlayerController::AMCPlayerController()
{
	PlayerCameraManagerClass = AMinecraftPlayerCameraManager::StaticClass();
}

void AMCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MinecraftHUD = Cast<AMinecraftHUD>(GetHUD());
}

void AMCPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void AMCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ShowDebugInfo", IE_Pressed, this, &AMCPlayerController::ShowDebugInfo);
}

UTexture2D* AMCPlayerController::CreateTextureFromArray()
{
	USimplexNoiseLibrary::SetNoiseSeed(Seed);

	TSharedPtr<NormalNoise> Offset = NormalNoise::Create(-3, { 1.0, 1.0, 1.0, 0.0 });
	TSharedPtr<NormalNoise> Continentalness = NormalNoise::Create(-9, { 1.0, 1.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0 });
	TSharedPtr<NormalNoise> Weirdness = NormalNoise::Create(-7, { 1.0, 2.0, 1.0, 0.0, 0.0, 0.0 });
	TSharedPtr<NormalNoise> Erosion = NormalNoise::Create(-9, { 1.0, 1.0, 0.0, 1.0, 1.0 });
	TSharedPtr<NormalNoise> Humidity = NormalNoise::Create(-8, { 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 });
	TSharedPtr<NormalNoise> Temperature = NormalNoise::Create(-10, { 1.5, 0.0, 1.0, 0.0, 0.0, 0.0 });

	int32 TextureHeight = 1024;
	int32 TextureWidth = 1024;
	UTexture2D* TheTexture2D = UTexture2D::CreateTransient(TextureHeight, TextureWidth, PF_B8G8R8A8);
	uint8* Pixels = new uint8[TextureWidth * TextureHeight * 4];
	for (int32 y = 0; y < TextureHeight; ++y)
	{
		for (int32 x = 0; x < TextureWidth; ++x)
		{
			int32 CurPixelIndex = ((y * TextureWidth) + x);

			double Color = 0;
			/*
			double ColorX = x + 4 * Offset->GetValue(x, 0, y);
			double ColorY = y + 4 * Offset->GetValue(y, x, 0);
			
			Color = Erosion->GetValue(ColorX, 0, ColorY);

			if (Color < -0.9)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), Color));
				*/

			Color /= (1 + 0.5 + 0.25);
			Color = FMath::Pow(Color * FudgeFactor, Exponent);
			Color = UKismetMathLibrary::MapRangeClamped(Color, -1, 1, 0, 255);

			Pixels[4 * CurPixelIndex] = Color;
			Pixels[4 * CurPixelIndex + 1] = Color;
			Pixels[4 * CurPixelIndex + 2] = Color;
			Pixels[4 * CurPixelIndex + 3] = 255;
		}
	}

	void* TextureData = TheTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureWidth * TextureHeight * 4);
	TheTexture2D->PlatformData->Mips[0].BulkData.Unlock();
	TheTexture2D->UpdateResource();

	return TheTexture2D;
}

void AMCPlayerController::ShowDebugInfo()
{
	if (bIsDebug)
	{
		if (MinecraftHUD)
		{
			MinecraftHUD->RemoveDebugInfo();
			bIsDebug = false;
		}
	}
	else
	{
		if (MinecraftHUD)
		{
			MinecraftHUD->AddDebugInfo();
			bIsDebug = true;
		}
	}
}