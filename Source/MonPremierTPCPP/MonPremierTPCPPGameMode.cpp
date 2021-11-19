// Copyright Epic Games, Inc. All Rights Reserved.

#include "MonPremierTPCPPGameMode.h"
#include "MonPremierTPCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMonPremierTPCPPGameMode::AMonPremierTPCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Player_BP.Player_BP_C'"));
	
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
