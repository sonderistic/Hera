// Copyright Epic Games, Inc. All Rights Reserved.


#include "HeraGameModeBase.h"
#include "MarksmanCharacter.h"

AHeraGameModeBase::AHeraGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bAllowTickOnDedicatedServer = false;

    DefaultPawnClass = AMarksmanCharacter::StaticClass();

}