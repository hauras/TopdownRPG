#include "Actor/TopdownEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"

ATopdownEffectActor::ATopdownEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));



}

void ATopdownEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void ATopdownEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 1. 안전장치: 타겟이 유효하지 않으면 아무것도 하지 않고 즉시 함수를 종료합니다.
	if (!IsValid(Target))
	{
		return;
	}

	// 2. 자기 자신 예외 처리: 이 장판을 소환한 액터(Instigator)와 장판에 닿은 액터(Target)가 동일 인물이라면,
	//    아무것도 하지 않고 즉시 함수를 종료합니다. 이것이 '나에게 데미지가 들어오는 것'을 막는 핵심 코드입니다.
	if (Target == GetInstigator())
	{
		return;
	}

	// 기존의 적 태그 체크 로직은 그대로 유지합니다.
	// (예: 아군에게는 적용 안 함 등의 로직이 있다면 여기에 위치)
	// if (Target->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	// 타겟의 어빌리티 시스템 컴포넌트(ASC)를 가져옵니다.
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC == nullptr) return;

	// GameplayEffectClass가 유효하지 않은 경우를 대비한 check 추가
	check(GameplayEffectClass);

	// 이펙트의 상세 정보를 담을 컨텍스트 핸들을 생성합니다.
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();

	// [핵심 수정] 이펙트의 최종 책임자(Instigator)와 물리적 원인 제공자(EffectCauser)를 설정합니다.
	// GetInstigator()는 이 장판을 소환한 플레이어를 반환합니다.
	// "이 데미지는 장판(this)이 가했지만, 시킨 건 플레이어(GetInstigator())다" 라고 명시하여
	// 경험치와 같은 공로가 올바르게 플레이어에게 돌아가도록 합니다.
	EffectContextHandle.AddInstigator(GetInstigator(), this);

	// 이 효과를 발생시킨 소스 오브젝트는 이 액터(ATopdownEffectActor) 자신임을 명시합니다.
	EffectContextHandle.AddSourceObject(this);

	// 위에서 설정한 정보(컨텍스트)를 포함하여 이펙트 스펙 핸들을 생성합니다.
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);

	// 유효한 스펙 핸들이 만들어졌는지 확인하고, 타겟에게 이펙트를 적용합니다.
	if (EffectSpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		// 이 이펙트가 무한 지속시간을 가지는지 확인합니다.
		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

		// 무한 지속 효과이며, Overlap이 끝났을 때 제거하는 정책이라면 맵에 저장해둡니다.
		if (bIsInfinite && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
		{
			ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
		}
	}
}
void ATopdownEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void ATopdownEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// EndOverlap에서 이펙트 적용
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	// Infinite 효과 제거 조건이면...
	if (InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		// 타겟의 ASC를 가져옴
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		// 제거할 핸들을 임시 저장할 배열
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		// 모든 저장된 효과 핸들을 순회
		for (auto HandlePair : ActiveEffectHandles)
		{
			// 타겟 ASC와 일치하는 경우
			if (TargetASC == HandlePair.Value)
			{
				// 1스택 제거
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		// 제거된 핸들들을 맵에서도 제거
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
