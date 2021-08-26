// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ScoreSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FScoreData
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Initials = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Minutes = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Seconds = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Milliseconds = 0;
};

/**
 * 
 */
UCLASS()
class GMTK2021_API UScoreSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FScoreData> HighScores;

	UFUNCTION(BlueprintCallable, Category = "Scores")
	void AddHighScore(FScoreData NewScore);
};
