

#include "AbilitySystem/TopdownAbilitySystemComponent.h"

void UTopdownAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTopdownAbilitySystemComponent::EffectApply);
}

void UTopdownAbilitySystemComponent::AddCharacterAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UTopdownAbilitySystemComponent::EffectApply(UAbilitySystemComponent* AbilitySystemComponent,
                                                 const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// 빈 태그 컨테이너 생성
	FGameplayTagContainer TagContainer;

	// EffectSpect의 모든 태그를 컨테이너에 채움
	EffectSpec.GetAllAssetTags(TagContainer);
	for (const FGameplayTag& Tag : TagContainer)
	{
		EffectAssetTags.Broadcast(TagContainer);
	}
}
