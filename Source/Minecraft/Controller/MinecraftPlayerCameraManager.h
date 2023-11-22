#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MinecraftPlayerCameraManager.generated.h"

class AChunk;
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
	// Ҫ��Tick��������
	bool IsOnFrustum(const AChunk* Chunk);

private:
	// ��ֱ�����ϵ�FOV
	float H_FOV;

	float Factor_Z;
	float Tan_Z;

	float Factor_Y;
	float Tan_Y;
};