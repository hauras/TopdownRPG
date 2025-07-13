
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
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EMonsterType MonsterType, float Level, UAbilitySystemComponent* ASC);
};
