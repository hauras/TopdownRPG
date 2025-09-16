
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TopdownGameplayAbility.h"
#include "TeleportSpell.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UTeleportSpell : public UTopdownGameplayAbility
{
	GENERATED_BODY()
	
public:
	UTeleportSpell();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport")
	float NavMeshProjectionRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	float MaxTeleportDistance = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	FGameplayTag TeleportStartCueTag;

};
