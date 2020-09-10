// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatformC.h"

AMovingPlatformC::AMovingPlatformC()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatformC::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatformC::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		float JourneyLenght = (GlobalTargetLocation - GlobalStartLocation).Size();
		float JourneyTravelled = (Location - GlobalStartLocation).Size();

		if (JourneyTravelled >= JourneyLenght)
		{
			Swap(GlobalStartLocation, GlobalTargetLocation);
		}
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		Location += (Speed * DeltaTime * Direction);
		SetActorLocation(Location);
	}
}
