#include "TestPreviewScene.h"
#include "GameFramework/WorldSettings.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"

FTestPreviewScene::FTestPreviewScene(ConstructionValues CVS)
	:FPreviewScene(CVS)
{
	GetWorld()->GetWorldSettings()->NotifyBeginPlay();
	GetWorld()->GetWorldSettings()->NotifyMatchStarted();
	GetWorld()->GetWorldSettings()->SetActorHiddenInGame(false);
	GetWorld()->bBegunPlay = true;

	//DirectionalLight->SetRelativeLocation(FVector(-1024.0f, 1024.0f, 2048.0f));
	//DirectionalLight->SetRelativeScale3D(FVector(15.f));

	//SetLightBrightness(4.0f);
	//DirectionalLight->InvalidateLightingCache();
	//SetSkyBrightness(10.0f);
}

FLinearColor FTestPreviewScene::GetBackgroundColor() const
{
	FLinearColor BackgroundColor = FColor(55, 55, 55);
	return BackgroundColor;
}
