// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include <Components/Button.h>

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