// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachmentPickup.h"
#include "PickupBase.h"
#include "PlayerCharacter.h"

AAttachmentPickup::AAttachmentPickup()
{
	bPersistant = true;
}

void AAttachmentPickup::BeginApply_Internal()
{
	if(!Player) return;

	if (Player->Attachments.Contains(AttachmentClass))
	{
		if(Player->Attachments[AttachmentClass])
			Player->Attachments[AttachmentClass]->ForceEndEffect();
	}

	Player->Attachments.Add(AttachmentClass, this);
}

void AAttachmentPickup::OnEffectEnd_Internal()
{
	if (Player)
	{
		if(Player->Attachments.Contains(AttachmentClass))
			Player->Attachments.Remove(AttachmentClass);
	}

	Super::OnEffectEnd_Internal();
}
