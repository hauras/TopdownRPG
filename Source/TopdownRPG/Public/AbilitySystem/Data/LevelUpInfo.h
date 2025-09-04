
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FTopdownLevelUPInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUPRequirement = 0; // 누적 경험치
	
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1; // 스텟 포인트
	
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1; // 스킬 포인트
};
/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FTopdownLevelUPInfo> LevelUpInformation;

	int32 FindLevelForXP(int32 XP) const;
};
