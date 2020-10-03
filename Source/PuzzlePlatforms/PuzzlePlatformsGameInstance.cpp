// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include <Engine/Engine.h>
#include <UObject/ConstructorHelpers.h>
#include <Blueprint/UserWidget.h>
#include <OnlineSubsystem.h>

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

    if (!ensure(MainMenuBPClass.Class))
    {
        return;
    }
    MenuClass = MainMenuBPClass.Class;

    ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));

    if (!ensure(InGameMenuBPClass.Class))
    {
        return;
    }
    InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{

    IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();
    if (!SubSystem) {
        UE_LOG(LogTemp, Warning, TEXT("igni::No found SubSystem."));
        return;
    }
    //UE_LOG(LogTemp, Warning, TEXT("igni::Found SessionInterface: %s"), *SubSystem->GetSubsystemName().ToString());
    IOnlineSessionPtr SessionInterface = SubSystem->GetSessionInterface();
    if (SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("igni::Found SessionInterface."));
    }

}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
    if (!IsValid(MenuClass)) { return; }

    Menu = CreateWidget<UMainMenu>(this, MenuClass);

    if (!IsValid(Menu)) { return; }
    
    Menu->Setup();
    
    Menu->SetMenuInterface(this);

}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
    if (!IsValid(InGameMenuClass)) { return; }

    UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);

    if (!IsValid(InGameMenu)) { return; }

    InGameMenu->Setup();

    InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
    if (Menu != nullptr)
    {
        Menu->Teardown();
    }

    auto Engine = GetEngine();
    if (!ensure(Engine))
    {
        return;
    }
    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

    UWorld* World = GetWorld();
    if (!ensure(World)) { return; }

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

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController))
    {
        return;
    }
    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}
