// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include <Engine/Engine.h>
#include <UObject/ConstructorHelpers.h>
#include <Blueprint/UserWidget.h>
#include "PlatformTrigger.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

    if (!ensure(MainMenuBPClass.Class))
    {
        return;
    }
    MenuClass = MainMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("igni::Found Class: %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Host()
{
    auto Engine = GetEngine();
    if (!ensure(Engine))
    {
        return;
    }
    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

    UWorld* World = GetWorld();
    if (!ensure(World))
    {
        return;
    }
    World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
    auto Engine = GetEngine();
    if (!ensure(Engine))
    {
        return;
    }
    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Join: %s"), *Address));

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController))
    {
        return;
    }
    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
