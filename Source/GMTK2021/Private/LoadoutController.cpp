// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadoutController.h"
#include "ComponentData.h"

class LoadoutController {

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

	LoadoutController() {
		ComponentData CoreSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
		ComponentData RightArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
		ComponentData LeftArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
		ComponentData LegSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
		ComponentPickup(BaseCore, MeleeArm, MeleeArm, BaseLeg);
	}

	void ComponentPickup(CoreComponent core, ArmComponent right, ArmComponent left, LegComponent leg) {
		if (core != NoCore) {
			switch (core) {
			case BaseCore:
				CoreSlot = ComponentData(10, 0.0, 0, 10, 1.0, 0, 0.0, 0);
			case HealthCore:
				CoreSlot = ComponentData(50, 0.0, 0, 10, 1.0, 0, 0.0, 0);
			case DamageCore:
				CoreSlot = ComponentData(10, 0.0, 0, 10, 1.0, 0, 0.0, 0);
			}
		}
		if (right != NoArm) {
			switch (right) {
			case MeleeArm:
				RightArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case MachineGun:
				RightArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case Rifle:
				RightArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case Shotgun:
				RightArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case RocketLauncher:
				RightArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case BouncyGun:
				RightArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			}
		}
		if (left != NoArm) {
			switch (left) {
			case MeleeArm:
				LeftArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case MachineGun:
				LeftArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case Rifle:
				LeftArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case Shotgun:
				LeftArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case RocketLauncher:
				LeftArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case BouncyGun:
				LeftArmSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			}
		}
		if (leg != NoLeg) {
			switch (leg) {
			case BaseLeg:
				LegSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case Dash:
				LegSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case MagLeg:
				LegSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			case DoubleJump:
				LegSlot = ComponentData(0, 0.0, 0, 0, 0.0, 0, 0.0, 0);
			}
		}
	}

};