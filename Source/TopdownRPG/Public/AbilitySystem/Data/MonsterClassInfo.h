
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MonsterClassInfo.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	// 일반 몬스터 3종
	Melee,      // 근접 공격 타입 
	Ranged,     // 원거리 공격 타입 
	Support,    // 특수 타입 

	// 보스 몬스터
	Boss        
};

USTRUCT(BlueprintType)
struct FMonsterClassDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryMonsterAttributes;
};


UCLASS()
class TOPDOWNRPG_API UMonsterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<EMonsterType, FMonsterClassDefaultInfo> MonsterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = " Attributes ")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = " Attributes ")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	FMonsterClassDefaultInfo GetClassDefaultInfo(EMonsterType MonsterClass);
};
