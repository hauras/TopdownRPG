
#include "Character/CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "TopdownGameplayTags.h"
#include "AbilitySystem/TopdownAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "TopdownRPG/TopdownRPG.h"
#include "GameplayTagsManager.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h" 
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// 어빌리티 시스템 반환
UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// 피격 반응 몽타주 반환
UAnimMontage* ACharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

// 사망 로직 구현 
void ACharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

EMonsterType ACharacterBase::GetMonsterType_Implementation()
{
	return MonsterClass;
}

// 래그돌 방식 구현 (죽음)
void ACharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// 무기 소켓의 위치 반환
FVector ACharacterBase::GetCombatSocketLocation() 
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponSocketName);
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterBase, bIsFrozen);

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const FTopdownGameplayTags& GameplayTags = FTopdownGameplayTags::Get();
	const bool bIsDebuffed = NewCount > 0;

	if (CallbackTag.MatchesTag(GameplayTags.State_Frozen))
	{
		bIsFrozen = bIsDebuffed; 

		// 멤버 변수 bIsFrozen의 상태에 따라 움직임을 제어
		GetCharacterMovement()->SetMovementMode(bIsFrozen ? MOVE_None : MOVE_Walking);
	}
}

void ACharacterBase::InitAbilityActorInfo()
{
	if (AbilitySystemComponent)
	{
		const FTopdownGameplayTags& GameplayTags = FTopdownGameplayTags::Get();

		AbilitySystemComponent->RegisterGameplayTagEvent(
			GameplayTags.State_Frozen,
            EGameplayTagEventType::NewOrRemoved // 이벤트 타입도 NewOrRemoved로 변경
			).AddUObject(this, &ACharacterBase::DebuffTagChanged);
		
	}
}

void ACharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	// 이펙트의 문맥 핸들 생성 ( 누가, 무엇으로 이펙트를 발생시키는지)
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);

}

void ACharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;

	UTopdownAbilitySystemComponent* TopdownASC = Cast<UTopdownAbilitySystemComponent>(AbilitySystemComponent);
	if (!TopdownASC) return;

	TopdownASC->AddCharacterAbilities(StartupAbilities);
	TopdownASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void ACharacterBase::OnRep_Frozen()
{
}





