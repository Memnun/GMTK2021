// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GMTK2021_API LoadoutController
{
public:

	enum CoreComponent {
		NoCore,
		BaseCore,
		HealthCore,
		DamageCore
	};

	enum ArmComponent {
		NoArm,
		MeleeArm,
		MachineGun,
		Rifle,
		Shotgun,
		RocketLauncher,
		BouncyGun
	};

	enum LegComponent {
		NoLeg,
		BaseLeg,
		Dash,
		MagLeg,
		DoubleJump
	};

	LoadoutController();
	~LoadoutController();
	CoreComponent CoreSlot;
	ArmComponent LeftArmSlot;
	ArmComponent RightArmSlot;
	LegComponent LegSlot;
};
