
#include "AbilitySystem/Abilities/BeamSpell.h"
#include "AbilitySystem/TopdownAbilityFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

void UBeamSpell::StartChaining(AActor* FirstTarget)
{
	if (!FirstTarget) return;

	// 연쇄 공격을 시작하기 전에 상태 변수들을 깨끗하게 초기화합니다.
	HitTargets.Empty();
	HitTargets.Add(FirstTarget);
	CurrentTarget = FirstTarget;

	// 첫 번째 연쇄 공격을 시작합니다.
	ChainNextTarget();
}

TArray<AActor*> UBeamSpell::GetHitTargets()
{
	TArray<AActor*> Result;
	for (const TObjectPtr<AActor>& HitActorPtr : HitTargets)
	{
		if (HitActorPtr.Get())
		{
			Result.Add(HitActorPtr.Get());
		}
	}
	return Result;
}

void UBeamSpell::ChainNextTarget()
{
	UAbilitySystemComponent* MyASC = GetAbilitySystemComponentFromActorInfo();
	// CurrentTarget이 딜레이 도중에 죽거나 사라졌을 수 있으므로 유효성을 검사합니다.
	if (!MyASC || !IsValid(CurrentTarget))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}

	// 1. 다음 타겟 찾기
	TArray<AActor*> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	for (const TObjectPtr<AActor>& HitActor : HitTargets)
	{
		if (HitActor.Get()) // TObjectPtr에서 AActor*를 안전하게 가져옵니다.
		{
			ActorsToIgnore.Add(HitActor.Get());
		}
	}
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CurrentTarget->GetActorLocation(), SearchRadius, ObjectTypes, nullptr, ActorsToIgnore, OverlappedActors);

	AActor* NextTarget = nullptr;
	if (OverlappedActors.Num() > 0)
	{
		TArray<AActor*> ClosestTargets;
		UTopdownAbilityFunctionLibrary::GetClosestTargets(1, OverlappedActors, ClosestTargets, CurrentTarget->GetActorLocation());
		if (ClosestTargets.Num() > 0)
		{
			NextTarget = ClosestTargets[0];
		}
	}

	// 다음 타겟이 없으면 어빌리티를 종료합니다.
	if (!NextTarget)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}

	// 2. 부모 클래스(UTopdownGameplayAbility)의 ApplyDamage 함수를 호출하여 데미지를 적용합니다.
	ApplyDamage(NextTarget);
	
	// 게임플레이 큐를 실행하여 이펙트를 표시합니다. (CurrentTarget -> NextTarget)
	if (ChainGameplayCueTag.IsValid())
	{
		FGameplayCueParameters CueParams;
		CueParams.Instigator = GetAvatarActorFromActorInfo();
		CueParams.SourceObject = this;
		CueParams.TargetAttachComponent = CurrentTarget->GetRootComponent();
		CueParams.Location = NextTarget->GetActorLocation();
		MyASC->ExecuteGameplayCue(ChainGameplayCueTag, CueParams);
	}

	// 3. 상태를 다음 단계로 업데이트합니다.
	HitTargets.Add(NextTarget);
	CurrentTarget = NextTarget;

	// 4. 최대 연쇄 횟수에 도달했는지 확인하고, 도달했다면 어빌리티를 종료합니다.
	if (HitTargets.Num() >= MaxChainTargets)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}

	// 5. 다음 연쇄를 위해 WaitDelay 태스크를 생성하고 OnFinish 델리게이트에 ChainNextTarget 함수를 바인딩합니다.
	UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, ChainDelay);
	WaitDelayTask->OnFinish.AddDynamic(this, &UBeamSpell::ChainNextTarget); // 람다 대신 AddDynamic 사용
	WaitDelayTask->ReadyForActivation();
}
