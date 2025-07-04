

#include "TopdownAssetManager.h"
#include "TopdownGameplayTags.h"

UTopdownAssetManager& UTopdownAssetManager::Get()
{
	check(GEngine);
	
	UTopdownAssetManager* TopdownAssetManager =  Cast<UTopdownAssetManager>(GEngine->AssetManager);
	return *TopdownAssetManager;
}

void UTopdownAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FTopdownGameplayTags::InitializeNativeGameplayTags();
}
