#include "Game/CH4TGameModeBase.h"
#include "Game/CH4TGameStateBase.h"
#include "Player/CH4TPlayerController.h"
#include "Player/CH4TPlayerState.h"
#include "EngineUtils.h"


void ACH4TGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACH4TPlayerController* CH4TPlayerController = Cast<ACH4TPlayerController>(NewPlayer);
	if (IsValid(CH4TPlayerController) == true)
	{
		CH4TPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		AllPlayerControllers.Add(CH4TPlayerController);

		ACH4TPlayerState* CH4TPS = CH4TPlayerController->GetPlayerState<ACH4TPlayerState>();
		if (IsValid(CH4TPS) == true)
		{
			CH4TPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACH4TGameStateBase* CH4TGameStateBase = GetGameState<ACH4TGameStateBase>();
		if (IsValid(CH4TGameStateBase) == true)
		{
			CH4TGameStateBase->MulticastRPCBroadcastLoginMessage(CH4TPS->PlayerNameString);
		}
	}
}

void ACH4TGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}


void ACH4TGameModeBase::PrintChatMessageString(ACH4TPlayerController* InChattingPlayerController,const FString& InChatMessageString)
{
	int32 Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);

	bool bIsGuessNumber = IsGuessNumberString(GuessNumberString);

	if (bIsGuessNumber == true)
	{
		IncreaseGuessCount(InChattingPlayerController);

		FString JudgeResultString =
			JudgeResult(SecretNumberString, GuessNumberString);

		FString CombinedMessageString =
			InChatMessageString + TEXT(" -> ") + JudgeResultString;

		for (TActorIterator<ACH4TPlayerController> It(GetWorld()); It; ++It)
		{
			ACH4TPlayerController* CH4TPlayerController = *It;
			if (IsValid(CH4TPlayerController) == true)
			{
				CH4TPlayerController->ClientRPCPrintChatMessageString(
					CombinedMessageString);
			}
		}

		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		for (TActorIterator<ACH4TPlayerController> It(GetWorld()); It; ++It)
		{
			ACH4TPlayerController* CH4TPlayerController = *It;
			if (IsValid(CH4TPlayerController) == true)
			{
				CH4TPlayerController->ClientRPCPrintChatMessageString(
					InChatMessageString);
			}
		}
	}
}


FString ACH4TGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ACH4TGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ACH4TGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACH4TGameModeBase::IncreaseGuessCount(ACH4TPlayerController* InChattingPlayerController)
{
	ACH4TPlayerState* CH4TPS = InChattingPlayerController->GetPlayerState<ACH4TPlayerState>();
	if (IsValid(CH4TPS) == true)
	{
		CH4TPS->CurrentGuessCount++;
	}
}

void ACH4TGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& CH4TPlayerController : AllPlayerControllers)
	{
		ACH4TPlayerState* CH4TPS = CH4TPlayerController->GetPlayerState<ACH4TPlayerState>();
		if (IsValid(CH4TPS) == true)
		{
			CH4TPS->CurrentGuessCount = 0;
		}
	}
}

void ACH4TGameModeBase::JudgeGame(ACH4TPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ACH4TPlayerState* CH4TPS = InChattingPlayerController->GetPlayerState<ACH4TPlayerState>();
		for (const auto& CH4TPlayerController : AllPlayerControllers)
		{
			if (IsValid(CH4TPS) == true)
			{
				FString CombinedMessageString = CH4TPS->PlayerNameString + TEXT(" has won the game.");
				CH4TPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CH4TPlayerController : AllPlayerControllers)
		{
			ACH4TPlayerState* CH4TPS = CH4TPlayerController->GetPlayerState<ACH4TPlayerState>();
			if (IsValid(CH4TPS) == true)
			{
				if (CH4TPS->CurrentGuessCount < CH4TPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& CXPlayerController : AllPlayerControllers)
			{
				CXPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}
