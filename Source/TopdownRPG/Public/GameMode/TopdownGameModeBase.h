
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopdownGameModeBase.generated.h"

class UMonsterClassInfo;
/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API ATopdownGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Monster Class Defaults")
	TObjectPtr<UMonsterClassInfo> MonsterClassInfo;
	
};
