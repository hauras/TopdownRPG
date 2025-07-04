

#include "Input/TopdownInputConfig.h"

const UInputAction* UTopdownInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FTopdownInputAction& Action : AbilityInputAction)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Input tag %s, InputConfig %s"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
	
}
