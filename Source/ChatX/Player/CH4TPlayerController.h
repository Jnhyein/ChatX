#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CH4TPlayerController.generated.h"

class UCH4TChatInput;
class UUserWidget;

UCLASS()
class CHATX_API ACH4TPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACH4TPlayerController();

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCH4TChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UCH4TChatInput> ChatInputWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	FString ChatMessageString;
};
