/* The game logic (no view code or direct user interaction)
The game is a simple guess the word game based on Mastermind
*/
#define TMap std::map 
#pragma once
#include <string>
#include <vector>

// to make syntax Unreal friendly
using FString = std::string;
using int32 = int;

struct FBullCowCount 
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase,
	No_Spaces_Allowed
};

class FBullCowGame {
public:	
	FBullCowGame(); //constructor
	void Reset(); //initializes some of private members

	int32 GetMaxTries() const;
	int32 GetCurrentTry() const { return MyCurrentTry; } 
	int32 GetHiddenWordLength() const { return MyHiddenWord.length(); }
	int32 GetMinWordLength() const { return MinWordLength; }
	int32 GetMaxWordLength() const { return MaxWordLength; }
	std::vector<FString> GetBCGameVocabulary() const { return BCGameVocabulary; }

	void IncrementCurrentTries();
	FBullCowCount CalculateGuessScore(FString);
	void CheckIfGuessCorrect(FBullCowCount);
	void PrintScorePoints(FBullCowCount);
	FString ChooseHiddenWord(int32);
	EGuessStatus CheckGuessValidity(FString) const;

	bool IsGameWon() const { return bGameIsWon; }
	bool CanContinuePlaying() const;


private: // see constructor for initialization
	FString MyHiddenWord = "";
	int32 MyCurrentTry; 

	bool bGameIsWon; 
	bool IsIsogram(FString) const;  
	bool IsLowerCase(FString) const; 
	bool IsCorrectLength(FString) const;
	

	int32 MinWordLength = 3;
	int32 MaxWordLength = 7;
	
	std::vector<FString> BCGameVocabulary
	{
		"son", "cat", "gay",
		"unit", "unit", "unit",
		"strom", "plane", "train",
		"planet", "search", "hotkey",
		"exactly", "isogram", "complex"
	};
};