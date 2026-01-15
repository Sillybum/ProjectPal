// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPalGameMode.h"
#include "ProjectPalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectPalGameMode::AProjectPalGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
