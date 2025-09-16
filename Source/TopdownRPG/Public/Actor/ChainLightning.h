#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChainLightning.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class UNiagaraSystem; // Niagara 시스템을 사용하기 위해 전방 선언

UCLASS()
class TOPDOWNRPG_API AChainLightning : public AActor
{
	GENERATED_BODY()
	
public:	
	AChainLightning();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// --- 기존에 사용하던 변수들 (대부분 그대로 사용) ---

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"), Category = "Lightning")
	TObjectPtr<AActor> InitialTarget;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"), Category = "Lightning")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	int32 MaxChainCount = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float ChainRadius = 850.f; // 강의 내용에 맞춰 기본값 상향

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float DelayBetweenJumps = 0.2f;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"), Category = "Lightning")
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
	
	// --- 새로 추가되거나 변경된 부분 ---

	// [새로 추가] 블루프린트에서 직접 설정할 나이아가라 파티클 시스템입니다.
	// GameplayCueTag보다 훨씬 안정적이고 직접적인 제어가 가능합니다.
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ChainVFX;

private:
	FTimerHandle ChainTimerHandle;
	TArray<TObjectPtr<AActor>> DamagedTargets; // 기존과 동일하게 피격된 타겟 추적용으로 사용

	TArray<TObjectPtr<AActor>> AllChainTargets;

	int32 TargetIndex = 0;

	void ChainToNextTarget();
	
	UFUNCTION()
	void OnTargetDied(AActor* DeadActor);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayChainEffect(const FVector& StartPoint, const FVector& EndPoint);
};