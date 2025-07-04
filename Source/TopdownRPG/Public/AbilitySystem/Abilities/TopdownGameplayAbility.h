
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TopdownGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UTopdownGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
