

#include "AbilitySystem/Data/AttributeInfo.h"

FTopdownAttributeInfo UAttributeInfo::FindAttributeInfo(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FTopdownAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag)
		{
			if (Info.AttributeTag.MatchesTagExact(AttributeTag))
			{
				return Info;
			}
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("%s on AttributeInfo %s"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FTopdownAttributeInfo();
}
