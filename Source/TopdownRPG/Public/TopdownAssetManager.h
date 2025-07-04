
#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TopdownAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UTopdownAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	static UTopdownAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
