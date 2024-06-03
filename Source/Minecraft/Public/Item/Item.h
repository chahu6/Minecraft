#pragma once

#include "CoreMinimal.h"
#include "Item/Info/ItemInfo.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UItem();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
	FItemDetails ItemInfo;

public:
	FORCEINLINE UStaticMesh* GetMesh() const { return ItemInfo.Mesh; }
	FORCEINLINE int32 GetMaxCount() const { return ItemInfo.MaxCount; }
	FORCEINLINE bool IsStack() const { return ItemInfo.bIsStack; }
};
