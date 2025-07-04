
#include "Controller/TopdownPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/TopdownAbilitySystemComponent.h"
#include "Input/TopdownInputComponent.h"
#include "Interface/EnemyInterface.h"



ATopdownPlayerController::ATopdownPlayerController()
{
	bReplicates = true;
}

void ATopdownPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void ATopdownPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(InputContext, 0);

	// 마우스 커서 화면에 보이게 설정
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default; // CrossHair 커서는 고민중

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ATopdownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UTopdownInputComponent* TopdownInputComponent = CastChecked<UTopdownInputComponent>(InputComponent);

	TopdownInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopdownPlayerController::Move);

	TopdownInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ATopdownPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ATopdownPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();
	
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void ATopdownPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void ATopdownPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

void ATopdownPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UTopdownAbilitySystemComponent* ATopdownPlayerController::GetASC()
{
	if (TopdownAbilitySystemComponent == nullptr)
	{
		TopdownAbilitySystemComponent = Cast<UTopdownAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return TopdownAbilitySystemComponent;
}
