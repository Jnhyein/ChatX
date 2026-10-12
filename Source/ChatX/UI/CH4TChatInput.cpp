#include "UI/CH4TChatInput.h"
#include "Player/CH4TPlayerController.h"
#include "Components/EditableTextBox.h"

void UCH4TChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UCH4TChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UCH4TChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ACH4TPlayerController* OwningCH4TPlayerController = Cast<ACH4TPlayerController>(OwningPlayerController);
			if (IsValid(OwningCH4TPlayerController) == true)
			{
				OwningCH4TPlayerController->SetChatMessageString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
