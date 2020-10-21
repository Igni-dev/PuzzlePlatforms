// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include <Engine/Engine.h>
#include <UObject/ConstructorHelpers.h>
#include <Blueprint/UserWidget.h>

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");

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
        UE_LOG(LogTemp, Warning, TEXT("igni::No found OnlineSubSystem."));
        return;
    }
    SessionInterface = SubSystem->GetSessionInterface();
    if (SessionInterface.IsValid())
    {
        SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
        SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
        SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);
        SessionSearch = MakeShareable(new FOnlineSessionSearch());
        if (SessionSearch.IsValid())
        {
            //SessionSearch->bIsLanQuery = true;
            UE_LOG(LogTemp, Warning, TEXT("igni::Starting Finding OnlineSession."));
            SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
        }
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
    if (SessionInterface.IsValid())
    {
        auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
        if (ExistingSession != nullptr)
        {
            SessionInterface->DestroySession(SESSION_NAME);
        }
        else
        {
            CreateSession();
        }
    }
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

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Succes)
{
    if (!Succes)
    {
        UE_LOG(LogTemp, Warning, TEXT("igni::Could not create session."));
        return;
    }

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

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Succes)
{
    if (Succes)
    {
        CreateSession();
    }
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool Succes)
{
    if (Succes && SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("igni::Finished Finding OnlineSession."));
        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            UE_LOG(LogTemp, Warning, TEXT("igni::Found session names: %s"), *SearchResult.GetSessionIdStr());
        }
    }
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
    if (SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = true;
        SessionSettings.NumPublicConnections = 2;
        SessionSettings.bShouldAdvertise = true;

        SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
    }
}
