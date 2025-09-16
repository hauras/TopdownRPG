
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/MonsterClassInfo.h"
#include "TopdownAbilityFunctionLibrary.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UTopdownAbilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	// 몬스터의 기본 속성 초기화
	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EMonsterType MonsterType, float Level, UAbilitySystemComponent* ASC);

	// 몬스터에 시작 어빌리티 부여
	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|CharacterClassDefaults" )
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static UMonsterClassInfo* GetMonsterClassInfo(const UObject* WorldContextObject);

	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, EMonsterType MonsterType, int32 MonsterLevel);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Targeting")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);
	
};
