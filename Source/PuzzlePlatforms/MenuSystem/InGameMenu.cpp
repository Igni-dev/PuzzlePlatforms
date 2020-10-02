// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Components/EditableTextBox.h>

void UInGameMenu::CancelPressed()
{
    Teardown();
}

void UInGameMenu::QuitPressed()
{
    if (MenuInterface)
    {
        Teardown();
        MenuInterface->LoadMainMenu();
    }
}

bool UInGameMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) { return false; }

    if (!IsValid(Cancel) && !IsValid(Quit)) { return false; }

    Cancel->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);
    Quit->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

    return true;
}