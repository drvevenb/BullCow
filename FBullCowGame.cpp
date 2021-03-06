#pragma once
#include "FBullCowGame.h"
#include <map>

//to make syntax Unreal friendly
#define TMap std::map 
using int32 = int;

FBullCowGame::FBullCowGame() { Reset(); } //default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

FString FBullCowGame::GetHiddenWord(int32 WordLength)
{
	TMap <int32, FString> HiddenWords{ {3,"ape"}, {4,"fork"}, {5,"table"}, {6,"planet"} };
	MyHiddenWord = HiddenWords[WordLength];
	return MyHiddenWord;
}

int32 FBullCowGame::GetMaxTries() const {
	TMap <int32, int32> WordLengthToMaxTries{ {3,5}, {4,6}, {5,7}, {6,8} };
	return WordLengthToMaxTries [MyHiddenWord.length()];
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "ape"; //this MUST be an isogram
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess)) //if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) //if the guess isn't all lowercase 
	{
		return EGuessStatus::Not_Lowercase; 
	}
	else if (Guess.length() != GetHiddenWordLength()) //if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

//receives a VALID guess, icrements turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); //assuming same length as guess

	//loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) 
	{
		//compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++) 
		{
			//if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar]) 
			{
				if(MHWChar == GChar) //if they're in the same place
				{ 
					BullCowCount.Bulls++; //icrement bulls 
				}
				else 
				{
					BullCowCount.Cows++; //increment cows
				}
			}
		}

	}
	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	//treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap <char, bool> LetterSeen; //setup our map
	for (auto Letter : Word) //for all letters of the word
	{
		Letter = tolower(Letter); //handle mixed case
		if (LetterSeen[Letter]) { //if the letter is in the map
			return false; //we do NOT have an isogram
		} else {
			LetterSeen[Letter] = true;//add the letter to the map as seen
		}
	}
	return true; //for example in cases where /0 is entered
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word) //loop through the guess letters
	{
		if (!islower(Letter)) //if not a lowercase letter
		{
			return false;
		}
	}
	return true;
}
