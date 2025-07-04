

#include "AbilitySystem/TopdownAbilitySystemComponent.h"

#include "TopdownGameplayTags.h"
#include "AbilitySystem/Abilities/TopdownGameplayAbility.h"

void UTopdownAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTopdownAbilitySystemComponent::ClientEffectApply);

	const FTopdownGameplayTags& GameplayTags = FTopdownGameplayTags::Get();
}

void UTopdownAbilitySystemComponent::AddCharacterAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1); // 어떤 어빌리티인지, 레벨은 몇으로 할지?
		if (const UTopdownGameplayAbility* TopdownAbility = Cast<UTopdownGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(TopdownAbility->StartupInputTag);
			GiveAbility(AbilitySpec);

		}
	}
}

void UTopdownAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UTopdownAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UTopdownAbilitySystemComponent::ClientEffectApply_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
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
