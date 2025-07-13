
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Magician,
	Warrior,
	// 추후 도적? 궁수? 등 직업 추가
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = " Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

UCLASS()
class TOPDOWNRPG_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = " Attributes ")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = " Attributes ")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);


};
