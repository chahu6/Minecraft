#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MinecraftPlayerCameraManager.generated.h"

class AChunkSection;
/**
 * 
 */
UCLASS()
class MINECRAFT_API AMinecraftPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AMinecraftPlayerCameraManager();

public:
	// 要在Tick函数中用
	bool IsOnFrustum(const AChunkSection* ChunkSection);

private:
	// 竖直方向上的FOV
	float H_FOV;

	float Factor_Z;
	float Tan_Z;

	float Factor_Y;
	float Tan_Y;
};
