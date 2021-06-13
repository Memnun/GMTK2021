// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadoutController.h"

LoadoutController::LoadoutController()
{
}

LoadoutController::~LoadoutController()
{
}

float LoadoutController::GetFireSpeed(bool right) {
	return 0.0;
}

int LoadoutController::GetDamage(bool right) {
	return 0;
}

float LoadoutController::GetMovespeedModifier() {
	return 1.0;
}

int LoadoutController::GetCurrentHealth() {
	return 0;
}

void LoadoutController::ApplyDamage(int damage) {
	return;
}