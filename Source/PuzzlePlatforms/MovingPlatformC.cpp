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
	
}

void AMovingPlatformC::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		Location += FVector(Speed * DeltaTime, 0, 0);
		SetActorLocation(Location);
	}
}
