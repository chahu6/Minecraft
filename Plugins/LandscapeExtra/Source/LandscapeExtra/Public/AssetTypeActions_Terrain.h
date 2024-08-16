#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class LANDSCAPEEXTRA_API FAssetTypeActions_Terrain : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_Terrain(EAssetTypeCategories::Type InAssetCategory);

	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;

	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;
	// ���ڶ����֮����ʾ�ı༭��������δ������ս��
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
private:

	EAssetTypeCategories::Type AssetCategory;
};
