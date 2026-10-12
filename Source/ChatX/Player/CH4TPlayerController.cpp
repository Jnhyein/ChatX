#include "Player/CH4TPlayerController.h"
#include "Player/CH4TPlayerState.h"
#include "UI/CH4TChatInput.h"
#include "Game/CH4TGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

ACH4TPlayerController::ACH4TPlayerController()
{
	bReplicates = true;
}

void ACH4TPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UCH4TChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void ACH4TPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ACH4TPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		ACH4TPlayerState* CH4TPS = GetPlayerState<ACH4TPlayerState>();
		if (IsValid(CH4TPS) == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("[1] SetChat Local:%d Msg:%s"), IsLocalController(), *InChatMessageString);

			FString CombinedMessageString = CH4TPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ACH4TPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 5.0f);
}

void ACH4TPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	UE_LOG(LogTemp, Warning, TEXT("[3] ClientRPC Msg:%s"), *InChatMessageString);

	PrintChatMessageString(InChatMessageString);
}

void ACH4TPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	UE_LOG(LogTemp, Warning, TEXT("[2] ServerRPC Msg:%s"), *InChatMessageString);

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ACH4TGameModeBase* CH4TGM = Cast<ACH4TGameModeBase>(GM);
		if (IsValid(CH4TGM) == true)
		{
			CH4TGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}