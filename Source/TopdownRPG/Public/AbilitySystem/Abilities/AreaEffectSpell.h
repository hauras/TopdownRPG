
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TopdownGameplayAbility.h"
#include "AreaEffectSpell.generated.h"

/**
 * 
 */
class ATopdownEffectActor;
UCLASS()
class TOPDOWNRPG_API UAreaEffectSpell : public UTopdownGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Spell")
	void SpawnAreaEffect(const FVector& TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
	TSubclassOf<ATopdownEffectActor> AreaEffectClass; 
	
	
};
