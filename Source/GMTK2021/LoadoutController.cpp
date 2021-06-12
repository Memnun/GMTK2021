// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadoutController.h"
#include "ComponentData.h"

class LoadoutController {

	LoadoutController() {
		CoreComponent CoreSlot = BaseCore;
		ArmComponent RightArmSlot = MeleeArm;
		ArmComponent LeftArmSlot = MeleeArm;
		LegComponent LegSlot = BaseLeg;
	}

	void ComponentPickup(CoreComponent core, ArmComponent right, ArmComponent left, LegComponent leg) {
		if (core != NoCore) {
			CoreSlot = core;
		}
		if (right != NoArm) {
			RightArmSlot = right;
			}
		}
		if (left != NoArm) {
			LeftArmSlot = left;
			}
		}
		if (leg != NoLeg) {
			LegSlot = leg;
		}
	}

};