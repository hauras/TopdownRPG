
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TopdownAbilitySystemComponent.generated.h"

/**
 * 
 */
// GameplayEffect에서 AssetTag를 추출했을 때 이를 알리기 위한 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& );
// 캐릭터에 기본 어빌리티가 부여되었을 때 호출되는 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UTopdownAbilitySystemComponent*);
// 모든 활성화 가능한 어빌리티를 순회하는 델리게이트(ex: 각 어빌리티의 정보를 UI에 표시등)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

UCLASS()
class TOPDOWNRPG_API UTopdownAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();

	/*델리게이트 */
	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGiven;

	// 캐릭터에 시작 어빌리티 부여 함수
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	bool bStartupAbilitiesGiven = false;

	// 특정 입력 태그에 연결된 어빌리티를 실행하거나 상태 업데이트
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	// 어빌리티 스펙에서 태그를 가져오는 함수
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
protected:

	virtual void OnRep_ActivateAbilities() override;
	
	UFUNCTION(Client, Reliable)
	void ClientEffectApply(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
