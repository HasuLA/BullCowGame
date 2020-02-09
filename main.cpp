/* This is the console executable, that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/
#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"
#include <map>

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// function prototypes as outside a class
int32 AskForWordLength(); //asking user for 
bool ValidateWordLength(const int32 &MyHiddenWordLength);
void PrintIntro();
void SelectWord(); // Selecting word with length pointed by user
void InviteToPlay();
void PlayGame();
void ShowCurrentTries();
FText AskForValidGuess();
void ValidateGuessStatus(EGuessStatus);
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; //instantiate a new game, which we reuse across plays

int main()
{
	bool bPlayAgain = false;
	do
	{
		PrintIntro();
		SelectWord();
		InviteToPlay();
		PlayGame();
		bPlayAgain = AskToPlayAgain();

	} while (bPlayAgain);
	return 0;
}

void PrintIntro()
{
	std::cout << "\n\n";
	std::cout << "Welcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------\\ /          \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O            O| COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << std::endl;
	std::cout << "\n\nPlease, input length of isogram (from 3 to 7): ";
}

void InviteToPlay()
{
	std::cout << "\n\n";
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
}

int32 AskForWordLength()
{
	int32 MyHiddenWordLength = 0;

	do {
		std::cin >> MyHiddenWordLength;
	} while (!ValidateWordLength(MyHiddenWordLength));

	return MyHiddenWordLength;
}

bool ValidateWordLength(const int32 &MyHiddenWordLength)
{
	int32 MaxLength = BCGame.GetMaxWordLength();
	int32 MinLength = BCGame.GetMinWordLength();

	if (std::cin.fail() || MyHiddenWordLength < MinLength || MyHiddenWordLength > MaxLength)
	{
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << "Please input a number from 3 to 7" << std::endl;
		return false;
	}

	return true;
}

void SelectWord()
{
	BCGame.ChooseHiddenWord(AskForWordLength());
	return;
}

// plays a single game to completion
void PlayGame()
{
	BCGame.Reset();

	// loop asking for guesses while the game
	while (BCGame.CanContinuePlaying()) {
		FText Guess = AskForValidGuess();

		BCGame.IncrementCurrentTries();
		FBullCowCount BullCowCount = BCGame.CalculateGuessScore(Guess);
		BCGame.CheckIfGuessCorrect(BullCowCount); // Is it a win guess?
		BCGame.PrintScorePoints(BullCowCount);
	}

	PrintGameSummary();
	return;
}

void ShowCurrentTries()
{
	int32 CurrentTry = BCGame.GetCurrentTry();
	std::cout << "Try" << CurrentTry << " of " << BCGame.GetMaxTries() << ". ";
}

void ValidateGuessStatus(EGuessStatus Status)
{
	switch (Status)
	{

	case EGuessStatus::Wrong_Length:
		std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
		break;
	case EGuessStatus::Not_Isogram:
		std::cout << "Please enter a word without repeating letters. \n\n";
		break;
	case EGuessStatus::Not_Lowercase:
		std::cout << "Please dont use uppercase\n\n";
		break;
	case EGuessStatus::No_Spaces_Allowed:
		std::cout << "Please enter all lowercase letters.\n\n";
		break;
	default:
		//assume the guess is valid
		break;
	}
	return;
}

// loop continually until the user gives a valid guess
FText AskForValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;

	do {
		// get a guess from the player
		ShowCurrentTries();
		std::cout << "Enter your guess: ";
		std::cin >> Guess;

		Status = BCGame.CheckGuessValidity(Guess);
		ValidateGuessStatus(Status);

	} while (Status != EGuessStatus::OK); // keep looping until we get no errors

	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "Do you wanna play again? y/n\n";
	FText Response = "";
	std::cin >> Response;
	return (Response[0] == 'y' || Response[0] == 'Y');
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon()) {
		std::cout << "WELL DONE - YOU WIN!\n ";
	}
	else
	{
		std::cout << "BETTER LUCK NEXT TIME\n";
	}
	return;
}


