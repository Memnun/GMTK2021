// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GMTK2021_API ComponentData
{
public:
	ComponentData(int health, float speed, int jumps, int maxEnergy, float regenEnergy, int shotDamage, float rate, float cost);
	~ComponentData();
	int maxHealth;
	int currentHealth;
	float speedBonus;
	int maxJumps;
	int maxEnergyBonus;
	float regenEnergyBonus;
	int damage;
	float fireSpeed;
	float energyCost;
};
