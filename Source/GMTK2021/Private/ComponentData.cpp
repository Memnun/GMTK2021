// Fill out your copyright notice in the Description page of Project Settings.


#include "ComponentData.h"

ComponentData::ComponentData(int health, float speed, int jumps, int maxEnergy, float regenEnergy, int shotDamage, float rate, float cost)
{
	int maxHealth = health;
	int currentHealth = health;
	float speedBonus = speed;
	int maxJumps = jumps;
	int maxEnergyBonus = maxEnergy;
	float regenEnergyBonus = regenEnergy;
	int damage = shotDamage;
	float fireSpeed = rate;
	float energyCost = cost;
}

ComponentData::~ComponentData()
{
}
