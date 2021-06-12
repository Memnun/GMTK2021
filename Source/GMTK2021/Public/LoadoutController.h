// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentData.h"

/**
 * 
 */
class GMTK2021_API LoadoutController
{
public:
	LoadoutController();
	~LoadoutController();
	ComponentData CoreSlot;
	ComponentData LeftArmSlot;
	ComponentData RightArmSlot;
	ComponentData LegSlot;
};
