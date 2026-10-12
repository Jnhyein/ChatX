#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CH4TGameStateBase.generated.h"

UCLASS()
class CHATX_API ACH4TGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));

};
