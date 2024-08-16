#include "LandscapeExtraTerrain.h"
#include "Kismet/KismetRenderingLibrary.h"

ULandscapeExtraTerrain::ULandscapeExtraTerrain()
{
	NoiseMapRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this);
}

#if WITH_EDITOR
void ULandscapeExtraTerrain::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	FProperty* MemberPropertyThatChanged = PropertyChangedEvent.MemberProperty;

	const FName MemberPropertyName = MemberPropertyThatChanged != NULL ? MemberPropertyThatChanged->GetFName() : NAME_None;

	if (MemberPropertyName.IsEqual(TEXT("TerrainSize")))
	{
		NoiseMapRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, TerrainSize.X, TerrainSize.Y, ETextureRenderTargetFormat::RTF_R16f);
	}

	if (OnAssetPropertyChanged.IsBound())
	{
		OnAssetPropertyChanged.Execute();
	}
}
#endif