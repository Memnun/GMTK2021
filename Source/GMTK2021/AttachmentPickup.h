// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "AttachmentPickup.generated.h"

UENUM(BlueprintType)
enum class EAttachmentClass : uint8
{
	Attachment_Legs,
	Attachment_Arms,
	Attachment_Core,
	Attachment_MAX
};

/**
 * 
 */
UCLASS(Abstract)
class GMTK2021_API AAttachmentPickup : public APickupBase
{
	GENERATED_BODY()
public:

	AAttachmentPickup();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	EAttachmentClass AttachmentClass = EAttachmentClass::Attachment_Legs;

protected:
	virtual void BeginApply_Internal() override;
	virtual void OnEffectEnd_Internal() override;
};
