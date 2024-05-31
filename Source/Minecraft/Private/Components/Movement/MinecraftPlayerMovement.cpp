#include "Components/Movement/MinecraftPlayerMovement.h"

void UMinecraftPlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Velocity = ConsumeInputVector().GetClampedToMaxSize(1) * MaxVelocity;
	FVector DeltaMovement = Velocity * DeltaTime;
	DeltaMovement.Z = GetGravityZ() * DeltaTime;
	if (!DeltaMovement.IsNearlyZero())
	{
		FHitResult HitResult;
		SafeMoveUpdatedComponent(DeltaMovement, UpdatedComponent->GetComponentRotation(), true, HitResult);

		if (HitResult.IsValidBlockingHit())
		{
			SlideAlongSurface(DeltaMovement, 1.0f - HitResult.Time, HitResult.Normal, HitResult);
		}
	}

	UpdateComponentVelocity();
}
