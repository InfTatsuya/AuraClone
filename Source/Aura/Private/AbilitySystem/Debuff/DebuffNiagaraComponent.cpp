// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/AuraLogChanels.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if(UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	else if(CombatInterface)
	{
		CombatInterface->GetOnASCRegisterDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
		});
	}

	if(CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerActorDead);
	}
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bIsOwnerValid = IsValid(GetOwner());
	const bool bIsOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	
	if(NewCount > 0 && bIsOwnerValid && bIsOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerActorDead(AActor* DeadActor)
{
	UE_LOG(LogAura, Warning, TEXT("Deactivate VFX"));
	Deactivate();
}
