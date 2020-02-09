#pragma once
#include "FBullCowGame.h"
#include <map>
#include <iostream>
#include <vector>
#include <random>
// to make syntax Unreal friendly
using int32 = int;
using FString = std::string;

FBullCowGame::FBullCowGame() { Reset(); } //default constructor

bool FBullCowGame::CanContinuePlaying() const
{
	int32 MaxTries = GetMaxTries();
	return (!IsGameWon() && GetCurrentTry() <= MaxTries); // is NOT won and there are still tries remaining
}

int32 FBullCowGame::GetMaxTries() const
{
	TMap<int32, int32> WordLengthToMaxTries{ {3, 4}, {4, 7}, {5,10}, {6,16}, {7, 20} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

FString FBullCowGame::ChooseHiddenWord(int32 IsogramLength)
{
	std::vector<FString> ReuquiredLengthWords;
	
	for (int i = 0; i < BCGameVocabulary.size(); i++) {
		if (BCGameVocabulary[i].length() == IsogramLength) {
			ReuquiredLengthWords.push_back(BCGameVocabulary[i]);
		}
	}

	MyHiddenWord = ReuquiredLengthWords[rand() % 2];

	return MyHiddenWord;
}

void FBullCowGame::Reset()
{
	MyCurrentTry = 1;
	bGameIsWon = false;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	//treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen;
	for (auto Letter : Word)
	{
		Letter = tolower(Letter); // handle mixed case
			if (LetterSeen[Letter]) { //if the letter is in the map
				return false; // we do NOT have an isogram
			}
			else {
				LetterSeen[Letter] = true; // add the letter to the map as seen
			}
	}
	
	return true;
}

bool FBullCowGame::IsLowerCase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (isupper(Letter)) {
			return false;
		}
	}

	return true;
}

bool FBullCowGame::IsCorrectLength(FString Guess) const
{
	return (Guess.length() == GetHiddenWordLength());
}


EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess))
	{
		return EGuessStatus::Not_Isogram; 
	}
	else if (!IsLowerCase(Guess)) 
	{
		return EGuessStatus::Not_Lowercase; 
	}

	else if (!IsCorrectLength(Guess)) 
	{
		return EGuessStatus::Wrong_Length;
	}
	else { 
		return EGuessStatus::OK;
	}
}

void FBullCowGame::IncrementCurrentTries()
{
	MyCurrentTry++;
}

// recei ves a VALID guess, incriments turn, and returns count
FBullCowCount FBullCowGame::CalculateGuessScore(FString Guess) 
{
	 
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); //assuming the same length as guess
	
	//loop trough all letters in the hidden word
	for (int32 MHWCharIndex = 0; MHWCharIndex < WordLength; MHWCharIndex++)
	{
		//compare letter against the hidden word
		for (int32 GCharIndex = 0; GCharIndex < WordLength; GCharIndex++)
		{
			if (Guess[GCharIndex] == MyHiddenWord[MHWCharIndex])
			{
				if (MHWCharIndex == GCharIndex) { //if ther're in the same place
					BullCowCount.Bulls++;
				}
				else {
					BullCowCount.Cows++; //must be a cow
				}
			}
		}
	}

	return BullCowCount;
}

void FBullCowGame::CheckIfGuessCorrect(FBullCowCount CalculatedGuess)
{
	if (CalculatedGuess.Bulls == MyHiddenWord.length() )
	{
		bGameIsWon = true;
	}
}

void FBullCowGame::PrintScorePoints(FBullCowCount BullCowCount)
{
	std::cout << "Bulls = " << BullCowCount.Bulls;
	std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
}

