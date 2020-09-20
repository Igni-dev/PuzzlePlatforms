// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include <Engine/Engine.h>


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance"));
}

void UPuzzlePlatformsGameInstance::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("Init"));
}

void UPuzzlePlatformsGameInstance::Host()
{
    auto Engine = GetEngine();
    if (!ensure(Engine))
    {
        return;
    }
    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
    auto Engine = GetEngine();
    if (!ensure(Engine))
    {
        return;
    }

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Join: %s"), *Address));
}
