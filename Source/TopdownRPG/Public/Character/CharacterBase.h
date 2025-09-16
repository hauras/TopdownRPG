
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "AbilitySystem/Data/MonsterClassInfo.h"
#include "GameplayTagContainer.h"
#include "CharacterBase.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UAnimMontage;

UCLASS(Abstract)
class TOPDOWNRPG_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	// 캐릭터의 어빌리티시스템 반환
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// 캐릭터의 AttributeSet 반환하는 인라인 함수
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/* CombatInterface*/
	// 피격시 재생할 몽타주 반환
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual EMonsterType GetMonsterType_Implementation() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;

	/* CombatInterface*/

	// 죽음을 모든 클라에 복제하는 함수
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

    virtual FOnDeathSignature& GetOnDeathDelegate() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	// 무기 
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// 무기를 장착하는 소켓의 이름
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName WeaponSocketName;

	// 전투 관련 (스킬 발사 등) 위치를 결정하기 위한 소켓의 위치 반환
	virtual FVector GetCombatSocketLocation() override;

	// 캐릭터의 어빌리티 시스템
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 캐릭터의 속성 셋(스탯 등)
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeathDelegate;
	
	// 어빌리티 시스템에 소유자(Owner)와 아바타액터 설정
	// 플레이어와 적을 다르게 구현하므로 자식 클래스에서 사용
	virtual void InitAbilityActorInfo();

	//기본 속성(힘, 지능, 민첩등)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	//보조 속성(방어력, 치명타확률 등)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	// 생명 속성
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// 시작시 가지고 있을 어빌리티 - 서버에서만 부여 
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// 자신에게 GameplayEffect를 적용하는 헬퍼 함수
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	// 블루프린트에서 설정된 속성들을 캐릭터에 적용
	virtual void InitializeDefaultAttributes() const;

	//캐릭터에 어빌리티 부여
	void AddCharacterAbilities();

	//몬스터 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster Class Defaults")
	EMonsterType MonsterClass = EMonsterType::Melee;

	UPROPERTY(ReplicatedUsing = OnRep_Frozen, BlueprintReadOnly)
	bool bIsFrozen = false;

	UFUNCTION()
	virtual void OnRep_Frozen();
private:

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
};
