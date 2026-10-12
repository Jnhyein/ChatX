#include "Player/CH4TPlayerState.h"
#include "Net/UnrealNetwork.h"

ACH4TPlayerState::ACH4TPlayerState()
	: PlayerNameString(TEXT("None"))
	, CurrentGuessCount(0)
	, MaxGuessCount(3)
{
	bReplicates = true;
}

void ACH4TPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString ACH4TPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}