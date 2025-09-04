
#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API APlayerCharacter : public ACharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// PlayerInterface의 함수
	virtual void AddToEXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	
	virtual int32 GetPlayerLevel_Implementation() override;
private:
	virtual void InitAbilityActorInfo() override;
};
