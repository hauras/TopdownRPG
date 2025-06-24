
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopdownEffectActor.generated.h"

class UGameplayEffect;
UCLASS()
class TOPDOWNRPG_API ATopdownEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATopdownEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

};
