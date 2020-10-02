// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Components/EditableTextBox.h>


bool UMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) { return false; }

    if (!IsValid(Host) && !IsValid(Join)) { return false; }
    Exit->OnClicked.AddDynamic(this, &UMainMenu::ExitPressed);
    Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
    Join->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
    Back->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
    ConfirmJoin->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    return true;
}

void UMainMenu::ExitPressed()
{
    auto World = GetWorld();
    if (!ensure(World)) { return; }

    auto PlayerController = World->GetFirstPlayerController();
    if (!IsValid(PlayerController)) { return; }

    PlayerController->ConsoleCommand("quit");
}

void UMainMenu::HostServer()
{
    UE_LOG(LogTemp, Warning, TEXT("I'm gonna host a server"));
    
    if (MenuInterface)
    {
        MenuInterface->Host();
    }
}

void UMainMenu::JoinServer()
{
    UE_LOG(LogTemp, Warning, TEXT("I'm gonna join to server"));

    if (MenuInterface)
    {
        FString Address = IPAddressField->GetText().ToString();
        MenuInterface->Join(Address);
    }
}

void UMainMenu::OpenJoinMenu()
{
    if (!IsValid(MenuSwitcher) && !IsValid(JoinMenu)) { return; }
    MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::BackToMainMenu()
{
    if (!IsValid(MenuSwitcher) && !IsValid(MainMenu)) { return; }
    MenuSwitcher->SetActiveWidget(MainMenu);
}

