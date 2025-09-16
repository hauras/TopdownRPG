
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

protected:

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void ApplyDamage(AActor* TargetActor);
};
