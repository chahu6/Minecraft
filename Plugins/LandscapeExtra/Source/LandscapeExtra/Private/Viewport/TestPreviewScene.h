#pragma once

#include "CoreMinimal.h"
#include "PreviewScene.h"

class FTestPreviewScene : public FPreviewScene
{
public:
	FTestPreviewScene(ConstructionValues CVS);

	/** Begin FPreviewScene */
	virtual FLinearColor GetBackgroundColor() const override;
	/** End FPreviewScene */
};