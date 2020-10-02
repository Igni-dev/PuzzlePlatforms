// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::Setup()
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

void UMenuWidget::Teardown()
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

void UMenuWidget::SetMenuInterface(IMenuInterface* MInterface)
{
    MenuInterface = MInterface;
}