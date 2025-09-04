
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TopdownAIController.generated.h"

class UAISenseConfig_Sight;
class UBlackboardComponent;
class UBehaviorTreeComponent;
class UAIPerceptionComponent;
/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API ATopdownAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATopdownAIController(const FObjectInitializer& ObjectInitializer);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent; // 감각 총괄 센서

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight; // 시각 설정

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY(EditDefaultsOnly, Category = " Crowd Avoidance")
	bool bEnableDetourCrowdAvoidance = true;

	UPROPERTY(EditDefaultsOnly, Category = " Crowd Avoidance", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = " Crowd Avoidance", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;

};
