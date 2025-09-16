#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TopdownGameplayAbility.h"
#include "BeamSpell.generated.h"

UCLASS()
class TOPDOWNRPG_API UBeamSpell : public UTopdownGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "BeamSpell")
	void StartChaining(AActor* FirstTarget);

	UFUNCTION(BlueprintCallable, Category = "BeamSpell")
	TArray<AActor*> GetHitTargets();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BeamSpell")
	int32 MaxChainTargets = 5; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BeamSpell")
	float SearchRadius = 800.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BeamSpell")
	FGameplayTag ChainGameplayCueTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BeamSpell")
	float ChainDelay = 0.2f;

private:
	// 딜레이 태스크가 완료된 후 호출될 함수입니다. UFUNCTION() 매크로가 필수입니다.
	UFUNCTION()
	void ChainNextTarget();

	// 연쇄 공격의 상태를 저장할 멤버 변수입니다.
	// UPROPERTY()를 붙여서 가비지 컬렉터가 임의로 메모리를 해제하는 것을 방지합니다.
	UPROPERTY()
	TArray<TObjectPtr<AActor>> HitTargets;

	// 현재 체인의 시작점이 될 타겟입니다.
	UPROPERTY()
	TObjectPtr<AActor> CurrentTarget;

	
};