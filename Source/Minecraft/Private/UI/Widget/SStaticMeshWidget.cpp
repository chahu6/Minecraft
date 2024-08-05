#include "UI/Widget/SStaticMeshWidget.h"
#include "SlateOptMacros.h"
#include "Slate/SlateVectorArtInstanceData.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SStaticMeshWidget::Construct(const FArguments& InArgs)
{
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

int32 SStaticMeshWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (StaticMeshIndex != INDEX_NONE)
    {
        const float Scale = AllottedGeometry.Scale;
        const FVector2D ScreenPosition = AllottedGeometry.LocalToAbsolute(AllottedGeometry.GetLocalSize() * 0.5f);

        FSlateInstanceBufferData SlateInstanceBufferData;
        FSlateVectorArtInstanceData SlateVectorArtInstaceData;
        SlateVectorArtInstaceData.SetPosition(FVector2D::ZeroVector);
        SlateVectorArtInstaceData.SetScale(Scale);
        SlateInstanceBufferData.Add(TArray<UE::Math::TVector4<float>>::ElementType(SlateVectorArtInstaceData.GetData()));
        const_cast<SStaticMeshWidget*>(this)->UpdatePerInstanceBuffer(StaticMeshIndex, SlateInstanceBufferData);
    }

    return SMeshWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

uint32 SStaticMeshWidget::AddStaticMesh(USlateVectorArtData& InMeshData)
{
    StaticMeshIndex = AddMesh(InMeshData);
    return StaticMeshIndex;
}
