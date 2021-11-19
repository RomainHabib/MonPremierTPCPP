// Fill out your copyright notice in the Description page of Project Settings.


#include "SprayDecal.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"

// Sets default values
ASprayDecal::ASprayDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> SprayMat(TEXT("MaterialInstanceConstant'/Game/PinkSprayPaint_Instance.PinkSprayPaint_Instance'"));
	MaterialInstance = CreateDefaultSubobject<UMaterialInstance>("sprayMat");

	if(SprayMat.Succeeded())
	{
		MaterialInstance = SprayMat.Object;
	}

	
	Decal = CreateDefaultSubobject<UDecalComponent>("decal");
	Decal->SetupAttachment((RootComponent));

	Decal->SetDecalMaterial(MaterialInstance);
	Decal->DecalSize = FVector(40.0f,40.0f,40.0f);

}

void ASprayDecal::DestroySprayDecal()
{
	Destroy();
}

// Called when the game starts or when spawned
void ASprayDecal::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
    	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASprayDecal::DestroySprayDecal, 3, false);
}

// Called every frame
void ASprayDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

