// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Hit TAB to type on the terminal"));
    Isograms = GetValidWords(EnglishWords);
    SetupGame();
    //bDebug = true;

    // PrintLine(TEXT("EnglishWords : %i"), EnglishWords.Num());
    // PrintLine(TEXT("ValidWords : %i"), HiddenWords.Num());
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{

    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
        return;
    }
    if (Input == TEXT("DEBUGON"))
    {
        bDebug = true;
        PrintLine(TEXT("The Hidden Word is: %s."), *HiddenWord); // debug
    }
    if (Input == TEXT("DEBUGOFF"))
    {
        bDebug = false;
    }
    if (Input == TEXT(""))
    {
        return;
    }
    
    ProcessGuess(Input);
    
}

void UBullCowCartridge::SetupGame()
{
    int32 WordIndex = FMath::RandRange(0, Isograms.Num() - 1);
    HiddenWord = Isograms[WordIndex];
    Lives = HiddenWord.Len();
    bGameOver = false;

    if (bDebug)
    {
        PrintLine(TEXT("The Hidden Word is: %s."), *HiddenWord); // debug
    }
    // this is using FString.PrintF template in cartridge.h
    PrintLine(TEXT("Guess the %i letter isogram"), HiddenWord.Len());
    PrintLine(TEXT("You have %i Lives"), Lives);
    PrintLine(TEXT("Type your guess and hit enter"));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine("You have Won!");
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The Hidden Word is %i characters long - try again"), 
            HiddenWord.Len());
        PrintLine(TEXT("You have %i lives remaining"), Lives);
        return;
    }

    // check if isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("Your guess is not an isogram - try again"));
        PrintLine(TEXT("You have %i lives remaining"), Lives);
        return;
    }

    PrintLine("You have lost a life");
    --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("The Hidden Word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    int32 BullCount, CowCount;
    GetBullCows(Guess, BullCount, CowCount);
    PrintLine(TEXT("%i Bulls and %i Cows"), BullCount, CowCount);

    PrintLine(TEXT("You have %i lives left - guess again"), Lives);

}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 i = 0; i < Word.Len(); i++)
    {
        for(int32 j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for(FString Word : WordList)
    {
        if (Word.Len() >= 4 
            && Word.Len() <=8
            && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for(int32 i = 0; i < Guess.Len(); i++)
    {
        for(int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j])
            {
                if (i == j)
                {
                    BullCount++;
                }
                else
                {
                    CowCount++;
                }
                break;
            }
        }
    }    
}