

#include "AbilitySystem/TopdownAbilitySystemComponent.h"

#include "TopdownGameplayTags.h"
#include "AbilitySystem/Abilities/TopdownGameplayAbility.h"
#include "TopdownRPG/TopdownLogChannels.h"

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
	// UI에 표시
	bStartupAbilitiesGiven = true;
	AbilitiesGiven.Broadcast(this);
}

void UTopdownAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1); // 어떤 어빌리티인지, 레벨은 몇으로 할지?
		GiveAbilityAndActivateOnce(AbilitySpec);
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

void UTopdownAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTopdown, Error, TEXT("델리게이트 연동 실패 %hs"), __FUNCTION__);
		}
	}
}

// 어빌리티 스펙에서 Abilities 태그 가져옴 
FGameplayTag UTopdownAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

// 어빌리티 스펙에서 InputTag를 가져옴
FGameplayTag UTopdownAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UTopdownAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGiven.Broadcast(this);

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
