// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSaveGame.h"

void UScoreSaveGame::AddHighScore(FScoreData NewScore)
{
	HighScores.Add(NewScore);
	HighScores.Sort([](const FScoreData& A, const FScoreData& B)
	{
		if (A.Minutes != B.Minutes)
			return A.Minutes > B.Minutes;
		if (A.Seconds != B.Seconds)
			return A.Seconds > B.Seconds;
		if (A.Milliseconds != B.Milliseconds)
			return A.Milliseconds > B.Milliseconds;
		return A.Initials > B.Initials;
	});

	const int32 MaxSize = 10000000;

	if(HighScores.Num() > MaxSize)
		HighScores.RemoveAt(MaxSize);
}
