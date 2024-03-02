// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for(const FAuraAttributeInfo& info : AttributeInfomation)
	{
		if(info.AttributeTag.MatchesTagExact(AttributeTag)) return info;
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find Info for Tag: [%s] on AttributeInfo [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}
