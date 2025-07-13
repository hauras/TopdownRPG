
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/MonsterClassInfo.h"
#include "Character/CharacterBase.h"
#include "Interface/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
class UWidgetComponent;

UCLASS()
class TOPDOWNRPG_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	virtual int32 GetPlayerLevel() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster Class Defaults")
	EMonsterType MonsterClass = EMonsterType::Melee;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
