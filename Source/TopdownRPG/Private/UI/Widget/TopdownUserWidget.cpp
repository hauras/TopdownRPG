

#include "UI/Widget/TopdownUserWidget.h"

void UTopdownUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
