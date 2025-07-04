
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "TopdownPlayerController.generated.h"

class UTopdownAbilitySystemComponent;
class UTopdownInputConfig;
class IEnemyInterface;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API ATopdownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopdownPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
	FHitResult CursorHit;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UTopdownInputConfig* InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UTopdownAbilitySystemComponent> TopdownAbilitySystemComponent;

	UTopdownAbilitySystemComponent* GetASC();
};
