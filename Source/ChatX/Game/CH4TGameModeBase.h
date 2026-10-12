#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH4TGameModeBase.generated.h"

class ACH4TPlayerController;

UCLASS()
class CHATX_API ACH4TGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual void BeginPlay() override;

	void PrintChatMessageString(ACH4TPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	void IncreaseGuessCount(ACH4TPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(ACH4TPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ACH4TPlayerController>> AllPlayerControllers;
};
