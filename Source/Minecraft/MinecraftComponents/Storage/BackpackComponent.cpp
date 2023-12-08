#include "BackpackComponent.h"

UBackpackComponent::UBackpackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Items.SetNum(27);
	HotbarItems.SetNum(9);
	ArmorItems.SetNum(4);
}

void UBackpackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBackpackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

FItemStack UBackpackComponent::GetSelected()
{
	if (HotbarItems.IsValidIndex(SelectedIndex))
	{
		return HotbarItems[SelectedIndex];
	}
	else
	{
		return FItemStack();
	}
}

