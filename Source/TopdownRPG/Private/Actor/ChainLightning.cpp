#include "Actor/ChainLightning.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"

AChainLightning::AChainLightning()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AChainLightning::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChainLightning::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (TObjectPtr<AActor> Target : AllChainTargets)
	{
		if (IsValid(Target.Get()))
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target.Get()))
			{
				CombatInterface->GetOnDeathDelegate().RemoveDynamic(this, &AChainLightning::OnTargetDied);
			}
		}
	}
	Super::EndPlay(EndPlayReason);
}

void AChainLightning::ChainToNextTarget()
{
	const int32 StartNodeIndex = TargetIndex;
	const int32 EndNodeIndex = TargetIndex + 1;

	// 경로의 시작과 끝이 모두 유효한지 확인합니다.
	if (AllChainTargets.IsValidIndex(StartNodeIndex) && AllChainTargets.IsValidIndex(EndNodeIndex))
	{
		AActor* StartTarget = AllChainTargets[StartNodeIndex];
		AActor* EndTarget = AllChainTargets[EndNodeIndex];

		// 1. 시각 효과 재생
		const FVector StartPoint = StartTarget->GetActorLocation();
		const FVector EndPoint = EndTarget->GetActorLocation();
		Multicast_PlayChainEffect(StartPoint, EndPoint);

		// [디버그 코드] 현재 연쇄 구간 표시
		DrawDebugSphere(GetWorld(), StartPoint, 25.f, 12, FColor::Green, false, DelayBetweenJumps + 0.1f);
		DrawDebugSphere(GetWorld(), EndPoint, 25.f, 12, FColor::Red, false, DelayBetweenJumps + 0.1f);
		UE_LOG(LogTemp, Log, TEXT("Chaining from '%s' to '%s'"), *StartTarget->GetName(), *EndTarget->GetName());

		// 2. [핵심 수정] "첫 번째 적부터" 대미지를 적용합니다. (플레이어는 대미지를 받지 않음)
		if (TargetIndex > 0 && DamageEffectClass)
		{
			FGameplayEffectSpecHandle SpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, 1.f, SourceAbilitySystemComponent->MakeEffectContext());
    
			if (SpecHandle.IsValid())
			{
				if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EndTarget))
				{
					TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}

		// 3. 다음 연쇄 준비
		TargetIndex++;
		GetWorld()->GetTimerManager().SetTimer(
			ChainTimerHandle,
			this,
			&AChainLightning::ChainToNextTarget,
			DelayBetweenJumps,
			false
		);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Chain Lightning finished. Destroying actor."));
		Destroy();
	}
}

void AChainLightning::OnTargetDied(AActor* DeadActor)
{
	if (AllChainTargets.Contains(DeadActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Target '%s' died. Stopping chain."), *DeadActor->GetName());
		GetWorld()->GetTimerManager().ClearTimer(ChainTimerHandle);
		Destroy();
	}
}

void AChainLightning::Multicast_PlayChainEffect_Implementation(const FVector& StartPoint, const FVector& EndPoint)
{
	if (ChainVFX)
	{
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ChainVFX,
			StartPoint
		);

		if (NiagaraComp)
		{
			NiagaraComp->SetNiagaraVariableVec3(FString("BeamEnd"), EndPoint);
		}
	}
}