#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CH4TPlayerState.generated.h"

UCLASS()
class CHATX_API ACH4TPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;

	FString GetPlayerInfoString();

	ACH4TPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
