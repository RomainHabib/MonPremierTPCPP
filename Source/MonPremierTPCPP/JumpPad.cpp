// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPad.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UJumpPad::UJumpPad()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSM"));
	StaticMesh->SetSimulatePhysics(true);
	//RootComponent = StaticMesh;

	// ...
}



// Called when the game starts
void UJumpPad::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UJumpPad::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

