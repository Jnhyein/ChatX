#include "Game/CH4TGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CH4TPlayerController.h"

void ACH4TGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ACH4TPlayerController* CH4TPC = Cast<ACH4TPlayerController>(PC);
			if (IsValid(CH4TPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CH4TPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
