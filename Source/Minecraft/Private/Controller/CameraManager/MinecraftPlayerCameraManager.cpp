#include "Controller/CameraManager/MinecraftPlayerCameraManager.h"
#include "Chunk/ChunkSection.h"
#include "World/WorldSettings.h"

AMinecraftPlayerCameraManager::AMinecraftPlayerCameraManager()
{
	Factor_Y = 1.0f / FMath::Cos(ViewTarget.POV.FOV * 0.5f);
	Tan_Y = FMath::Tan(ViewTarget.POV.FOV * 0.5f);
	
	H_FOV = 2 * FMath::Atan(FMath::Tan(ViewTarget.POV.FOV * 0.5f) * ViewTarget.POV.AspectRatio);

	Factor_Z = 1.0f / FMath::Cos(H_FOV * 0.5f);
	Tan_Z = FMath::Tan(H_FOV * 0.5f);
}

bool AMinecraftPlayerCameraManager::IsOnFrustum(const AChunkSection* ChunkSection)
{
	const FRotator& Rotation = ViewTarget.POV.Rotation;
	FVector SpereVector = ChunkSection->GetCenter() - ViewTarget.POV.Location;

	// 超出这个近剪切平面
	double SX = SpereVector.Dot(Rotation.Quaternion().GetRightVector());
	if (!(SX > NearClipPlane - CHUNK_SPHERE_RADIUS * BlockSize))
		return false;

	// 超出这个TOP和BOTTOM
	double SZ = SpereVector.Dot(Rotation.Quaternion().GetUpVector());
	double Dist = Factor_Z * CHUNK_SPHERE_RADIUS * BlockSize + SX * Tan_Z;
	if (!(-Dist < SZ && SZ < Dist))
		return false;

	// 超出左右面
	double SY = SpereVector.Dot(Rotation.Quaternion().GetRightVector());
	Dist = Factor_Y * CHUNK_SPHERE_RADIUS * BlockSize + SX * Tan_Y;
	if (!(-Dist < SY && SY < Dist))
		return false;
		
	return true;
}
