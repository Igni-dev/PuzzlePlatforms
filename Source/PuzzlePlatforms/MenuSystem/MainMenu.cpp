// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include <Components/Button.h>

void UMainMenu::Setup()
{
    this->AddToViewport();

    auto World = GetWorld();
    if (!ensure(World)) { return; }

    auto PlayerController = World->GetFirstPlayerController();
    if (!IsValid(PlayerController)) { return; }

    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);

    PlayerController->bShowMouseCursor = true;
}

void UMainMenu::Teardown()
{
    RemoveFromViewport();

    auto World = GetWorld();
    if (!ensure(World)) { return; }

    auto PlayerController = World->GetFirstPlayerController();
    if (!IsValid(PlayerController)) { return; }

    FInputModeGameOnly InputModeData;
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = false;
}

bool UMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) { return false; }

    if (!IsValid(Host) && !IsValid(Join)) { return false; }
    Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

    return true;
}

void UMainMenu::HostServer()
{
    UE_LOG(LogTemp, Warning, TEXT("I'm gonna host a server"));
    
    if (MenuInterface)
    {
        MenuInterface->Host();
    }
}

void UMainMenu::SetMenuInterface(IMenuInterface* MInterface)
{
    MenuInterface = MInterface;
}