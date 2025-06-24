

#include "UI/WidgetController/TopdownWidgetController.h"

void UTopdownWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UTopdownWidgetController::BroadcastInitialValues()
{
	
}

void UTopdownWidgetController::BindCallbacksToDependencies()
{
}
